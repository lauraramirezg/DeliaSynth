/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MySynthAudioProcessor::MySynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Params", creatParams())
#endif
{
    //Añade en el constructor las voces y los sonidos que se deseen
    for (int i = 0; i < 8; ++i)
    {
        synth.addVoice(new SynthVoice());
    }
    synth.addSound(new SynthSound());
}

MySynthAudioProcessor::~MySynthAudioProcessor()
{
}

//==============================================================================
const juce::String MySynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MySynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MySynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MySynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MySynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MySynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MySynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MySynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MySynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void MySynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MySynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    synth.setCurrentPlaybackSampleRate(sampleRate); //le pasa la frecuencia de muestreo al objeto synth
    
    //Se llama al metodo prepare to play que se creo, como no hace parte de la clase SynthesizerVoice, se crea un puntero que verifica que cada voz creada es del tipo SynthVoice(de la clase que se creo) y ahi si puede llamar al prepareToPlay para cada voz.
    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice* >(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void MySynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MySynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void MySynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //lee cada una de las voces del objeto synth creadas y verifica que cada una corresponda al tipo de clase que creamos que hereda del SynthesiserVoice.
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            //Osciladores, ADSR, etc, se actualizan los parametros aqui de cada voz
            //Gain Master Level
            auto& master = *apvts.getRawParameterValue("GAIN");
            voice->updateGAIN(master.load());
            
            //OSC1
            auto& osc1Choice = *apvts.getRawParameterValue("OSC1");
            //getOscillator devuelve el objeto osc y setWaveType es el metodo para seleccionar el tipo de onda
            voice->getOscillator().setWaveType(osc1Choice.load());
            
            auto& osc1Gain = *apvts.getRawParameterValue("GAIN OSC1");
            voice->updateGainOsc1(osc1Gain.load());
            
            auto& _detuningOsc1 = *apvts.getRawParameterValue("TUNING OSC1");
            voice->detuningOsc1 = _detuningOsc1;
            
            //OSC2
            auto& osc2Choice = *apvts.getRawParameterValue("OSC2");
            voice->getOscillator2().setWaveType(osc2Choice.load());
            
            auto& osc2Gain = *apvts.getRawParameterValue("GAIN OSC2");
            voice->updateGainOsc2(osc2Gain.load());
            
            auto& _detuningOsc2 = *apvts.getRawParameterValue("TUNING OSC2");
            voice->detuningOsc2 = _detuningOsc2;
            
            //ADSR
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
            
            //AM frequency and gain
            auto& freqAM = *apvts.getRawParameterValue("FREQ AM");
            auto& oscChoiceAM = *apvts.getRawParameterValue("OSC AM");
            auto& gainAM = *apvts.getRawParameterValue("GAIN AM");
            
            voice->getOscillatorAM().setWaveType(oscChoiceAM.load());
            voice->updateAM(gainAM, freqAM);
            voice->buttonStateAM = _buttonStateAM;
            
            //Filter
            auto& filterType = *apvts.getRawParameterValue("FILTER TYPE");
            auto& cutoff = *apvts.getRawParameterValue("CUTOFF");
            auto& Q = *apvts.getRawParameterValue("Q");
            
            voice->updateFilter(cutoff, Q, (int) filterType);
            voice->buttonStateFilter = _buttonStateFilter;
            
            //Compressor
            auto& threshold = *apvts.getRawParameterValue("THRESHOLD");
            auto& ratio = *apvts.getRawParameterValue("RATIO");
            auto& Tattk = *apvts.getRawParameterValue("COMPATTACK");
            auto& Trels = *apvts.getRawParameterValue("COMPRELEASE");
            auto& makeUp = *apvts.getRawParameterValue("COMPMAKEUP");
            
            voice->updateComp(threshold, ratio, Tattk, Trels, makeUp);
            
        }
    }
    
    //Llama al metodo render que procesa el oscilador y lo envia al buffer de salida para poder escucharlo
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
   
}

//==============================================================================
bool MySynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MySynthAudioProcessor::createEditor()
{
    return new MySynthAudioProcessorEditor (*this);
}

//==============================================================================
void MySynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MySynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MySynthAudioProcessor();
}

//Se llama a la funcion de crear lista parametros
juce::AudioProcessorValueTreeState::ParameterLayout MySynthAudioProcessor::creatParams()
{
    //vector llamado params del tipo RangeAudio para almacenar los diferentes parametros
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //agrega un parametro
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "GAIN", 1 }, "Gain Master", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),0.5f));
    
    //Waveform OSC1
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "OSC1", 1 }, "Oscillator", juce::StringArray{"Sine", "Square", "Triangle", "Saw"}, 0));
                     
    //Ganancia OSC1
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "GAIN OSC1", 1 }, "Gain", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),0.5f));
    
    //Tuning OSC1
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "TUNING OSC1", 1 }, "Tuning", juce::NormalisableRange<float>(-24, 24, 1),0));
    
    //Waveform OSC2
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "OSC2", 1 }, "Oscillator 2", juce::StringArray{"Sine", "Square", "Triangle", "Saw"}, 0));
                     
    //Ganancia OSC2
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "GAIN OSC2", 1 }, "Gain 2", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),0.5f));
    
    //Tuning OSC2
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "TUNING OSC2", 1 }, "Tuning", juce::NormalisableRange<float>(-24, 24, 1),0));
    
    //Waveform AM
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "OSC AM", 1 }, "Oscillator AM", juce::StringArray{"Sine", "Square", "Triangle", "Saw"}, 0));
    
    //Frequency AM
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "FREQ AM", 1 }, "Frequency AM", juce::NormalisableRange<float>(1.0f, 7.0f, 0.1f), 6.0f));
    
    //Ganancia AM
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "GAIN AM", 1 }, "Gain AM", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),0.3f));
    
    //ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "ATTACK", 1 }, "Attack", juce::NormalisableRange<float>(0.1f, 1.0f, 0.01f), 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "DECAY", 1 }, "Decay", juce::NormalisableRange<float>(0.1f, 1.0f, 0.01f), 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "SUSTAIN", 1 }, "Sustain", juce::NormalisableRange<float>(0.1f, 1.0f, 0.01f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "RELEASE", 1 }, "Release", juce::NormalisableRange<float>(0.1f, 3.0f, 0.01f), 0.4f));
    
    //Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "FILTER TYPE", 1 }, "Filter Type", juce::StringArray{"LP 2", "LP 4", "LP 6", "HP 2", "HP 4", "HP 6"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "CUTOFF", 1 }, "Cutoff", juce::NormalisableRange<float>(50.0f, 16000.0f, 0.1f), 100.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "Q", 1 }, "Q", juce::NormalisableRange<float>(0.2f, 1.0f, 0.01f), 0.7f));
    
    //Compressor
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"THRESHOLD",1}, "Threshold",
        juce::NormalisableRange<float>{-50.0f, 0.0f, 0.1f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RATIO",1}, "Ratio",
        juce::NormalisableRange<float>{1.0f, 30.0f, 0.1f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"COMPATTACK",1}, "Comp Attack",
        juce::NormalisableRange<float>{0.0f, 200.0f, 0.1f}, 20.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"COMPRELEASE",1}, "Comp Release",
        juce::NormalisableRange<float>{5.0f, 5000.0f, 0.1f}, 10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"COMPMAKEUP",1}, "Comp Release",
        juce::NormalisableRange<float>{-20.0f, 50.0f, 0.1f}, 0.0f));

    
    //devuelve un ParameterLayour
    return {params.begin(), params.end()};
}

