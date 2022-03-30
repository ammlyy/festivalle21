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
    TutorialStrategyCanvas();
    ~TutorialStrategyCanvas() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TutorialStrategyCanvas)
};
