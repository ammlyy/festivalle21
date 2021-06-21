/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
Festivalle21AudioProcessorEditor::Festivalle21AudioProcessorEditor (Festivalle21AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    this->startTimerHz(60);
    addAndMakeVisible(this->colorwheel);


    this->port.setText("5005", juce::dontSendNotification);
    this->ip.setText("127.0.0.1", juce::dontSendNotification);
    this->port.setEditable(true);
    this->ip.setEditable(true);
    this->port.setFont(20.0f);
    this->ip.setFont(20.0f);

    this->ip.setBounds(10, 50, 75, 25);
    this->port.setBounds(10, 10, 75, 25);

    addAndMakeVisible(this->port);
    addAndMakeVisible(this->ip);
}

Festivalle21AudioProcessorEditor::~Festivalle21AudioProcessorEditor()
{
}

void Festivalle21AudioProcessorEditor::timerCallback()
{
    this->colorwheel.setAV(this->audioProcessor.getAV());
    this->repaint();
}
//==============================================================================
void Festivalle21AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
}

void Festivalle21AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

