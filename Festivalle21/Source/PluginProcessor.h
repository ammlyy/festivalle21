/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fdeep/fdeep.hpp>
#include <string>

#define PI 3.14159265

#define COLOR_FREQUENCY 6 // number of 500ms windows analyzed before sending a color change (3seconds)
#define SCENE_SELECTOR 20 // number of 500ms windows analyzed before sending a Scene triger (10seconds)

//#define PATH_TO_JSON = "D:\\GitHub\\festivalle21\\Festivalle21\\Source\\models\\exported\\pred_model.json";

//==============================================================================
/**
*/
class Festivalle21AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Festivalle21AudioProcessor();
    ~Festivalle21AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    std::vector<float> getAV();

private:

    double sampleRate;
    double samplesPerBlock;
    std::vector<std::vector<float>> av;
    int currentAVindex;
    fdeep::model model = fdeep::load_model("D:\\GitHub\\festivalle21\\Festivalle21\\Source\\models\\exported\\td_modelNone.json");
    std::vector<float> bufferToFill;
    int bufferToFillSampleIdx;
    juce::OSCSender sender;
    std::vector<float> getRGBValue(std::vector<std::vector<float>>);


    //==============================================================================
    void predictAV(std::vector<float> buffer);
};
