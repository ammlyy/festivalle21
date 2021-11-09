/*
  ==============================================================================

    ArousalValenceStrategy.h
    Created: 9 Nov 2021 8:25:31pm
    Author:  wadda

  ==============================================================================
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
class ArousalValenceStrategy  : public Strategy
{
public:
    ArousalValenceStrategy(juce::AudioProcessorValueTreeState* treeState);
    ~ArousalValenceStrategy();

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

    juce::AudioProcessorValueTreeState* treeState;

    float avgValence;
    float avgArousal;
};
