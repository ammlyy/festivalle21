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
#include "ArousalValenceCanvas.h"

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

    juce::Label portLabel;
    juce::Label portInput;
    juce::Label ipLabel;
    juce::Label ipInput;

    Component* canvas;
    ArousalValenceCanvas* AVCanvas;
    ColourMappingCanvas* CMCanvas;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> strategySelectionAttachment;

    juce::ComboBox strategySelector;
    void strategySelectionChanged();


    juce::AudioProcessorValueTreeState* valueTreeState;

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Festivalle21AudioProcessorEditor)
};