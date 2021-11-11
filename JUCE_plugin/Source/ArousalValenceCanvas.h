/*
  ==============================================================================

    ArousalValenceCanvas.h
    Created: 11 Nov 2021 3:51:11pm
    Author:  wadda

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Strategy.h"

//==============================================================================
/*
*/
class ArousalValenceCanvas  : public juce::Component
{
public:
    ArousalValenceCanvas(juce::AudioProcessorValueTreeState* valueTreeState, juce::Rectangle<int> area, Strategy* analisysStrategy);
    ~ArousalValenceCanvas() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState* valueTreeState;
    juce::Rectangle<int> area;
    Strategy* analisysStrategy;
};
