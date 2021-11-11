/*
  ==============================================================================

    ColourMappingCanvas.h
    Created: 10 Nov 2021 12:14:27am
    Author:  wadda

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ColorWheel.h"
#include "MyKnob.h"
#include "Strategy.h"

//==============================================================================
/*
*/
class ColourMappingCanvas  : public juce::Component, public juce::Timer
{
public:
    ColourMappingCanvas(juce::AudioProcessorValueTreeState* valueTreeState, juce::Rectangle<int> area, Strategy* analisysStrategy);
    ~ColourMappingCanvas() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    ColorWheel colorwheel{ 400,300 };
    juce::Label radiusLabel;
    juce::Label rotationLabel;
    juce::Label manualLabel;
    juce::Label bypassRYBLabel;

    MyKnob rotationSlider;
    MyKnob radiusSlider;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> isManualAttachment;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> bypassRYBAttachment;
    juce::ToggleButton toggleManual;
    juce::ToggleButton bypassRYB;
    
    juce::Rectangle<int> area;

    Strategy* analisysStrategy;
};
