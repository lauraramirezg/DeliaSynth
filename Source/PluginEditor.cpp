/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MySynthAudioProcessorEditor::MySynthAudioProcessorEditor (MySynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //Cambiar fuente a toda la app
    getLookAndFeel().setDefaultSansSerifTypefaceName("Futura");
    
    //Signature Label
    setLabelParams(signatureLabel,"BY LAURA RAMIREZ", 20.0f);
    signatureLabel.setBounds(15, 5, 200, 40);
    
    //---------------------------MASTER LEVEL---------------------------------
    //Label Master Level
    setLabelParams(gainLabel, "LEVEL", 20.0f);
    gainLabel.setBounds(660, 5, 80, 40);
    
    //Slider Master Level
    setSliderBarParams(gainSlider, 720, 10, 100, 30);
    
    //Gain Master Level attachment
    gainSliderAttachment =  std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "GAIN" , gainSlider);
    
    //------------------------------OSC1---------------------------------------
    //Label Titulo OSC1
    setLabelParams(osc1Label, "OSC1", 35.0f);
    osc1Label.setBounds(175,70, 170, 40);
    
    //Label Waveform OSC1
    setLabelParams(waveformLabel, "Waveform", 20.0f);
    waveformLabel.setBounds(55, 80, 100, 100);
    
    //Combobox Waveform OSC1;
    setComboBoxParams(waveformOsc1,juce::StringArray{"Sine", "Square", "Triangle", "Saw"} ,50, 155);
    
    //Waveform attachment
    osc1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, "OSC1", waveformOsc1);
    
    //Label Ganancia OSC1
    setLabelParams(gainOsc1Label, "Level", 20.0f);
    gainOsc1Label.setBounds(197, 110, 100, 40);
    
    //Slider Ganancia OSC1
    setSliderRotatoryParams(osc1Slider, 180, 140, 80, 70);
    
    //Ganancia OSC1 attahcment
    gainOsc1SliderAttachment =  std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "GAIN OSC1" , osc1Slider);
    
    //Label Tuning OSC1
    setLabelParams(tuningOsc1Label, "Tuning", 20.0f);
    tuningOsc1Label.setBounds(300, 117, 100, 25);
    
    //Slider Tuning OSC1
    setSliderBarParams(tuningOsc1Slider, 295, 155, 70, 35);
    
    //Tuning OSC1 attachments
    tuningOsc1SliderAttachment =  std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "TUNING OSC1" , tuningOsc1Slider);
    
    //------------------------------OSC2---------------------------------------
    //Label Titulo OSC2
    setLabelParams(osc2Label, "OSC2", 35.0f);
    osc2Label.setBounds(175,253, 170, 40);
    
    //Label Waveform OSC2
    setLabelParams(waveform2Label, "Waveform", 20.0f);
    waveform2Label.setBounds(55, 260, 100, 100);
    
    //Combobox Waveform OSC2;
    setComboBoxParams(waveformOsc2, juce::StringArray{"Sine", "Square", "Triangle", "Saw"} ,50, 335);
    
    //Waveform attachment
    osc2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, "OSC2", waveformOsc2);
    
    //Label Ganancia OSC2
    setLabelParams(gainOsc2Label, "Level", 20.0f);
    gainOsc2Label.setBounds(197, 290, 100, 40);
    
    //Slider Ganancia OSC2
    setSliderRotatoryParams(osc2Slider, 180, 320, 80, 70);
    
    //Ganancia OSC2 attahcment
    gainOsc2SliderAttachment =  std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "GAIN OSC2" , osc2Slider);
    
    //Label Tuning OSC2
    setLabelParams(tuningOsc2Label, "Tuning", 20.0f);
    tuningOsc2Label.setBounds(300, 297, 100, 25);
    
    //Slider Tuning OSC2
    setSliderBarParams(tuningOsc2Slider, 295, 335, 70, 35);
    
    //Tuning OSC2 attachments
    tuningOsc2SliderAttachment =  std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "TUNING OSC2" , tuningOsc2Slider);
    
    //------------------------------ADSR----------------------------------------
    //Label Titulo ADSR
    setLabelParams(adsrLabel, "ADSR", 35.0f);
    adsrLabel.setBounds(600, 40, 140, 100);
    
    //Label Attack ADSR
    setLabelParams(attackLabel, "A", 20.0f);
    attackLabel.setBounds(470, 117, 70, 30);
    
    //Slider Attack ADSR
    setSliderBarParams(attSlider, 500, 121, 290, 20);
    attSlider.setRange(0.01,1,0.06);
    
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "ATTACK", attSlider);
    
    //Label Decay ADSR
    setLabelParams(decayLabel, "D", 20.0f);
    decayLabel.setBounds(470, 151, 70, 30);
    
    //Slider Decay ADSR
    setSliderBarParams(decSlider, 500, 158, 290, 20);
    decSlider.setRange(0.01,1,0.06);
    
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "DECAY", decSlider);
    
    //Label Sustain ADSR
    setLabelParams(sustainLabel, "S", 20.0f);
    sustainLabel.setBounds(470, 188, 70, 30);
    
    //Slider Sustain ADSR
    setSliderBarParams(susSlider, 500, 193, 290, 20);
    susSlider.setRange(0,1,0.06);
    
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "SUSTAIN", susSlider);
    
    //Label Release ADSR
    setLabelParams(releaseLabel, "R", 20.0f);
    releaseLabel.setBounds(470, 226, 70, 30);
    
    //Slider Release ADSR
    setSliderBarParams(relSlider, 500, 228, 290, 20);
    relSlider.setRange(0,1,0.06);

    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "RELEASE", relSlider);
   
    //------------------------------SINTESIS AM--------------------------------
    //Label Titulo AM
    setLabelParams(amLabel, "TREMOLO", 35.0f);
    amLabel.setBounds(535,490, 200, 40);
    
    //Botton AM
    setButtonParams(amButton, 685, 496);
    amButton.onClick = [this]
    {
        updateButtonAM();
        audioProcessor._buttonStateAM = onClickStateAM;
    };
    
    //Label Waveform AM
    setLabelParams(waveformAmLabel, "Waveform", 20.0f);
    waveformAmLabel.setBounds(487, 500, 100, 100);
    
    //Combobox Waveform AM
    setComboBoxParams(waveformAm,juce::StringArray{"Sine", "Triangle"} ,480, 580);
    
    oscAmAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, "OSC AM", waveformAm);
    
    //Label Gain AM
    setLabelParams(gainAmLabel, "Depth", 20.0f);
    gainAmLabel.setBounds(615, 530, 100, 40);
    
    //Slider Gain AM
    setSliderRotatoryParams(gainAmSlider, 605, 565, 80, 70);
    
    gainAmSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "GAIN AM", gainAmSlider);
    
    //Label Frecuency AM
    setLabelParams(freqAmLabel, "Frequency", 20.0f);
    freqAmLabel.setBounds(713, 500, 100, 100);
    
    //Slider Frequency AM
    setSliderBarParams(freqAmSlider, 718, 580, 70, 35);
    
    freqAmSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "FREQ AM", freqAmSlider);
    
    //------------------------------FILTER--------------------------------
    //Label Titulo FILTER
    setLabelParams(filterLabel, "FILTER", 35.0f);
    filterLabel.setBounds(565,310, 120, 40);
    
    //Botton FILTER
    setButtonParams(filterButton, 665, 317);
    filterButton.onClick = [this]
    {
        updateButtonFilter();
        audioProcessor._buttonStateFilter = onClickStateFilter;
    };
    
    //Label type FILTER
    setLabelParams(typeFilterLabel, "Type", 20.0f);
    typeFilterLabel.setBounds(505, 360, 100, 25);
    
    //Combobox type FILTER
    setComboBoxParams(typeFilter,juce::StringArray{"LP 2", "LP 4", "LP 6", "HP 2", "HP 4", "HP 6"} ,480, 395);
    
    filterTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, "FILTER TYPE", typeFilter);
    
    //Label Cutoff FILTER
    setLabelParams(cutoffLabel, "Cutoff", 20.0f);
    cutoffLabel.setBounds(627, 360, 100, 25);
    
    //Slider Cutoff FILTER
    setSliderBarParams(cutoffSlider, 620, 397, 70, 35);
    cutoffSlider.setRange(50.0f, 16000.0f);
    
    cutoffSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "CUTOFF", cutoffSlider);
    
    //Label Q FILTER
    setLabelParams(QLabel, "Q", 20.0f);
    QLabel.setBounds(740, 360, 100, 25);
    
    //Slider Q FILTER
    setSliderRotatoryParams(QSlider, 715, 380, 80, 70);
    
    QSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "Q", QSlider);
    
    //------------------------------COMPRESOR--------------------------------
    //Label Titulo COMRPESOR
    setLabelParams(compLabel, "COMPRESSOR", 35.0f);
    compLabel.setBounds(115, 430, 200, 50);
    
    //Threshold COMPRESOR
    setLabelParams(threshCompLabel, "Threshold", 20);
    threshCompLabel.setBounds(35, 480, 100, 25);
    setSliderRotatoryParams(threshCompSlider, 35, 510, 80, 70);
    thresholdSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "THRESHOLD", threshCompSlider);
    
    //Ratio COMPRESOR
    setLabelParams(ratCompLabel, "Ratio", 20);
    ratCompLabel.setBounds(140, 480, 100, 25);
    setSliderRotatoryParams(ratCompSlider, 125, 510, 80, 70);
    ratioSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "RATIO", ratCompSlider);
    
    //Attack COMPRESOR
    setLabelParams(attCompLabel, "Attack", 20);
    attCompLabel.setBounds(230, 480, 100, 25);
    setSliderRotatoryParams(attCompSlider, 215, 510, 80, 70);
    attCompSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "COMPATTACK", attCompSlider);
    
    //Release COMPRESOR
    setLabelParams(relCompLabel, "Release", 20);
    relCompLabel.setBounds(310, 480, 100, 25);
    setSliderRotatoryParams(relCompSlider, 300, 510, 80, 70);
    
    relCompSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "COMPRELEASE", relCompSlider);
    
    //MakeUp COMPRESSOR
    setLabelParams(makeUpLabel, "Make Up", 20);
    makeUpLabel.setBounds(35, 595, 100, 25);
    setSliderBarParams(makeUpSlider, 145, 595, 220, 30);
    
    makeUpCompSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "COMPMAKEUP", makeUpSlider);
    
    
    setSize (840, 670);
}

MySynthAudioProcessorEditor::~MySynthAudioProcessorEditor()
{
}

//==============================================================================
void MySynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::lightgrey);
    g.setFont (juce::Font ("Futura", 45.0f, juce::Font::plain));
    g.drawFittedText ("DELIA", getLocalBounds(), juce::Justification::centredTop, 1);
    
    //Cuadro Oscilador 1
    drawBox(g, 20, 395, 163, 268, 230, 90);
    
    //Cuadro Oscilador2
    drawBox(g, 20, 395, 163, 268, 410, 275);
    
    //Cuadro Compressor
    drawBox(g, 20, 395, 105, 320, 650, 455);
    
    //Cuadro ADSR
    drawBox(g, 440, 820, 590, 690, 285, 90);
    
    //Cuadro Filter
    drawBox(g, 440, 820, 555, 740, 465, 330);
    
    //Cuadro Tremolo
    drawBox(g, 440, 820, 525, 763, 650, 510);
    
    
}

void MySynthAudioProcessorEditor::drawBox(juce::Graphics& g,const float x1, const float x2, const float x3, const float x4, const float y1, const float y2)
{
    juce::Line<float> lineAbajo (juce::Point<float> (x1, y1),
                                juce::Point<float> (x2, y1));
    juce::Line<float> lineIzquierda (juce::Point<float> (x1, y1),
                                juce::Point<float> (x1, y2));
    juce::Line<float> lineDerecha (juce::Point<float> (x2, y1),
                                juce::Point<float> (x2, y2));
    juce::Line<float> lineArribaIzq (juce::Point<float> (x1, y2),
                                juce::Point<float> (x3, y2));
    juce::Line<float> lineArribaDer (juce::Point<float> (x2, y2),
                                juce::Point<float> (x4, y2));
    
    g.drawLine (lineAbajo, 1.5f);
    g.drawLine (lineDerecha, 1.5f);
    g.drawLine (lineIzquierda, 1.5f);
    g.drawLine (lineArribaIzq, 1.5f);
    g.drawLine (lineArribaDer, 1.5f);
}

void MySynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void MySynthAudioProcessorEditor::setLabelParams(juce::Label& label, std::string _label, float _size)
{
    addAndMakeVisible(label);
    label.setText(_label, juce::dontSendNotification);
    label.setFont (juce::Font ("Futura", _size , juce::Font::plain));
    label.setColour(juce::Label::textColourId,juce::Colours::lightgrey);
}

void MySynthAudioProcessorEditor::setSliderBarParams(juce::Slider& slider, const int x, const int y, const int ancho, const int alto)
{
    addAndMakeVisible(slider);
    slider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 80, 20);
    slider.setBounds(x,y,ancho,alto);
    slider.setColour(juce::Slider::trackColourId, juce::Colour(209,75,250));
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
}

void MySynthAudioProcessorEditor::setSliderRotatoryParams(juce::Slider& slider, const int x, const int y, const int ancho, const int alto)
{
    addAndMakeVisible(slider);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 80, 20);
    slider.setBounds(x,y,ancho,alto);
    slider.setColour(juce::Slider::rotarySliderFillColourId,
                     juce::Colour(75,220,81));
    slider.setColour(juce::Slider::thumbColourId,
                     juce::Colour(75,220,81));
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setPopupDisplayEnabled(true, true, nullptr);
}

void MySynthAudioProcessorEditor::setComboBoxParams(juce::ComboBox& ComboBox,juce::StringArray choices, const int x, const int y)
{
    addAndMakeVisible(ComboBox);
    ComboBox.addItemList(choices, 1);
    ComboBox.setBounds(x, y, 95, 40);
}

void MySynthAudioProcessorEditor::setButtonParams(juce::TextButton& TextButton, const int x, const int y)
{
    addAndMakeVisible(TextButton);
    TextButton.setBounds(x,y,60,25);
    TextButton.setButtonText("ON/OFF");
    TextButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
}

void MySynthAudioProcessorEditor::updateButtonAM()
{
    contadorAM += 1;
    
    if (contadorAM == 1) {
        onClickStateAM = 1;
        amButton.setColour(juce::TextButton::buttonColourId,juce::Colour(75,220,81));
    }
    else if (contadorAM == 2)
    {
        onClickStateAM  = 0;
        amButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        contadorAM = 0;
        
    }
}

void MySynthAudioProcessorEditor::updateButtonFilter()
{
    contadorFilter += 1;
    
    if (contadorFilter == 1) {
        onClickStateFilter = 1;
        filterButton.setColour(juce::TextButton::buttonColourId,juce::Colour(75,220,81));
    }
    else if (contadorFilter == 2)
    {
        onClickStateFilter  = 0;
        filterButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        contadorFilter = 0;
        
    }
}
