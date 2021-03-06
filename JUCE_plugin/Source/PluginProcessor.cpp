/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace std;

//==============================================================================
Festivalle21AudioProcessor::Festivalle21AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
    ,
    treeState(*this, nullptr, juce::Identifier("CURRENT_STATE"),
        {
        std::make_unique<juce::AudioParameterFloat>("rotationAngle", "RotationAngle", 0.0f, 360.0f, 0.0f), // id, name, min,max, initial value
        std::make_unique<juce::AudioParameterFloat>("manualRadius", "ManualRadius", 0.1f, 1.0f, 0.1f),
        std::make_unique<juce::AudioParameterInt>("isManual", "IsManual", 0, 1, 0),
        std::make_unique<juce::AudioParameterBool>("bypassRYB", "ByPassRYB", false),
        std::make_unique<juce::AudioParameterInt>("strategySelection", "StrategySelection", 0, 1, 1),
        std::make_unique<juce::AudioParameterBool>("enableSend", "EnableSend", false),
        })
{
#ifdef MEASURE_TIME
    this->myfile.open("timing_measure.txt");
#endif

    this->sampleRate = 0.0;
    this->samplesPerBlock = 0.0;
    this->rms = 0.0f;


    this->oscIpAddress = "127.0.0.1";
    this->oscPort = 5005;
    this->connectToOsc();

    this->AVStrategy = new ArousalValenceStrategy(&this->treeState);
    this->CMStrategy = new ColourMappingStrategy(&this->treeState);
    this->analisysStrategy = CMStrategy;
}

Festivalle21AudioProcessor::~Festivalle21AudioProcessor()
{
#ifdef MEASURE_TIME
    this->myfile.close();
#endif
    if (this->analisysStrategy->getAddresses()[0] != "/juce/RGB/R") {
        if (this->analisysStrategy->getAddresses()[0] != "/juce/AV/A") {
            delete this->analisysStrategy;
        }
    }
    delete this->AVStrategy;
    delete this->CMStrategy;

}

//==============================================================================
const juce::String Festivalle21AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Festivalle21AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Festivalle21AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Festivalle21AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double Festivalle21AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Festivalle21AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Festivalle21AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Festivalle21AudioProcessor::setCurrentProgram(int index)
{
}

const juce::String Festivalle21AudioProcessor::getProgramName(int index)
{
    return {};
}

void Festivalle21AudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void Festivalle21AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    this->sampleRate = sampleRate;
    this->samplesPerBlock = samplesPerBlock;
    //this->bufferToFill.resize(sampleRate * BUFFER_SIZE); // Initiliase the array to contain 500ms
   // this->bufferToFill.resize(22050); // Initiliase the array to contain 500ms
}

void Festivalle21AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Festivalle21AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void Festivalle21AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

#ifdef MEASURE_TIME
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
#endif

    if (this->connected)
    {
        this->analisysStrategy->processBuffer(buffer, totalNumInputChannels, &sender);
    }

    //DBG(*treeState.getRawParameterValue("strategySelection"));

#ifdef MEASURE_TIME
                auto t2 = high_resolution_clock::now();

                /* Getting number of milliseconds as an integer. */
                auto ms_int = duration_cast<milliseconds>(t2 - t1);

                /* Getting number of milliseconds as a double. */
                duration<double, std::milli> ms_double = t2 - t1;
                this->myfile << to_string(ms_double.count());
                this->myfile << "\n";
#endif


}

//==============================================================================
bool Festivalle21AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Festivalle21AudioProcessor::createEditor()
{
    return new Festivalle21AudioProcessorEditor(*this);
}

//==============================================================================
void Festivalle21AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Festivalle21AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


bool Festivalle21AudioProcessor::setIP(juce::String ipAddress)
{
    this->oscIpAddress = ipAddress;    
    this->connectToOsc();
    return this->connected;
}

bool Festivalle21AudioProcessor::setPort(juce::String port)
{
    this->oscPort = port.getIntValue();
    this->connectToOsc();
    return this->connected;
}

juce::AudioProcessorValueTreeState* Festivalle21AudioProcessor::getValueTreeState()
{
    return &treeState;
}

Strategy* Festivalle21AudioProcessor::getStrategy()
{
    return this->analisysStrategy;
}

void Festivalle21AudioProcessor::changeStrategy(int strat)
{
    /*this control is needed since the analisys strategies implementing a neural network must be allocated in the constructor and 
    can't be dinamically allocated*/
    if (this->analisysStrategy->getAddresses()[0] != "/juce/RGB/R") {
        if (this->analisysStrategy->getAddresses()[0] != "/juce/AV/A") {
            delete this->analisysStrategy;
        }
    }

    switch (strat) {
    case 0:
        this->analisysStrategy = this->AVStrategy;
        break;
    case 1:
        this->analisysStrategy = this->CMStrategy;
        break;
    case 2:
        this->analisysStrategy = new TutorialStrategy(&this->treeState);
    default:
        break;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Festivalle21AudioProcessor();
}

void Festivalle21AudioProcessor::connectToOsc()
{
    // specify here where to send OSC messages to: host URL and UDP port number
    sender.connect(this->oscIpAddress, this->oscPort);   // [4]

    (this->oscPort == 0 || this->oscPort >= 65536) ? this->connected = false : this->connected = true;
}

