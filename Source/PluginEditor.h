/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
using namespace juce;

//==============================================================================
/**
*/
class FreqzeqAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FreqzeqAudioProcessorEditor (FreqzeqAudioProcessor&);
    ~FreqzeqAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    std::unique_ptr<Slider> gainSlider, lpFrequencySlider, lpQSlider, hpFrequencySlider,hpQSlider, 
                            bpFrequencySlider, bpQ1Slider, lmfFrequencySlider, lmfQSilder,
                            mfFrequencySlider, mfQSilder, hmfFrequencySlider, hmfQSilder,
                            hfFrequencySlider, hfQSilder, nFrequencySlider, nQSilder;

    ToggleButton witheNoise, lowPassTB, highPassTB, bandPassTB, lmfTB, mfTB, hmfTB, hfTB, nTB;

    ImageComponent myImageComponent;

    Label gainLabel, lpFrequencyLabel, hpFrequencyLabel, bpFrequencyLabel, bpQ1Label, witheNoiseLabel, lpLabelBT,
          hpLabelBT, bpLabelBT, lmfLabelBT, textLabel1, textLabel2, textLabel3, textLabel4, textLabel5,
          mfLabelBT, textLabel6, hmfLabelBT, textLabel7, hfLabelBT, textLabel8, nLabelBT;

    FreqzeqAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreqzeqAudioProcessorEditor)
};
