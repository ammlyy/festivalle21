/*
  ==============================================================================

    TutorialStrategy.cpp
    Created: 30 Mar 2022 10:50:36pm
    Author:  wadda

  ==============================================================================
*/

#include "TutorialStrategy.h"

TutorialStrategy::TutorialStrategy(juce::AudioProcessorValueTreeState* treeState)
{
    this->treeState = treeState;
    this->addresses.push_back("/juce/Tutorial");
    this->data.push_back(.0f);
}

TutorialStrategy::~TutorialStrategy()
{
}

void TutorialStrategy::processBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, juce::OSCSender* sender)
{
    if (*treeState->getRawParameterValue("enableSend") == 1.0f) {
        auto max = buffer.getMagnitude(0, buffer.getNumSamples());
        sender->send(this->addresses[0], juce::OSCArgument(max));
        this->data[0] = max;
    }
}
