/*
  ==============================================================================

    AVCanvas.cpp
    Created: 10 Nov 2021 12:14:27am
    Author:  wadda

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ColourMappingCanvas.h"

//==============================================================================
ColourMappingCanvas::ColourMappingCanvas(juce::AudioProcessorValueTreeState* valueTreeState, juce::Rectangle<int> area, Strategy* analisysStrategy)
{
    this->area = area;
    this->analisysStrategy = analisysStrategy;

    setSize(area.getWidth(), area.getHeight());
    this->startTimerHz(60);

    addAndMakeVisible(this->colorwheel);

    this->rotationSlider.setAttachment(*valueTreeState, "rotationAngle");
    this->rotationSlider.setSize(300, 20);
    addAndMakeVisible(this->rotationSlider);

    this->rotationLabel.setText("Rotation", juce::dontSendNotification);
    this->rotationLabel.setFont(juce::Font(20.0f));
    this->rotationLabel.setJustificationType(juce::Justification::centred);
    this->rotationLabel.setSize(50, 30);
    this->rotationLabel.attachToComponent(&this->rotationSlider, true);
    addAndMakeVisible(this->rotationLabel);

    this->radiusSlider.setAttachment(*valueTreeState, "manualRadius");
    this->radiusSlider.setSize(300, 20);
    addAndMakeVisible(this->radiusSlider);

    this->radiusLabel.setText("Radius", juce::dontSendNotification);
    this->radiusLabel.setFont(juce::Font(20.0f));
    this->radiusLabel.setJustificationType(juce::Justification::centred);
    this->radiusLabel.setSize(50, 30);
    this->radiusLabel.attachToComponent(&this->radiusSlider, true);
    addAndMakeVisible(this->radiusLabel);

    this->isManualAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(*valueTreeState, "isManual", this->toggleManual));
    this->toggleManual.setSize(30, 30);
    addAndMakeVisible(this->toggleManual);

    this->manualLabel.setText("Manual", juce::dontSendNotification);
    this->manualLabel.setFont(juce::Font(20.0f));
    this->manualLabel.setJustificationType(juce::Justification::centred);
    this->manualLabel.setSize(50, 30);
    this->manualLabel.attachToComponent(&this->toggleManual, true);
    addAndMakeVisible(this->manualLabel);

    // BYPASS RYB BUTTON
    this->bypassRYBAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(*valueTreeState, "bypassRYB", this->bypassRYB));
    this->toggleManual.setSize(30, 30);
    addAndMakeVisible(this->bypassRYB);

    this->bypassRYBLabel.setText("ByPassRYB", juce::dontSendNotification);
    this->bypassRYBLabel.setFont(juce::Font(20.0f));
    this->bypassRYBLabel.setJustificationType(juce::Justification::centred);
    this->bypassRYBLabel.setSize(50, 30);
    this->bypassRYBLabel.attachToComponent(&this->bypassRYB, true);
    addAndMakeVisible(this->bypassRYBLabel);
}

ColourMappingCanvas::~ColourMappingCanvas()
{
}

void ColourMappingCanvas::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

}

void ColourMappingCanvas::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    this->colorwheel.setBounds(this->area.removeFromTop(400));

    auto slidersArea = this->area.removeFromTop(35);
    this->rotationSlider.setBounds(slidersArea.removeFromRight(400));
    area.removeFromTop(10);
    slidersArea = this->area.removeFromTop(40);
    this->radiusSlider.setBounds(slidersArea.removeFromRight(400));
    slidersArea = this->area.removeFromTop(30);
    this->bypassRYB.setBounds(slidersArea.removeFromRight(160));
    this->toggleManual.setBounds(slidersArea.removeFromRight(200));

}

void ColourMappingCanvas::timerCallback()
{
    this->colorwheel.setAV(this->analisysStrategy->getData());
    this->repaint();
}
