/*
  ==============================================================================

    MyKnob.h
    Created: 31 May 2020 4:18:23pm
    Author:  daveg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//==============================================================================
/*
*/
class MyKnob : public juce::Slider
{
public:
    MyKnob() : Slider() {};
    MyKnob(Slider::SliderStyle style, TextEntryBoxPosition textBoxPos) : Slider(style, textBoxPos) {};
    ~MyKnob();

    void setAttachment(juce::AudioProcessorValueTreeState& apvts, const juce::String& id); //attach knob value to audio processor state

private:
    // std::unique_ptr<Label> label; // text label for the knob
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> attachment; // knob value

};
