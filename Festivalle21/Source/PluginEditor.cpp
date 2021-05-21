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

    /*
    g.drawLine(juce::Line<float>(0.0, 0.0, getWidth(), getHeight()));
    g.drawLine(juce::Line<float>(getWidth() / 2.0 - diameter *0.5 * cos(PI), getHeight()*0.5 + diameter *0.5 * sin(PI),getWidth()/2.0 + diameter/2.0 * cos(PI), getHeight()/2.0 - diameter/2.0 * sin(PI) ));
    */

    //TOP
    const auto centre = getLocalBounds().toFloat().getCentre();
    const auto pi = juce::MathConstants<float>::pi;

    juce::ColourGradient gradient(juce::Colours::blue, centre,
        juce::Colours::blue, centre.withY(centre.y + 100.f),
        juce::ColourGradient::Conical);
    gradient.addColour(0.25, juce::Colours::purple);
    gradient.addColour(0.5, juce::Colours::red);
    gradient.addColour(0.75, juce::Colours::yellow);
    g.setGradientFill(gradient);

    juce::Path p;
    float thickness = diameter/3.0 + 5;
    p.addCentredArc(centre.x, centre.y, (diameter- thickness) * 0.5f, (diameter - thickness) * 0.5f, 0.f, -pi, pi, true);
    g.strokePath(p, juce::PathStrokeType(thickness));

    /*
    //BOTTOM-RIGHT
    juce::ColourGradient cg2(juce::Colours::blue, right, juce::Colours::yellow, bottom, false);
    g.setGradientFill(cg2);
    juce::Path path2;

    path2.startNewSubPath(right);
    path2.addCentredArc(getWidth() / 2, getHeight() / 2, diameter / 2.0, diameter / 2.0, PI/2, 0, PI/2);
    path2.lineTo(getWidth() / 2.0, getHeight() / 2.0);
    path2.lineTo(right);
    path2.closeSubPath();
    g.fillPath(path2);

    //BOTTOM-LEFT
    juce::ColourGradient cg3(juce::Colours::yellow, bottom, juce::Colours::green, left, false);
    g.setGradientFill(cg3);
    juce::Path path3;

    path3.startNewSubPath(bottom);
    path3.addCentredArc(getWidth() / 2, getHeight() / 2, diameter / 2.0, diameter / 2.0, PI, 0, PI / 2);
    path3.lineTo(getWidth() / 2.0, getHeight() / 2.0);
    path3.lineTo(bottom);
    path3.closeSubPath();
    g.fillPath(path3);

    */


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
