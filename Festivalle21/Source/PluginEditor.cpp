/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
Festivalle21AudioProcessorEditor::Festivalle21AudioProcessorEditor (Festivalle21AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(p.getValueTreeState())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 500);
    this->startTimerHz(60);
    addAndMakeVisible(this->colorwheel);

    this->ipLabel.setText("IP Address: ", juce::dontSendNotification);
    this->ipLabel.setFont(juce::Font(20.0f));
    this->ipLabel.setJustificationType(juce::Justification::centred);
    this->ipLabel.setSize(100, 30);
    addAndMakeVisible(this->ipLabel);

    this->ipInput.setText("127.0.0.1", juce::dontSendNotification);
    this->ipInput.setFont(juce::Font(20.0f));
    this->ipInput.setEditable(true, false, true);
    this->ipInput.setJustificationType(juce::Justification::centred);
    this->ipInput.setSize(150, 30);
    this->ipInput.setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);
    this->ipInput.onTextChange = [this] {
        if(!this->audioProcessor.setIP(this->ipInput.getText()))
            this->ipInput.setText("Invalid IP address", juce::dontSendNotification);
    };
    addAndMakeVisible(this->ipInput);

    this->portLabel.setText("Port: ", juce::dontSendNotification);
    this->portLabel.setFont(juce::Font(20.0f));
    this->portLabel.setJustificationType(juce::Justification::centred);
    this->portLabel.setSize(50, 30);
    addAndMakeVisible(this->portLabel);

    this->portInput.setText("5005", juce::dontSendNotification);
    this->portInput.setFont(juce::Font(20.0f));
    this->portInput.setEditable(true);
    this->portInput.setJustificationType(juce::Justification::centred);
    this->portInput.setSize(150, 30);
    this->portInput.setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);
    this->portInput.onTextChange = [this] {
        if(!this->audioProcessor.setPort(this->portInput.getText()))
            this->portInput.setText("Invalid port", juce::dontSendNotification);
    };
    addAndMakeVisible(this->portInput);

    setResizable(false, false);

    this->rotationSlider.setAttachment(*valueTreeState, "rotationAngle");
    this->rotationSlider.setSize(300, 20);
    this->rotationSlider.addListener(&this->colorwheel);
    addAndMakeVisible(this->rotationSlider);

}

Festivalle21AudioProcessorEditor::~Festivalle21AudioProcessorEditor()
{
}

void Festivalle21AudioProcessorEditor::timerCallback()
{
    this->colorwheel.setAV(this->audioProcessor.getAverageAV());
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

    int margin = 10;
    auto area = getLocalBounds().reduced(margin);

    this->colorwheel.setBounds(area.removeFromTop(400));

    auto labelsArea = area.removeFromTop(30);
    labelsArea.removeFromLeft(30);
    this->ipLabel.setBounds(labelsArea.removeFromLeft(100));
    this->ipInput.setBounds(labelsArea.removeFromLeft(150));
    labelsArea.removeFromLeft(50);
    this->portLabel.setBounds(labelsArea.removeFromLeft(50));
    this->portInput.setBounds(labelsArea.removeFromLeft(150));
}

