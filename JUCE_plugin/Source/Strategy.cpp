/*
  ==============================================================================

    Strategy.cpp
    Created: 9 Nov 2021 8:15:27pm
    Author:  wadda

  ==============================================================================
*/

#include "Strategy.h"

std::vector<juce::String> Strategy::getAddresses()
{
    return this->addresses;
}

std::vector<float> Strategy::getData()
{
    return this->data;
}

void Strategy::processBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, juce::OSCSender* sender)
{
}
