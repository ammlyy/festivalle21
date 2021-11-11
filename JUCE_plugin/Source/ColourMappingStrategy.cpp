/*
  ==============================================================================

    ColourMappingStrategy.cpp
    Created: 11 Nov 2021 12:05:08pm
    Author:  wadda

  ==============================================================================
*/

#include "ColourMappingStrategy.h"

ColourMappingStrategy::ColourMappingStrategy(juce::AudioProcessorValueTreeState* treeState)
{
    this->treeState = treeState;
    this->currentAVindex = 0;
    this->bufferToFillSampleIdx = 0;
    this->bufferToFill.setSize(1, BUFFER_SIZE);
    this->av = std::vector<std::vector<float>>(CHANGE_FREQUENCY, std::vector<float>(2, 0));
    this->avgArousal = 0;
    this->avgValence = 0;
    this->data = std::vector<float>();
    this->addresses = std::vector<juce::String>();
    this->addresses.push_back("/juce/RGB/R");
    this->addresses.push_back("/juce/RGB/G");
    this->addresses.push_back("/juce/RGB/B");
    this->data.push_back(0.0f);
    this->data.push_back(0.0f);
}

ColourMappingStrategy::ColourMappingStrategy()
{
}

void ColourMappingStrategy::processBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, juce::OSCSender* sender)
{
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        float monoSample = 0.0;

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* inputChannelData = buffer.getReadPointer(channel);
            monoSample += inputChannelData[sample];
        }

        monoSample /= totalNumInputChannels;
        this->bufferToFill.getWritePointer(0)[this->bufferToFillSampleIdx] = monoSample;

        this->bufferToFillSampleIdx++;
        if (this->bufferToFillSampleIdx == BUFFER_SIZE)
        {
            this->bufferToFillSampleIdx = 0;

            this->av.at(currentAVindex) = this->predictAV(this->bufferToFill);

            /*
            this->rms = this->bufferToFill.getRMSLevel(0, 0, BUFFER_SIZE);
            this->rms = max((20 * log(this->rms) / 2)/3, -100.f);               //convert to dB
            float brightness = 1.0 + this->rms;

            sender.send("/juce/brightness", juce::OSCArgument(brightness));
            */

            this->currentAVindex++;
            if (this->currentAVindex == CHANGE_FREQUENCY) {
                this->averageAV(this->av);
                std::vector<float> msg = this->calculateRGB(this->avgValence, this->avgArousal);
                // create and send an OSC message with an address and a float value:
                sender->send("/juce/RGB/R", juce::OSCArgument(msg[0]));
                sender->send("/juce/RGB/G", juce::OSCArgument(msg[1]));
                sender->send("/juce/RGB/B", juce::OSCArgument(msg[2]));
                this->currentAVindex = 0;
                this->data[0] = this->avgArousal;
                this->data[1] = this->avgValence;
            }
        }
    }
}

std::vector<float> ColourMappingStrategy::predictAV(juce::AudioBuffer<float> buffer)
{
    std::vector<float> my_vector{ buffer.getReadPointer(0), buffer.getReadPointer(0) + BUFFER_SIZE };
    const fdeep::tensor input(fdeep::tensor_shape(22050, 1), my_vector);

    const fdeep::tensors result = model.predict({ input });

    return result[0].to_vector();
}

void ColourMappingStrategy::averageAV(std::vector<std::vector<float>>)
{
    int isManual = *treeState->getRawParameterValue("isManual");

    float avg_valence = 0.0f;
    float avg_arousal = 0.0f;

    if (!isManual) {
        for (int i = 0; i < CHANGE_FREQUENCY; i++) {
            avg_valence += av[i][1];
            avg_arousal += av[i][0];
        }
        avg_valence /= av.size();
        avg_arousal /= av.size();

        float rotationAngle = *treeState->getRawParameterValue("rotationAngle");
        float newAvgVal = avg_valence * cos(rotationAngle * PI / 180.0) - avg_arousal * sin(rotationAngle * PI / 180.0);
        float newAvgArous = avg_valence * sin(rotationAngle * PI / 180.0) + avg_arousal * cos(rotationAngle * PI / 180.0);

        this->avgValence = std::min(1.f, newAvgVal * SCALING_FACTOR);
        this->avgArousal = std::min(1.f, newAvgArous * SCALING_FACTOR);
    }
    else {
        float radius = *treeState->getRawParameterValue("manualRadius");
        float angle = *treeState->getRawParameterValue("rotationAngle");
        this->avgValence = radius * cos(angle * PI / 180.0);
        this->avgArousal = radius * sin(angle * PI / 180.0);
    }
}

std::vector<float> ColourMappingStrategy::calculateRGB(float valence, float arousal)
{
    float R = 0.0f;
    float Y = 0.0f;
    float B = 0.0f;

    float H = (atan2(valence, arousal) * 180.0 / PI) + 30;    // Hue
    if (H < 0) {
        H = 360.0 + H;
    }
    //DBG("H: " + std::to_string(H));
    float S = std::min(std::max(sqrt(pow(valence, 2) + pow(arousal, 2)), 0.6), 1.0);    // Saturation (distance)
    float V = 1;  //Intensity

    float C = V * S;
    float X = C * (1.0 - std::abs(std::fmod((H / 60.0), 2.0) - 1.0));
    float m = V - C;

    if (H >= 0 && H < 60) {
        R = C;
        Y = X;
        B = 0;
    }
    else if (H >= 60 && H < 120) {
        R = X;
        Y = C;
        B = 0;
    }
    else if (H >= 120 && H < 180) {
        R = 0;
        Y = C;
        B = X;
    }
    else if (H >= 180 && H < 240) {
        R = 0;
        Y = X;
        B = C;
    }
    else if (H >= 240 && H < 300) {
        R = X;
        Y = 0;
        B = C;
    }
    else if (H >= 300 && H <= 360) {
        R = C;
        Y = 0;
        B = X;
    }

    R = (R + m);
    Y = (Y + m);
    B = (B + m);

    std::vector<float> rgbValues = ryb2RGB(R, Y, B);
    treeState->getRawParameterValue("bypassRYB") ? rgbValues = { R, Y, B } : rgbValues = ryb2RGB(R, Y, B);
    /*for (auto& el : rgbValues) {
        el *= 255.0f;
    }*/

    /*DBG("R: " + std::to_string(rgbValues[0]));
    DBG("G: " + std::to_string(rgbValues[1]));
    DBG("B: " + std::to_string(rgbValues[2]));*/

    return rgbValues;
}

float ColourMappingStrategy::cubicInterp(float t, float A, float B)
{
    float w = t * t * (3.0f - 2.0f * t);

    return A + w * (B - A);
}

std::vector<float> ColourMappingStrategy::ryb2RGB(float r, float y, float b)
{
    std::vector<float> rgb(3);

    float x0 = this->cubicInterp(b, RYB_COLORS[0][0], RYB_COLORS[4][0]);
    float x1 = this->cubicInterp(b, RYB_COLORS[1][0], RYB_COLORS[5][0]);
    float x2 = this->cubicInterp(b, RYB_COLORS[2][0], RYB_COLORS[6][0]);
    float x3 = this->cubicInterp(b, RYB_COLORS[3][0], RYB_COLORS[7][0]);
    float y0 = this->cubicInterp(y, x0, x1);
    float y1 = this->cubicInterp(y, x2, x3);

    rgb[0] = this->cubicInterp(r, y0, y1);

    x0 = this->cubicInterp(b, RYB_COLORS[0][1], RYB_COLORS[4][1]);
    x1 = this->cubicInterp(b, RYB_COLORS[1][1], RYB_COLORS[5][1]);
    x2 = this->cubicInterp(b, RYB_COLORS[2][1], RYB_COLORS[6][1]);
    x3 = this->cubicInterp(b, RYB_COLORS[3][1], RYB_COLORS[7][1]);
    y0 = this->cubicInterp(y, x0, x1);
    y1 = this->cubicInterp(y, x2, x3);

    rgb[1] = this->cubicInterp(r, y0, y1);

    x0 = this->cubicInterp(b, RYB_COLORS[0][2], RYB_COLORS[4][2]);
    x1 = this->cubicInterp(b, RYB_COLORS[1][2], RYB_COLORS[5][2]);
    x2 = this->cubicInterp(b, RYB_COLORS[2][2], RYB_COLORS[6][2]);
    x3 = this->cubicInterp(b, RYB_COLORS[3][2], RYB_COLORS[7][2]);
    y0 = this->cubicInterp(y, x0, x1);
    y1 = this->cubicInterp(y, x2, x3);

    rgb[2] = this->cubicInterp(r, y0, y1);

    return rgb;
}


