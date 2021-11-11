/*
  ==============================================================================

    ArousalValenceCanvas.cpp
    Created: 11 Nov 2021 3:51:11pm
    Author:  wadda

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ArousalValenceCanvas.h"

//==============================================================================
ArousalValenceCanvas::ArousalValenceCanvas(juce::AudioProcessorValueTreeState* valueTreeState, juce::Rectangle<int> area, Strategy* analisysStrategy)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    this->area = area;
    this->analisysStrategy = analisysStrategy;
    setSize(area.getWidth(), area.getHeight());

}

ArousalValenceCanvas::~ArousalValenceCanvas()
{
}

void ArousalValenceCanvas::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ArousalValenceCanvas", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void ArousalValenceCanvas::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
