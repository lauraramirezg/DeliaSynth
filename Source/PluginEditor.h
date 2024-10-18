/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MySynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MySynthAudioProcessorEditor (MySynthAudioProcessor&);
    ~MySynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void setLabelParams(juce::Label& label, std::string _label, float _size);
    void setSliderBarParams(juce::Slider& slider, const int x, const int y, const int ancho, const int alto);
    void setSliderRotatoryParams(juce::Slider& slider, const int x, const int y, const int ancho, const int alto);
    void setComboBoxParams(juce::ComboBox& ComboBox,juce::StringArray choices, const int x, const int y);
    void setButtonParams(juce::TextButton& TextButton, const int x, const int y);
    void updateButtonAM();
    void updateButtonFilter();
    void drawBox (juce::Graphics& g,const float x1, const float x2, const float x3, const float x4, const float y1, const float y2);
    void resized() override;
    
    //BOTONES
    int onClickStateAM;
    int onClickStateFilter;
    int contadorAM;
    int contadorFilter;
    
private:
    
    //SIGNATURE
    juce::Label signatureLabel;
    
    //MASTER LEVEL
    juce::Slider gainSlider;
    juce::Label gainLabel;
    
    //OSC1
    juce::Label osc1Label;
    juce::Label waveformLabel;
    juce::Label gainOsc1Label;
    juce::Slider osc1Slider;
    juce::ComboBox waveformOsc1;
    
    //OSC2
    juce::Label osc2Label;
    juce::Label waveform2Label;
    juce::Label gainOsc2Label;
    juce::Slider osc2Slider;
    juce::ComboBox waveformOsc2;
    
    //ADSR
    juce::Label adsrLabel;
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;
    juce::Slider attSlider;
    juce::Slider decSlider;
    juce::Slider susSlider;
    juce::Slider relSlider;
    
    //AM SINTESIS
    juce::Label amLabel;
    juce::Label waveformAmLabel;
    juce::Label freqAmLabel;
    juce::Label gainAmLabel;
    juce::Label depthAmLabel;
    juce::ComboBox waveformAm;
    juce::Slider freqAmSlider;
    juce::Slider gainAmSlider;
    juce::TextButton amButton;
    
    //FILTER
    juce::Label filterLabel;
    juce::Label typeFilterLabel;
    juce::Label cutoffLabel;
    juce::Label QLabel;
    juce::ComboBox typeFilter;
    juce::Slider cutoffSlider;
    juce::Slider QSlider;
    juce::TextButton filterButton;
    
    //OCTAVADOR
    juce::Label tuningOsc1Label;
    juce::Slider tuningOsc1Slider;
    juce::Label tuningOsc2Label;
    juce::Slider tuningOsc2Slider;
    
    //COMPRESOR
    juce::Label compLabel;
    juce::Label attCompLabel;
    juce::Label relCompLabel;
    juce::Label ratCompLabel;
    juce::Label threshCompLabel;
    juce::Slider attCompSlider;
    juce::Slider relCompSlider;
    juce::Slider ratCompSlider;
    juce::Slider threshCompSlider;
    juce::Label makeUpLabel;
    juce::Slider makeUpSlider;

    
    
    
    //ATTACHMENTS
    
    //Gain Master Attachment: crea un atachment del Slider a parametros del audioProcessorValueTree
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;

    //OSC1 Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainOsc1SliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tuningOsc1SliderAttachment;
    
    //OSC2 Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainOsc2SliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tuningOsc2SliderAttachment;
    
    //Sintesis AM Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscAmAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAmSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqAmSliderAttachment;
    
    //ADSR attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
    //Filter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> QSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAttachment;
    
    //Compressor attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attCompSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> relCompSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> makeUpCompSliderAttachment;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MySynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MySynthAudioProcessorEditor)
};
