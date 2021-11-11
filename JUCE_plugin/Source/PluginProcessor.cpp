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
        std::make_unique<juce::AudioParameterInt>("strategySelection", "strategySelection", 0, 1, 0),
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
    this->analisysStrategy = AVStrategy;
}

Festivalle21AudioProcessor::~Festivalle21AudioProcessor()
{
#ifdef MEASURE_TIME
    this->myfile.close();
#endif
    delete this->AVStrategy;
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Festivalle21AudioProcessor();
}


// Calculate RGB values corresponding to the point defined by valence and arousal and returns them as std::vector (size 3)
std::vector<float> Festivalle21AudioProcessor::calculateRGB(float valence, float arousal)
{
    float R = 0.0f;
    float Y = 0.0f;
    float B = 0.0f;

    float H = (atan2(valence, arousal) * 180.0 / PI) + 30;    // Hue
    if (H < 0) {
        H = 360.0 + H;
    }
    DBG("H: " + to_string(H));
    float S = min(max(sqrt(pow(valence, 2) + pow(arousal, 2)), 0.6), 1.0);    // Saturation (distance)
    float V = 1;  //Intensity

    float C = V * S;
    float X = C * (1.0 - std::abs(std::fmod((H / 60.0), 2.0) - 1.0));
    float m = V - C;

    if (H >= 0 && H < 60) {
        R = C;
        Y = X;
        B = 0;
    }
    else if (H >= 60 && H < 120) {
        R = X;
        Y = C;
        B = 0;
    }
    else if (H >= 120 && H < 180) {
        R = 0;
        Y = C;
        B = X;
    }
    else if (H >= 180 && H < 240) {
        R = 0;
        Y = X;
        B = C;
    }
    else if (H >= 240 && H < 300) {
        R = X;
        Y = 0;
        B = C;
    }
    else if (H >= 300 && H <= 360) {
        R = C;
        Y = 0;
        B = X;
    }

    R = (R + m);
    Y = (Y + m);
    B = (B + m);

    std::vector<float> rgbValues = ryb2RGB(R, Y, B);
    *treeState.getRawParameterValue("bypassRYB") ? rgbValues = { R, Y, B } : rgbValues = ryb2RGB(R, Y, B);
    /*for (auto& el : rgbValues) {
        el *= 255.0f;
    }*/

    DBG("R: " + to_string(rgbValues[0]));
    DBG("G: " + to_string(rgbValues[1]));
    DBG("B: " + to_string(rgbValues[2]));

    return rgbValues;
}

void Festivalle21AudioProcessor::connectToOsc()
{
    // specify here where to send OSC messages to: host URL and UDP port number
    sender.connect(this->oscIpAddress, this->oscPort);   // [4]

    (this->oscPort == 0 || this->oscPort >= 65536) ? this->connected = false : this->connected = true;
}

// Convert RYB values to RGB
std::vector<float> Festivalle21AudioProcessor::ryb2RGB(float r, float y, float b)
{
    std::vector<float> rgb(3);

    float x0 = this->cubicInterp(b, RYB_COLORS[0][0], RYB_COLORS[4][0]);
    float x1 = this->cubicInterp(b, RYB_COLORS[1][0], RYB_COLORS[5][0]);
    float x2 = this->cubicInterp(b, RYB_COLORS[2][0], RYB_COLORS[6][0]);
    float x3 = this->cubicInterp(b, RYB_COLORS[3][0], RYB_COLORS[7][0]);
    float y0 = this->cubicInterp(y, x0, x1);
    float y1 = this->cubicInterp(y, x2, x3);

    rgb[0] = this->cubicInterp(r, y0, y1);

    x0 = this->cubicInterp(b, RYB_COLORS[0][1], RYB_COLORS[4][1]);
    x1 = this->cubicInterp(b, RYB_COLORS[1][1], RYB_COLORS[5][1]);
    x2 = this->cubicInterp(b, RYB_COLORS[2][1], RYB_COLORS[6][1]);
    x3 = this->cubicInterp(b, RYB_COLORS[3][1], RYB_COLORS[7][1]);
    y0 = this->cubicInterp(y, x0, x1);
    y1 = this->cubicInterp(y, x2, x3);

    rgb[1] = this->cubicInterp(r, y0, y1);

    x0 = this->cubicInterp(b, RYB_COLORS[0][2], RYB_COLORS[4][2]);
    x1 = this->cubicInterp(b, RYB_COLORS[1][2], RYB_COLORS[5][2]);
    x2 = this->cubicInterp(b, RYB_COLORS[2][2], RYB_COLORS[6][2]);
    x3 = this->cubicInterp(b, RYB_COLORS[3][2], RYB_COLORS[7][2]);
    y0 = this->cubicInterp(y, x0, x1);
    y1 = this->cubicInterp(y, x2, x3);

    rgb[2] = this->cubicInterp(r, y0, y1);

    return rgb;
}

float Festivalle21AudioProcessor::cubicInterp(float t, float A, float B)
{
    float w = t * t * (3.0f - 2.0f * t);

    return A + w * (B - A);
}