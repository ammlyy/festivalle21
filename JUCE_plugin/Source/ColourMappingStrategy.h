/*
  ==============================================================================

    ColourMappingStrategy.h
    Created: 11 Nov 2021 12:05:08pm
    Author:  wadda

  ==============================================================================

  N.B. getData() still returns Arousal and Valence since it is needed to properly draw the canvas

*/

#pragma once

#include <JuceHeader.h>
#include "Strategy.h"
#include <fdeep/fdeep.hpp>

#define CHANGE_FREQUENCY 4 // number of 500ms windows analyzed before sending new data (2seconds)
#define SCALING_FACTOR 3
#define PI 3.14159265
#define BUFFER_SIZE 22050 //500 ms at 44.1kHz

//==============================================================================
/*
*/
class ColourMappingStrategy : public Strategy
{
public:
    ColourMappingStrategy(juce::AudioProcessorValueTreeState* treeState);
    ColourMappingStrategy();

    void processBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, juce::OSCSender* sender) override;

private:
    const std::string pathToModel = "D:\\GitHub\\OSC_toolkit\\JUCE_plugin\\Source\\models\\exported\\exported_model.json";     //Insert here the path to model.json
    fdeep::model model = fdeep::load_model(this->pathToModel);

    int currentAVindex;
    juce::AudioBuffer<float> bufferToFill;
    int bufferToFillSampleIdx;

    std::vector<std::vector<float>> av;

    std::vector<float> predictAV(juce::AudioBuffer<float> buffer);
    void averageAV(std::vector<std::vector<float>>);
    std::vector<float> calculateRGB(float valence, float arousal);
    float cubicInterp(float t, float A, float B);
    std::vector<float> ryb2RGB(float r, float y, float b);

    juce::AudioProcessorValueTreeState* treeState;

    float avgValence;
    float avgArousal;

    float RYB_COLORS[8][3] = {
        {1.0, 1.0, 1.0},
        { 1.0, 1.0 , 0.0 },
        { 1.0, 0.0, 0.0 },
        { 1.0, .5, 0.0 },
        {0.163, 0.373, 0.6},
        { 0.0, 0.66, 0.2 },
        { .5, 0.0, 0.5 },
        { 0.2, 0.094, 0.0 }
    };
};

