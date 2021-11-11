/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fdeep/fdeep.hpp>
#include <string>
#include "ArousalValenceStrategy.h"

#define PI 3.14159265
#define BUFFER_SIZE 22050 //500 ms at 44.1kHz

#define COLOR_FREQUENCY 4 // number of 500ms windows analyzed before sending a color change (2seconds)
#define SCENE_SELECTOR 20 // number of 500ms windows analyzed before sending a Scene triger (10seconds)
//#define MEASURE_TIME 0  // comment this line not to measure prediction time
#define SCALING_FACTOR 3

//==============================================================================
/**
*/
class Festivalle21AudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    Festivalle21AudioProcessor();
    ~Festivalle21AudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    bool setIP(juce::String ipAddress);
    bool setPort(juce::String port);

    juce::AudioProcessorValueTreeState* getValueTreeState();
    Strategy* getStrategy();

private:

#ifdef MEASURE_TIME
    std::ofstream myfile;
#endif
    ArousalValenceStrategy* AVStrategy;
    Strategy* analisysStrategy;
    juce::AudioProcessorValueTreeState treeState;
    double sampleRate;
    double samplesPerBlock;
    float rms;
    juce::OSCSender sender;
    bool connected;         // True if the OSC port is valid (!= 0 || < 65536)
    juce::String oscIpAddress;
    int oscPort;


    float RYB_COLORS[8][3] = { 
        {1.0, 1.0, 1.0},
        { 1.0, 1.0 , 0.0 },
        { 1.0, 0.0, 0.0 },
        { 1.0, .5, 0.0 },
        {0.163, 0.373, 0.6},
        { 0.0, 0.66, 0.2 },
        { .5, 0.0, 0.5 },
        { 0.2, 0.094, 0.0 }
    };

    //==============================================================================
    std::vector<float> calculateRGB(float valence, float arousal);
    void connectToOsc();
    std::vector<float> ryb2RGB(float r, float y, float b);
    float cubicInterp(float t, float A, float B);

};
