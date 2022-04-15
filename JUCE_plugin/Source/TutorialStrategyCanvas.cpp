/*
  ==============================================================================

    TutorialStrategyCanvas.cpp
    Created: 30 Mar 2022 10:34:42pm
    Author:  wadda

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TutorialStrategyCanvas.h"

//==============================================================================
TutorialStrategyCanvas::TutorialStrategyCanvas(juce::AudioProcessorValueTreeState* valueTreeState, juce::Rectangle<int> area)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(area.getWidth(), area.getHeight());
    this->enableSendAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(*valueTreeState, "enableSend", this->enableSend));
    this->enableSend.setSize(30, 30);
    addAndMakeVisible(this->enableSend);
}

TutorialStrategyCanvas::~TutorialStrategyCanvas()
{
}

void TutorialStrategyCanvas::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}

void TutorialStrategyCanvas::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
