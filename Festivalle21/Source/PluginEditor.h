/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Festivalle21AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Festivalle21AudioProcessorEditor (Festivalle21AudioProcessor&);
    ~Festivalle21AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Festivalle21AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Festivalle21AudioProcessorEditor)
};
