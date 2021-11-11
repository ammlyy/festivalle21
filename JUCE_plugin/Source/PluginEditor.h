/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyKnob.h"
#include "ColourMappingCanvas.h"

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
    //ColorWheel colorwheel{ 400,300 };
    juce::Label portLabel;
    juce::Label portInput;
    juce::Label ipLabel;
    juce::Label ipInput;

    Component* canvas;

    /*juce::Label radiusLabel;
    juce::Label rotationLabel;
    juce::Label manualLabel;
    juce::Label bypassRYBLabel;

    MyKnob rotationSlider;
    MyKnob radiusSlider;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> isManualAttachment;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> bypassRYBAttachment;*/
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> strategySelectionAttachment;
    /*juce::ToggleButton toggleManual;
    juce::ToggleButton bypassRYB;*/

    juce::ComboBox strategySelector;


    juce::AudioProcessorValueTreeState* valueTreeState;

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Festivalle21AudioProcessorEditor)
};