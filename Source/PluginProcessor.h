/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Filtering.h"
using namespace juce;

class Visualiser : public AudioVisualiserComponent
{
public:
    Visualiser() : AudioVisualiserComponent(2)
    {
        setBufferSize(512);
        setSamplesPerBlock(256);
        setColours(Colours::black, Colours::white);
    }
};

class FreqzeqAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FreqzeqAudioProcessor();
    ~FreqzeqAudioProcessor() override;

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

    Filtering lPfilterL, lPfilterR, hPfilterR, hPfilterL, bPfilterL, bPfilterR, 
              lmffilterL, lmffilterR, mffilterL, mffilterR, hmffilterL, hmffilterR, 
              hffilterL, hffilterR, nFilterL, nFilterR;

    Visualiser waveVisualiser;

    std::atomic<float> gain;

    std::atomic<bool> isWitheNoise;
private:
    Random rand;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreqzeqAudioProcessor)
};
