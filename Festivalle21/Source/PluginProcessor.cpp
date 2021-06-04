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
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ) 
#endif
{
    this->sampleRate = 0.0;
    this->samplesPerBlock = 0.0;
    this->bufferToFillSampleIdx = 0;
    this->bufferToFill.setSize(1, BUFFER_SIZE);
    this->av = std::vector<std::vector<float>>(COLOR_FREQUENCY, std::vector<float>(2,0));
    this->rms = 0.0f;
    this->currentAVindex = 0;

    // specify here where to send OSC messages to: host URL and UDP port number
    if (!sender.connect("127.0.0.1", 5005))   // [4]
        DBG("Error: could not connect to UDP port 5005."); 


    
}

Festivalle21AudioProcessor::~Festivalle21AudioProcessor()
{
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

void Festivalle21AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Festivalle21AudioProcessor::getProgramName (int index)
{
    return {};
}

void Festivalle21AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Festivalle21AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
bool Festivalle21AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void Festivalle21AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (int sample = 0; sample < buffer.getNumSamples(); sample++) 
    {
        float monoSample = 0.0;

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* inputChannelData = buffer.getReadPointer(channel);
            monoSample += inputChannelData[sample];
        }

        monoSample /= totalNumInputChannels;
        this->bufferToFill.getWritePointer(0)[this->bufferToFillSampleIdx] = monoSample;

        this->bufferToFillSampleIdx++;
        if (this->bufferToFillSampleIdx == BUFFER_SIZE)
        {
            this->bufferToFillSampleIdx = 0;

            this->av.at(currentAVindex) = this->predictAV(this->bufferToFill);
            this->rms = this->bufferToFill.getRMSLevel(0, 0, BUFFER_SIZE);

            sender.send("/juce/RMS", juce::OSCArgument(this->rms));

            
            this->currentAVindex++;
            if (this->currentAVindex == COLOR_FREQUENCY) {
                std::vector<float> msg;
                msg = this->getRGBValue(this->av);
                // create and send an OSC message with an address and a float value:
                if (!sender.send("/juce/RGB", juce::OSCArgument(msg[0]), juce::OSCArgument(msg[1]), juce::OSCArgument(msg[2])))
                    this->av.clear();

                this->currentAVindex = 0;
            }
        }
    }

   
}

//==============================================================================
bool Festivalle21AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Festivalle21AudioProcessor::createEditor()
{
    return new Festivalle21AudioProcessorEditor (*this);
}

//==============================================================================
void Festivalle21AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Festivalle21AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

std::vector<float> Festivalle21AudioProcessor::getAV()
{
    return this->av[0];
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Festivalle21AudioProcessor();
}

std::vector<float> Festivalle21AudioProcessor::predictAV(juce::AudioBuffer<float> buffer)
{
        std::vector<float> my_vector{ buffer.getReadPointer(0), buffer.getReadPointer(0) + BUFFER_SIZE };
        const fdeep::tensor input(fdeep::tensor_shape(22050, 1), my_vector);

        const fdeep::tensors result = model.predict({input});
       
        return result[0].to_vector();
}

std::vector<float> Festivalle21AudioProcessor::getRGBValue(std::vector<std::vector<float>> av)
{
    float avg_valence = 0.0f;
    float avg_arousal = 0.0f;

    float R = 0;
    float G = 0;
    float B = 0;

    for (int i = 0; i < COLOR_FREQUENCY; i++) {
        avg_valence += av[i][0];
        avg_arousal += av[i][1];
    }
    avg_valence /= av.size();
    avg_arousal /= av.size();

    float H = atan2(avg_arousal, avg_valence) * 180.0 / PI;    // Hue
    float S = std::min(sqrt(pow(avg_valence, 2) + pow(avg_arousal, 2)), 1.0);    // Saturation (distance)
    float V = 1.0;  //Intensity

    float C = V * S;
    float X = C * (1.0 - std::abs(std::fmod((H / 60.0), 2.0) - 1.0));
    float m = V - C;

    if (H >= 0 && H < 60) {
        R = C;
        G = X;
        B = 0;
    }
    else if (H >= 60 && H < 120) {
        R = X;
        G = C;
        B = 0;
    }
    else if (H >= 120 && H < 180) {
        R = 0;
        G = C;
        B = X;
    }
    else if (H >= 180 && H < 240) {
        R = 0;
        G = X;
        B = C;
    }
    else if (H >= 240 && H < 300) {
        R = X;
        G = 0;
        B = C;
    }
    else if (H >= 300 && H < 360) {
        R = C;
        G = 0;
        B = X;
    }

    R = (R + m) * 255;
    G = (G + m) * 255;
    B = (B + m) * 255;

    DBG(R);
    DBG(G);
    DBG(B);

    return { R,G,B };
}
