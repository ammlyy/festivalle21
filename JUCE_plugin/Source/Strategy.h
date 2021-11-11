/*
  ==============================================================================

    Strategy.h
    Created: 9 Nov 2021 8:15:27pm
    Author:  wadda

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//==============================================================================
/*
*/
class Strategy
{
public:

    std::vector<juce::String> getAddresses();
    std::vector<float> getData();

    virtual void processBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, juce::OSCSender* sender);

protected:
    std::vector<juce::String> addresses;
    std::vector<float> data;

};