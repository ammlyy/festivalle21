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
    this->startTimerHz(30);
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
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    float diameter = getWidth()/2.0;
    juce::Point<float> top(getWidth() / 2, (getHeight() - diameter) * 0.5);
    juce::Point<float> bottom(getWidth() / 2, (getHeight() + diameter) * 0.5);
    juce::Point<float> left((getWidth() - diameter) * 0.5, getHeight() / 2.0);
    juce::Point<float> right((getWidth() + diameter) * 0.5, getHeight() / 2.0);

  
    //TOP
    const auto centre = getLocalBounds().toFloat().getCentre();
    const auto pi = juce::MathConstants<float>::pi;

    juce::ColourGradient gradient(juce::Colours::blue, centre,
        juce::Colours::blue, centre.withY(centre.y + 100.f),
        true);
    gradient.addColour(0.25, juce::Colours::purple);
    gradient.addColour(0.5, juce::Colours::red);
    gradient.addColour(0.75, juce::Colours::yellow);
    g.setGradientFill(gradient);

    juce::Path p;
    float thickness = diameter/3.0 + 5;
    p.addCentredArc(centre.x, centre.y, (diameter- thickness) * 0.5f, (diameter - thickness) * 0.5f, 0.f, -pi, pi, true);
    g.strokePath(p, juce::PathStrokeType(thickness));

    g.setColour(juce::Colours::white);
    g.setOpacity(0.3);
    g.drawEllipse((getWidth() - diameter) / 2.0, (getHeight() - diameter) / 2.0, diameter, diameter, 2);
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
    float diameter = getWidth() / 2.0;



    g.drawEllipse(centre.x + av[1] * 1000.f, centre.y - av[0] * 1000.f, 5.f, 5.f, 3.f);


}
