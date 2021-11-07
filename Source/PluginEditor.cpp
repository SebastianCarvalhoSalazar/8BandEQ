/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
using namespace juce;

//==============================================================================
FreqzeqAudioProcessorEditor::FreqzeqAudioProcessorEditor(FreqzeqAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{ 
    ///////////////////////////// FREQZ-EQ ////////////////////////////////////////
    auto FreqzImage = ImageCache::getFromMemory(BinaryData::Freqz_PNG, BinaryData::Freqz_PNGSize);
    myImageComponent.setImage(FreqzImage, RectanglePlacement::stretchToFit);

    if (!FreqzImage.isNull())
        myImageComponent.setImage(FreqzImage, RectanglePlacement::stretchToFit);
    else
        jassert(!FreqzImage.isNull());

    gainSlider.reset(new Slider());
    addAndMakeVisible(gainSlider.get());

    lpFrequencySlider.reset(new Slider());
    addAndMakeVisible(lpFrequencySlider.get());

    hpFrequencySlider.reset(new Slider());
    addAndMakeVisible(hpFrequencySlider.get());

    bpFrequencySlider.reset(new Slider());
    addAndMakeVisible(bpFrequencySlider.get());

    lpQSlider.reset(new Slider());
    addAndMakeVisible(lpQSlider.get());

    hpQSlider.reset(new Slider());
    addAndMakeVisible(hpQSlider.get());

    bpQ1Slider.reset(new Slider());
    addAndMakeVisible(bpQ1Slider.get());

    lmfFrequencySlider.reset(new Slider());
    addAndMakeVisible(lmfFrequencySlider.get());

    lmfQSilder.reset(new Slider());
    addAndMakeVisible(lmfQSilder.get());

    mfFrequencySlider.reset(new Slider());
    addAndMakeVisible(mfFrequencySlider.get());

    mfQSilder.reset(new Slider());
    addAndMakeVisible(mfQSilder.get());

    hmfFrequencySlider.reset(new Slider());
    addAndMakeVisible(hmfFrequencySlider.get());

    hmfQSilder.reset(new Slider());
    addAndMakeVisible(hmfQSilder.get());

    hfFrequencySlider.reset(new Slider());
    addAndMakeVisible(hfFrequencySlider.get());
  
    hfQSilder.reset(new Slider());
    addAndMakeVisible(hfQSilder.get());

    nFrequencySlider.reset(new Slider());
    addAndMakeVisible(nFrequencySlider.get());

    nQSilder.reset(new Slider());
    addAndMakeVisible(nQSilder.get());
    
    addAndMakeVisible(myImageComponent);
    addAndMakeVisible(audioProcessor.waveVisualiser);

    addAndMakeVisible(witheNoise);

    addAndMakeVisible(lowPassTB);
    addAndMakeVisible(highPassTB);
    addAndMakeVisible(bandPassTB);
    addAndMakeVisible(lmfTB);
    addAndMakeVisible(mfTB);
    addAndMakeVisible(hmfTB);
    addAndMakeVisible(hfTB);
    addAndMakeVisible(nTB);

    addAndMakeVisible(textLabel1);
    addAndMakeVisible(textLabel2);
    addAndMakeVisible(textLabel3);
    addAndMakeVisible(textLabel4);
    addAndMakeVisible(textLabel5);
    addAndMakeVisible(textLabel6);
    addAndMakeVisible(textLabel7);
    addAndMakeVisible(textLabel8);

    ///////////////////////////// WITHE NOISE AND GAIN ////////////////////////////////////////

    // Withe Noise
    witheNoise.setToggleState(false, dontSendNotification);
    witheNoise.onClick = [this]
    {
        audioProcessor.isWitheNoise = witheNoise.getToggleState();
    };

    witheNoiseLabel.setText("Withe Noise", dontSendNotification);
    witheNoiseLabel.attachToComponent(&witheNoise, true);

    // Gain Slider
    gainSlider->setRange(-80, 0, 0.01);
    gainSlider->setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::green));
    gainSlider->setColour(Slider::ColourIds::thumbColourId, Colour(Colours::darkorange));
    gainSlider->setTextValueSuffix("dB");
    gainSlider->setValue(-80);
    gainSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
    //gainLabel.setText("Gain", dontSendNotification);
    //gainLabel.attachToComponent(gainSlider.get(), true);

    ///////////////////////////// LOW PASS FILTER ////////////////////////////////////////
    
    // Titulo LPF
    textLabel1.setText("LPF", dontSendNotification);
    textLabel1.setColour(Label::textColourId, Colours::red);
    textLabel1.setJustificationType(Justification::centred);

    // IsLowPass
    lowPassTB.setToggleState(false, dontSendNotification);
    lowPassTB.onClick = [this]
    {
      audioProcessor.lPfilterL.isLowPass = lowPassTB.getToggleState();
      audioProcessor.lPfilterR.isLowPass = lowPassTB.getToggleState();
    };

    lpLabelBT.setText("On/Off", dontSendNotification);
    lpLabelBT.attachToComponent(&lowPassTB, true);

    // Slider (Lowpass)
    lpFrequencySlider -> setRange(20,8000, 0.1);
    lpFrequencySlider -> setSliderStyle(Slider::SliderStyle::Rotary);
    lpFrequencySlider -> setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::red));
    lpFrequencySlider -> setValue(20);
    lpFrequencySlider -> setTextValueSuffix("Hz");
    lpFrequencySlider -> setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
    //lpFrequencyLabel.setText("LPF", dontSendNotification);
    //lpFrequencyLabel.attachToComponent(lpFrequencySlider.get(), true);

    lpFrequencySlider->onValueChange = [this]
    {
        float lP = lpFrequencySlider->getValue();
        audioProcessor.lPfilterL.setCutOffFrequencylowPass(lP);
        audioProcessor.lPfilterR.setCutOffFrequencylowPass(lP);
    };

    // Q LP Slider
    lpQSlider->setRange(0.1, 10, 0.1);
    lpQSlider->setSliderStyle(Slider::SliderStyle::Rotary);
    lpQSlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::blue));
    lpQSlider->setValue(0.7);
    lpQSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    lpQSlider -> onValueChange = [this]
    {
        float lPQ = lpQSlider -> getValue();
        audioProcessor.lPfilterL.setQLowPass(lPQ);
        audioProcessor.lPfilterR.setQLowPass(lPQ);
    };

    ///////////////////////////// HIGH PASS FILTER ////////////////////////////////////////

    // Titulo LHF
    textLabel2.setText("HPF", dontSendNotification);
    textLabel2.setColour(Label::textColourId, Colours::blue);
    textLabel2.setJustificationType(Justification::centred);

    // Frequency Slider (LHF)
    hpFrequencySlider -> setRange(120, 20000, 0.1);
    hpFrequencySlider -> setSliderStyle(Slider::SliderStyle::Rotary);
    hpFrequencySlider -> setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::red));
    hpFrequencySlider -> setValue(120);
    hpFrequencySlider -> setTextValueSuffix("Hz");
    hpFrequencySlider -> setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
    //hpFrequencyLabel.setText("HPF", dontSendNotification);
    //hpFrequencyLabel.attachToComponent(hpFrequencySlider.get(), true);

    hpFrequencySlider -> onValueChange = [this]
    {
        float hP = hpFrequencySlider -> getValue();
        audioProcessor.hPfilterL.setCutOffFrequencyHighPass(hP);
        audioProcessor.hPfilterR.setCutOffFrequencyHighPass(hP);
    };

    // Q HPF Slider
    hpQSlider->setRange(0.1, 10, 0.1);
    hpQSlider->setSliderStyle(Slider::SliderStyle::Rotary);
    hpQSlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::blue));
    hpQSlider->setValue(0.7);
    hpQSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    hpQSlider -> onValueChange = [this]
    {
        float hPQ = hpQSlider -> getValue();
        audioProcessor.hPfilterL.setQHighPass(hPQ);
        audioProcessor.hPfilterR.setQHighPass(hPQ);
    };


    // isHighPass
    highPassTB.setToggleState(false, dontSendNotification);
    highPassTB.onClick = [this]
    {
        audioProcessor.hPfilterL.isHighPass = highPassTB.getToggleState();
        audioProcessor.hPfilterR.isHighPass = highPassTB.getToggleState();
    };

    hpLabelBT.setText("On/Off", dontSendNotification);
    hpLabelBT.attachToComponent(&highPassTB, true);   

    gainSlider -> onValueChange = [this]
    {
        float decJ = Decibels::decibelsToGain(gainSlider -> getValue());
        audioProcessor.gain = decJ;
    };
 
    ///////////////////////////// LF ////////////////////////////////////////
    // Titulo BPF 1
    textLabel3.setText("LF", dontSendNotification);
    textLabel3.setColour(Label::textColourId, Colours::orange);
    textLabel3.setJustificationType(Justification::centred);

    // Frequency Slider (BPF 1)
    bpFrequencySlider -> setRange(20, 500, 0.1);
    bpFrequencySlider -> setSliderStyle(Slider::SliderStyle::Rotary);
    bpFrequencySlider -> setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::red));
    bpFrequencySlider -> setValue(20);
    bpFrequencySlider -> setTextValueSuffix("Hz");
    bpFrequencySlider -> setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
    //bpFrequencyLabel.setText("BPF", dontSendNotification);
    //bpFrequencyLabel.attachToComponent(bpFrequencySlider.get(), true);

    bpFrequencySlider -> onValueChange = [this]
    {
        float bP = bpFrequencySlider -> getValue();
        audioProcessor.bPfilterL.setCutOffFrequencyBandPass(bP);
        audioProcessor.bPfilterR.setCutOffFrequencyBandPass(bP);
    };

    // isBandPass
    bandPassTB.setToggleState(false, dontSendNotification);
    bandPassTB.onClick = [this]
    {
        audioProcessor.bPfilterL.isBandPass = bandPassTB.getToggleState();
        audioProcessor.bPfilterR.isBandPass = bandPassTB.getToggleState();
    };

    bpLabelBT.setText("On/Off", dontSendNotification);
    bpLabelBT.attachToComponent(&bandPassTB, true);

    // Q Slider (BPF 1)
    bpQ1Slider -> setRange(0.1, 10, 0.1);
    bpQ1Slider -> setSliderStyle(Slider::SliderStyle::Rotary);
    bpQ1Slider -> setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::blue));
    bpQ1Slider -> setValue(0.7);
    bpQ1Slider -> setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
    //bpQ1Label.setText("Q", dontSendNotification);
    //bpQ1Label.attachToComponent(bpQ1Slider.get(), true);

    bpQ1Slider -> onValueChange = [this]
    {
        float Q1 = bpQ1Slider -> getValue();
        audioProcessor.bPfilterL.setQ1BandPass(Q1);
        audioProcessor.bPfilterR.setQ1BandPass(Q1);
    };

    ///////////////////////////// LMF ////////////////////////////////////////

    // Titulo BPF 2
    textLabel4.setText("LMF", dontSendNotification);
    textLabel4.setColour(Label::textColourId, Colours::purple);
    textLabel4.setJustificationType(Justification::centred);

    // Frequency Slider (BPF 2)
    lmfFrequencySlider->setRange(40, 1000, 0.1);
    lmfFrequencySlider->setSliderStyle(Slider::SliderStyle::Rotary);
    lmfFrequencySlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::red));
    lmfFrequencySlider->setValue(20);
    lmfFrequencySlider->setTextValueSuffix("Hz");
    lmfFrequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    lmfFrequencySlider->onValueChange = [this]
    {
        float lmfbP = lmfFrequencySlider->getValue();
        audioProcessor.lmffilterL.setCutOffFrequencyBandPass(lmfbP);
        audioProcessor.lmffilterR.setCutOffFrequencyBandPass(lmfbP);
    };

    // isBandPass
    lmfTB.setToggleState(false, dontSendNotification);
    lmfTB.onClick = [this]
    {
        audioProcessor.lmffilterL.isBandPass = lmfTB.getToggleState();
        audioProcessor.lmffilterR.isBandPass = lmfTB.getToggleState();
    };

    lmfLabelBT.setText("On/Off", dontSendNotification);
    lmfLabelBT.attachToComponent(&lmfTB, true);

    // Q Slider (BPF 2)
    lmfQSilder -> setRange(0.1, 10, 0.1);
    lmfQSilder-> setSliderStyle(Slider::SliderStyle::Rotary);
    lmfQSilder-> setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::blue));
    lmfQSilder-> setValue(0.7);
    lmfQSilder-> setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    lmfQSilder -> onValueChange = [this]
    {
        float lmfQ = lmfQSilder -> getValue();
        audioProcessor.lmffilterL.setQ1BandPass(lmfQ);
        audioProcessor.lmffilterR.setQ1BandPass(lmfQ);
    };

    ///////////////////////////// MF ////////////////////////////////////////

    // Titulo BPF 3
    textLabel5.setText("MF", dontSendNotification);
    textLabel5.setColour(Label::textColourId, Colours::blueviolet);
    textLabel5.setJustificationType(Justification::centred);

    // Frequency Slider (BPF 3)
    mfFrequencySlider->setRange(125, 8000, 0.1);
    mfFrequencySlider->setSliderStyle(Slider::SliderStyle::Rotary);
    mfFrequencySlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::red));
    mfFrequencySlider->setValue(125);
    mfFrequencySlider->setTextValueSuffix("Hz");
    mfFrequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    mfFrequencySlider -> onValueChange = [this]
    {
        float mfbP = mfFrequencySlider->getValue();
        audioProcessor.mffilterL.setCutOffFrequencyBandPass(mfbP);
        audioProcessor.mffilterR.setCutOffFrequencyBandPass(mfbP);
    };

    // isBandPass
    mfTB.setToggleState(false, dontSendNotification);
    mfTB.onClick = [this]
    {
        audioProcessor.mffilterL.isBandPass = mfTB.getToggleState();
        audioProcessor.mffilterR.isBandPass = mfTB.getToggleState();
    };

    mfLabelBT.setText("On/Off", dontSendNotification);
    mfLabelBT.attachToComponent(&mfTB, true);

    // Q Slider (BPF 3)
    mfQSilder->setRange(0.1, 10, 0.1);
    mfQSilder->setSliderStyle(Slider::SliderStyle::Rotary);
    mfQSilder->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::blue));
    mfQSilder->setValue(0.7);
    mfQSilder->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);;

    mfQSilder->onValueChange = [this]
    {
        float mfQ = mfQSilder->getValue();
        audioProcessor.mffilterL.setQ1BandPass(mfQ);
        audioProcessor.mffilterR.setQ1BandPass(mfQ);
    };

    ///////////////////////////// HMF ////////////////////////////////////////

    // Titulo BPF 4
    textLabel6.setText("HMF", dontSendNotification);
    textLabel6.setColour(Label::textColourId, Colours::darkred);
    textLabel6.setJustificationType(Justification::centred);

    // Frequency Slider (Bandpass 4)
    hmfFrequencySlider->setRange(200, 18000, 0.1);
    hmfFrequencySlider->setSliderStyle(Slider::SliderStyle::Rotary);
    hmfFrequencySlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::red));
    hmfFrequencySlider->setValue(200);
    hmfFrequencySlider->setTextValueSuffix("Hz");
    hmfFrequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    hmfFrequencySlider->onValueChange = [this]
    {
        float hmfbP = hmfFrequencySlider->getValue();
        audioProcessor.hmffilterL.setCutOffFrequencyBandPass(hmfbP);
        audioProcessor.hmffilterR.setCutOffFrequencyBandPass(hmfbP);
    };

    // isBandPass
    hmfTB.setToggleState(false, dontSendNotification);
    hmfTB.onClick = [this]
    {
        audioProcessor.hmffilterL.isBandPass = hmfTB.getToggleState();
        audioProcessor.hmffilterR.isBandPass = hmfTB.getToggleState();
    };

    hmfLabelBT.setText("On/Off", dontSendNotification);
    hmfLabelBT.attachToComponent(&hmfTB, true);

    // Q Slider (BPF 4)
    hmfQSilder->setRange(0.1, 10, 0.1);
    hmfQSilder->setSliderStyle(Slider::SliderStyle::Rotary);
    hmfQSilder->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::blue));
    hmfQSilder->setValue(0.7);
    hmfQSilder->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    hmfQSilder->onValueChange = [this]
    {
        float hmfQ = hmfQSilder->getValue();
        audioProcessor.hmffilterL.setQ1BandPass(hmfQ);
        audioProcessor.hmffilterR.setQ1BandPass(hmfQ);
    };

    ///////////////////////////// HF ////////////////////////////////////////

    // Titulo BPF 5
    textLabel7.setText("HF", dontSendNotification);
    textLabel7.setColour(Label::textColourId, Colours::yellow);
    textLabel7.setJustificationType(Justification::centred);

    // Frequency Slider (Bandpass 5)
    hfFrequencySlider->setRange(1800, 20000, 0.1);
    hfFrequencySlider->setSliderStyle(Slider::SliderStyle::Rotary);
    hfFrequencySlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::red));
    hfFrequencySlider->setValue(200);
    hfFrequencySlider->setTextValueSuffix("Hz");
    hfFrequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    hfFrequencySlider->onValueChange = [this]
    {
        float hfbP = hfFrequencySlider->getValue();
        audioProcessor.hffilterL.setCutOffFrequencyBandPass(hfbP);
        audioProcessor.hffilterR.setCutOffFrequencyBandPass(hfbP);
    };

    // isBandPass
    hfTB.setToggleState(false, dontSendNotification);
    hfTB.onClick = [this]
    {
        audioProcessor.hffilterL.isBandPass = hfTB.getToggleState();
        audioProcessor.hffilterR.isBandPass = hfTB.getToggleState();
    };

    hfLabelBT.setText("On/Off", dontSendNotification);
    hfLabelBT.attachToComponent(&hfTB, true);

    // Q Slider (BPF 5)
    hfQSilder->setRange(0.1, 10, 0.1);
    hfQSilder->setSliderStyle(Slider::SliderStyle::Rotary);
    hfQSilder->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::blue));
    hfQSilder->setValue(0.7);
    hfQSilder->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    hfQSilder->onValueChange = [this]
    {
        float hfQ = hfQSilder->getValue();
        audioProcessor.hffilterL.setQ1BandPass(hfQ);
        audioProcessor.hffilterR.setQ1BandPass(hfQ);
    };

    //////////////////////////////// Notch Filter /////////////////////////////////////////////

    // Titulo Notch
    textLabel8.setText("Notch", dontSendNotification);
    textLabel8.setColour(Label::textColourId, Colours::aquamarine);
    textLabel8.setJustificationType(Justification::centred);

    // Frequency Slider (Notch)
    nFrequencySlider->setRange(20, 20000, 0.1);
    nFrequencySlider->setSliderStyle(Slider::SliderStyle::Rotary);
    nFrequencySlider->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::red));
    nFrequencySlider->setValue(1000);
    nFrequencySlider->setTextValueSuffix("Hz");
    nFrequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    nFrequencySlider->onValueChange = [this]
    {
        float N = nFrequencySlider->getValue();
        audioProcessor.nFilterL.setCutOffFrequencyNotch(N);
        audioProcessor.nFilterR.setCutOffFrequencyNotch(N);
    };

    // isNotchFilter
    nTB.setToggleState(false, dontSendNotification);
    nTB.onClick = [this]
    {
        audioProcessor.nFilterL.isNotchFilter = nTB.getToggleState();
        audioProcessor.nFilterR.isNotchFilter = nTB.getToggleState();
    };

    nLabelBT.setText("On/Off", dontSendNotification);
    nLabelBT.attachToComponent(&nTB, true);

    // Q Slider (Notch)
    nQSilder->setRange(0.1, 10, 0.1);
    nQSilder->setSliderStyle(Slider::SliderStyle::Rotary);
    nQSilder->setColour(Slider::ColourIds::rotarySliderFillColourId, Colour(Colours::blue));
    nQSilder->setValue(0.7);
    nQSilder->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);

    nQSilder->onValueChange = [this]
    {
        float nQ = nQSilder->getValue();
        audioProcessor.nFilterL.setQNotch(nQ);
        audioProcessor.nFilterR.setQNotch(nQ);
    };

    //////////////////////////////////////////////////////////////////////////////////////

    witheNoise.setBounds(930, 30, 25, 25);
    lowPassTB.setBounds(70, 242, 25, 25);
    highPassTB.setBounds(190, 242, 25, 25);
    bandPassTB.setBounds(310, 242, 25, 25);
    lmfTB.setBounds(430, 242, 25, 25);
    mfTB.setBounds(550, 242, 25, 25);
    hmfTB.setBounds(670, 242, 25, 25);
    hfTB.setBounds(790, 242, 25, 25);
    nTB.setBounds(910, 240, 25, 25);

    textLabel1.setBounds(25, 225, 100, 10);
    textLabel2.setBounds(145, 225, 100, 10);
    textLabel3.setBounds(265, 225, 100, 10);
    textLabel4.setBounds(385, 225, 100, 10);
    textLabel5.setBounds(505, 225, 100, 10);
    textLabel6.setBounds(625, 225, 100, 10);
    textLabel7.setBounds(745, 225, 100, 10);
    textLabel8.setBounds(870, 225, 100, 10);

    myImageComponent.setBounds(860, 420, 120, 30);
    audioProcessor.waveVisualiser.setBounds(20, 20, 780, 195);

    gainSlider->setBounds(840, 50, 120, 150);
    lpFrequencySlider -> setBounds(35, 270, 80, 80);
    hpFrequencySlider -> setBounds(155, 270, 80, 80);
    bpFrequencySlider -> setBounds(275, 270, 80, 80);
    lmfFrequencySlider -> setBounds(395, 270, 80, 80);
    mfFrequencySlider->setBounds(515, 270, 80, 80);
    hmfFrequencySlider->setBounds(635, 270, 80, 80);
    hfFrequencySlider->setBounds(755, 270, 80, 80);
    nFrequencySlider->setBounds(885, 256, 80, 80);

    lpQSlider  -> setBounds(35, 350, 80, 80);
    hpQSlider  -> setBounds(155, 350, 80, 80);
    bpQ1Slider -> setBounds(275, 350, 80, 80);
    lmfQSilder -> setBounds(395, 350, 80, 80);
    mfQSilder->setBounds(515, 350, 80, 80);
    hmfQSilder->setBounds(635, 350, 80, 80);
    hfQSilder->setBounds(755, 350, 80, 80);
    nQSilder->setBounds(885, 333, 80, 80);
    
    setSize(1000, 470);
}

FreqzeqAudioProcessorEditor::~FreqzeqAudioProcessorEditor()
{
    gainSlider = nullptr;

    lpFrequencySlider = nullptr;
    lpQSlider = nullptr;

    hpFrequencySlider = nullptr;
    hpQSlider = nullptr;

    bpFrequencySlider = nullptr;
    bpQ1Slider = nullptr;
    
    lmfFrequencySlider = nullptr;
    lmfQSilder = nullptr;

    mfFrequencySlider = nullptr;
    mfQSilder = nullptr;

    hmfFrequencySlider = nullptr;
    hmfQSilder = nullptr;

    hfFrequencySlider = nullptr;
    hfQSilder = nullptr;   

    nFrequencySlider = nullptr;
    nQSilder = nullptr;
}

//==============================================================================
void FreqzeqAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colours::black);
    g.setColour(Colour(Colours::darkgrey));
    g.drawRect(10, 10, 980, 450);

    g.setColour(Colour(Colours::green));
    g.drawRect(810, 15, 170, 205);

    g.setColour(Colour(Colours::red));
    g.drawRect(20, 240, 110, 210);

    g.setColour(Colour(Colours::blue));
    g.drawRect(140, 240, 110, 210);

    g.setColour(Colour(Colours::orange));
    g.drawRect(260, 240, 110, 210);

    g.setColour(Colour(Colours::purple));
    g.drawRect(380, 240, 110, 210);

    g.setColour(Colour(Colours::blueviolet));
    g.drawRect(500, 240, 110, 210);

    g.setColour(Colour(Colours::darkred));
    g.drawRect(620, 240, 110, 210);

    g.setColour(Colour(Colours::yellow));
    g.drawRect(740, 240, 110, 210);

    g.setColour(Colour(Colours::aquamarine));
    g.drawRect(860, 240, 120, 178);

    g.setColour(Colour(Colours::cornflowerblue));
    g.drawRect(15, 15, 790, 205);
}

void FreqzeqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
