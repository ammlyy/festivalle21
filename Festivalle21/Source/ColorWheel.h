#pragma once
#include <JuceHeader.h>

class ColorWheel : public juce::Component {
public:

    ColorWheel(int width, int height);
    ~ColorWheel();

    void paint(juce::Graphics&) override;
    void resized() override;

    void drawPoint(juce::Graphics& g, std::vector<float> av);
    void setAV(std::vector<float> av);


private:
    juce::Image wheelImage;
    std::vector<float> av;

};
