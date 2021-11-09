/*
  ==============================================================================

    MyKnob.cpp
    Created: 31 May 2020 4:18:23pm
    Author:  daveg

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MyKnob.h"

//==============================================================================



MyKnob::~MyKnob()
{
    attachment.reset(nullptr);
    //label.reset(nullptr);
     
}


void MyKnob::setAttachment(juce::AudioProcessorValueTreeState& apvts, const juce::String& id)
{
    // knobValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, id, this);
    attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, id, *this));
}

