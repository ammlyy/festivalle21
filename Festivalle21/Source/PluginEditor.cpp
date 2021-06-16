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
    this->colorWheel = juce::ImageCache::getFromMemory(BinaryData::COLOR_WHEEL_png, BinaryData::COLOR_WHEEL_pngSize);
    this->colorWheel = this->colorWheel.rescaled(getWidth() / 2.0 , getWidth() /2.0, juce::Graphics::highResamplingQuality);

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
    this->repaint();
}
//==============================================================================
void Festivalle21AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    float diameter = getWidth()/2.0;
    juce::Point<float> top(getWidth() / 2, (getHeight() - diameter) * 0.5);
    juce::Point<float> bottom(getWidth() / 2, (getHeight() + diameter) * 0.5);
    juce::Point<float> left((getWidth() - diameter) * 0.5, getHeight() / 2.0);
    juce::Point<float> right((getWidth() + diameter) * 0.5, getHeight() / 2.0);

    g.drawImageAt(this->colorWheel, (getWidth() - diameter) / 2.0, (getHeight() - diameter) / 2.0);
    

    g.setColour(juce::Colours::white);
    g.setOpacity(0.3);
    g.drawEllipse((getWidth() - diameter) / 2.0, (getHeight() - diameter) / 2.0, diameter, diameter, 3);
    g.drawVerticalLine(getWidth() / 2, top.getY(), bottom.getY());
    g.drawHorizontalLine(getHeight() / 2.0, left.getX(), right.getX());


    this->drawPoint(g);
    

    
}

void Festivalle21AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void Festivalle21AudioProcessorEditor::drawPoint(juce::Graphics& g)
{
    const auto av = this->audioProcessor.getAV();
    const auto centre = getLocalBounds().toFloat().getCentre();
    float radius = getWidth() / 4.0;

    g.drawEllipse(centre.x + av[1] * radius, centre.y - av[0] * radius, 5.f, 5.f, 3.f);


}
