/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FreqzeqAudioProcessor::FreqzeqAudioProcessor() :
    gain(-80),
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

FreqzeqAudioProcessor::~FreqzeqAudioProcessor()
{
}

//==============================================================================
const juce::String FreqzeqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FreqzeqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FreqzeqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FreqzeqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FreqzeqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FreqzeqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FreqzeqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FreqzeqAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FreqzeqAudioProcessor::getProgramName (int index)
{
    return {};
}

void FreqzeqAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FreqzeqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lPfilterL.setSampleRate(sampleRate);
    lPfilterR.setSampleRate(sampleRate);

    hPfilterL.setSampleRate(sampleRate);
    hPfilterR.setSampleRate(sampleRate);

    bPfilterL.setSampleRate(sampleRate);
    bPfilterR.setSampleRate(sampleRate);

    lmffilterL.setSampleRate(sampleRate);
    lmffilterR.setSampleRate(sampleRate);

    mffilterL.setSampleRate(sampleRate);
    mffilterR.setSampleRate(sampleRate);

    hmffilterL.setSampleRate(sampleRate);
    hmffilterR.setSampleRate(sampleRate);

    hffilterL.setSampleRate(sampleRate);
    hffilterR.setSampleRate(sampleRate);

    nFilterL.setSampleRate(sampleRate);
    nFilterR.setSampleRate(sampleRate);
}

void FreqzeqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FreqzeqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FreqzeqAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (isWitheNoise)
    {
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float noiseSample = (rand.nextFloat() * 2) - 1;

            for (auto channel = 0; channel < totalNumInputChannels; ++channel)
            {
                float* channelN = buffer.getWritePointer(channel);

                if (channel == 0)
                {
                    float xn0 = lPfilterL.lpFilter(channelN[sample] + noiseSample * gain);
                    float xn1 = hPfilterL.hpFilter(xn0);
                    float xn2 = bPfilterL.bpFilter(xn1);
                    float xn3 = lmffilterL.bpFilter(xn2);
                    float xn4 = mffilterL.bpFilter(xn3);
                    float xn5 = hmffilterL.bpFilter(xn4);
                    float xn6 = hffilterL.bpFilter(xn5);
                    channelN[sample] = nFilterL.nFilter(xn6);
                }
                else if (channel == 1)
                {
                    float xn0 = lPfilterR.lpFilter(channelN[sample] + noiseSample * gain);
                    float xn1 = hPfilterR.hpFilter(xn0);
                    float xn2 = bPfilterR.bpFilter(xn1);
                    float xn3 = lmffilterR.bpFilter(xn2);
                    float xn4 = mffilterR.bpFilter(xn3);
                    float xn5 = hmffilterR.bpFilter(xn4);
                    float xn6 = hffilterR.bpFilter(xn5);
                    channelN[sample] = nFilterR.nFilter(xn6);
                }
            }
        }
    }
    else 
    {
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float noiseSample = (rand.nextFloat() * 2) - 1;

            for (auto channel = 0; channel < totalNumInputChannels; ++channel)
            {
                float* channelN = buffer.getWritePointer(channel);

                if (channel == 0)
                {
                    float xn0 = lPfilterL.lpFilter(channelN[sample] * gain);
                    float xn1 = hPfilterL.hpFilter(xn0);
                    float xn2 = bPfilterL.bpFilter(xn1);
                    float xn3 = lmffilterL.bpFilter(xn2);
                    float xn4 = mffilterL.bpFilter(xn3);
                    float xn5 = hmffilterL.bpFilter(xn4);
                    float xn6 = hffilterL.bpFilter(xn5);
                    channelN[sample] = nFilterL.nFilter(xn6);
                }
                else if (channel == 1)
                {
                    float xn0 = lPfilterR.lpFilter(channelN[sample] * gain);
                    float xn1 = hPfilterR.hpFilter(xn0);
                    float xn2 = bPfilterR.bpFilter(xn1);
                    float xn3 = lmffilterR.bpFilter(xn2);
                    float xn4 = mffilterR.bpFilter(xn3);
                    float xn5 = hmffilterR.bpFilter(xn4);
                    float xn6 = hffilterR.bpFilter(xn5);
                    channelN[sample] = nFilterR.nFilter(xn6);
                }
            }
        }
    }
    
    waveVisualiser.pushBuffer(buffer);
}

//==============================================================================
bool FreqzeqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FreqzeqAudioProcessor::createEditor()
{
    return new FreqzeqAudioProcessorEditor (*this);
}

//==============================================================================
void FreqzeqAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FreqzeqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FreqzeqAudioProcessor();
}
