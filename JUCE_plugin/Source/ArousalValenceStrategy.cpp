/*
  ==============================================================================

    ArousalValenceStrategy.cpp
    Created: 9 Nov 2021 8:25:31pm
    Author:  wadda

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ArousalValenceStrategy.h"

//==============================================================================

ArousalValenceStrategy::ArousalValenceStrategy(juce::AudioProcessorValueTreeState* treeState)
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
    this->addresses.push_back("/juce/AV/A");
    this->addresses.push_back("/juce/AV/V");
    this->data.push_back(0.0f);
    this->data.push_back(0.0f);
}

ArousalValenceStrategy::~ArousalValenceStrategy()
{
}

void ArousalValenceStrategy::processBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, juce::OSCSender* sender)
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
                this->currentAVindex = 0;
                sender->send("/juce/AV/A", juce::OSCArgument(this->avgArousal));
                sender->send("/juce/AV/V", juce::OSCArgument(this->avgValence));
                this->data[0] = this->avgArousal;
                this->data[1] = this->avgValence;
            }
        }
    }
}

std::vector<float> ArousalValenceStrategy::predictAV(juce::AudioBuffer<float> buffer)
{
    std::vector<float> my_vector{ buffer.getReadPointer(0), buffer.getReadPointer(0) + BUFFER_SIZE };
    const fdeep::tensor input(fdeep::tensor_shape(22050, 1), my_vector);

    const fdeep::tensors result = model.predict({ input });

    return result[0].to_vector();
}

// Calculate the average Valence and Arousal (from the av vector) and store them in the attributes 
void ArousalValenceStrategy::averageAV(std::vector<std::vector<float>> av)
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
