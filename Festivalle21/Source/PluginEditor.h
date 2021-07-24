/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ColorWheel.h"
#include "MyKnob.h"

//==============================================================================
/**
*/
class Festivalle21AudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    Festivalle21AudioProcessorEditor (Festivalle21AudioProcessor&);
    ~Festivalle21AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Festivalle21AudioProcessor& audioProcessor;
    ColorWheel colorwheel{ 400,300 };
    juce::Label portLabel;
    juce::Label portInput;
    juce::Label ipLabel;
    juce::Label ipInput;

    MyKnob rotationSlider;

    juce::AudioProcessorValueTreeState* valueTreeState;

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Festivalle21AudioProcessorEditor)
};