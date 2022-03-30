/*
  ==============================================================================

    TutorialStrategy.h
    Created: 30 Mar 2022 10:50:36pm
    Author:  wadda

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Strategy.h"

class TutorialStrategy : public Strategy
{
public:
    TutorialStrategy();
    ~TutorialStrategy();

    void processBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, juce::OSCSender* sender) override;

};
