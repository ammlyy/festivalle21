/*
  ==============================================================================

    ColorWheel.cpp
    Created: 17 Jun 2021 10:26:14am
    Author:  amere

  ==============================================================================
*/

#include "ColorWheel.h"

ColorWheel::ColorWheel(int width, int height)
{
    setSize(width, height);
    this->av = std::vector<float>(2);
    this->wheelImage = juce::ImageCache::getFromMemory(BinaryData::COLOR_WHEEL_png, BinaryData::COLOR_WHEEL_pngSize);
    this->wheelImage = this->wheelImage.rescaled(getWidth() / 2.0, getWidth() / 2.0, juce::Graphics::highResamplingQuality);

}

ColorWheel::~ColorWheel()
{
}

void ColorWheel::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    float diameter = getWidth() / 2.0;
    juce::Point<float> top(getWidth() / 2, (getHeight() - diameter) * 0.5);
    juce::Point<float> bottom(getWidth() / 2, (getHeight() + diameter) * 0.5);
    juce::Point<float> left((getWidth() - diameter) * 0.5, getHeight() / 2.0);
    juce::Point<float> right((getWidth() + diameter) * 0.5, getHeight() / 2.0);

    g.drawImageAt(this->wheelImage, (getWidth() - diameter) / 2.0, (getHeight() - diameter) / 2.0);

    g.setColour(juce::Colours::white);
    g.setOpacity(1);
    g.drawEllipse((getWidth() - diameter) / 2.0, (getHeight() - diameter) / 2.0, diameter, diameter, 4);
    g.drawVerticalLine(getWidth() / 2, top.getY(), bottom.getY());
    g.drawHorizontalLine(getHeight() / 2.0, left.getX(), right.getX());
    this->drawPoint(g, this->av);

}

void ColorWheel::resized()
{
    this->wheelImage = this->wheelImage.rescaled(getWidth() / 2.0, getWidth() / 2.0, juce::Graphics::highResamplingQuality);
    this->repaint();
}

void ColorWheel::drawPoint(juce::Graphics& g, std::vector<float> av)
{
    const auto centre = getLocalBounds().toFloat().getCentre();
    float radius = getWidth() / 4.0;
    g.setColour(juce::Colours::darkgrey);
    g.drawEllipse(centre.x + av[1] * radius, centre.y - av[0] * radius, 5.f, 5.f, 5.f);

}

void ColorWheel::setAV(std::vector<float> av)
{
    this->av = av;
}
