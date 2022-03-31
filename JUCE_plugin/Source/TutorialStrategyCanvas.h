/*
  ==============================================================================

    TutorialStrategyCanvas.h
    Created: 30 Mar 2022 10:34:42pm
    Author:  wadda

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TutorialStrategyCanvas  : public juce::Component
{
public:
    TutorialStrategyCanvas(juce::AudioProcessorValueTreeState* valueTreeState, juce::Rectangle<int> area);
    ~TutorialStrategyCanvas() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ToggleButton enableSend;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> enableSendAttachment;
};
