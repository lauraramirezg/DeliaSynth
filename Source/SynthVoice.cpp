/*
  ==============================================================================

    SynthVoice.cpp
    Created: 18 Nov 2023 6:26:37pm
    Author:  user

  ==============================================================================
*/

//Aca se llaman a cada uno de los metodos de la SynthVoice

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    newFrequencyOsc1 = frequency * std::pow(2, detuningOsc1/12);
    newFrequencyOsc2 = frequency * std::pow(2, detuningOsc2/12);
    
    //varia le frecuencia del objeto osc segun la nota midi=
    if (detuningOsc1 == 0)
    {
        osc.setFrequency(frequency);
    }
    else
    {
        osc.setFrequency(newFrequencyOsc1);
    }
    
    if (detuningOsc2 == 0)
    {
        osc2.setFrequency(frequency);
    }
    else
    {
        osc2.setFrequency(newFrequencyOsc2);
    }
    
    adsr.noteOn(); //empieza etapa ataque 
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff(); //empieza etapa release
    
    if (! allowTailOff && ! adsr.isActive())
    {
        clearCurrentNote();
    }
}
//bool isVoiceActive () const;

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{

}
void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);
    
    //Para pasarle informacion al oscilador, que viene del modulo de dsp
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock; //buffersize
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    //Se le pasan los parametros anteriores a cada oscilador y gain creados
    osc.prepare(spec);
    osc2.prepare(spec);
    oscAM.prepare(spec);
    gain.prepare(spec);
    gainOsc1.prepare(spec);
    gainOsc2.prepare(spec);
    gainAM.prepare(spec);
    
    //Se le pasa la frecuencia de muestreo a los objetos filtros creados
    myfilterL.setSampleRate(sampleRate);
    myfilterR.setSampleRate(sampleRate);
    
    //AM Y FILTER
    buttonStateAM = 0;
    buttonStateFilter = 0;
    
    //Compressor
    myCompressor.setSampleRate(sampleRate);
}

//Metodo para llamarlo en el process y que actualice los valores de la adsr
void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    
    adsr.setParameters(adsrParams);
}

//Metodo para actualizar el valor del gain master
void SynthVoice::updateGAIN (const float _gain)
{
    gain.setGainLinear(_gain);
}

//Metodo para actualziar el gain del OSC1
void SynthVoice::updateGainOsc1(const float _gain)
{
    gainOsc1.setGainLinear(_gain);
}

//Metodo para actualizar el gain del OSC2
void SynthVoice::updateGainOsc2(const float _gain)
{
    gainOsc2.setGainLinear(_gain);
}

//Metodo para actualizar los valores de Sintesis AM
void SynthVoice::updateAM(const float _gain, const float _freq)
{
    gainAM.setGainLinear(_gain);
    oscAM.setFrequency(_freq);
}

//Metodo para actualizar los valores del filtro
void SynthVoice::updateFilter(const float _cutoff, const float _Q, const int _filterType)
{
    myfilterL.setFc(_cutoff);
    myfilterL.setQ(_Q);
    myfilterL.setFilterType(_filterType);
    myfilterR.setFc(_cutoff);
    myfilterR.setQ(_Q);
    myfilterR.setFilterType(_filterType);
}

void SynthVoice::updateComp (float _thresh, float _ratio, float _Tattk, float _Trels, float _makeUp)
{
    myCompressor.updateParamsComp(_thresh, _ratio, _Tattk, _Trels, _makeUp);
}

//Aca se hace el procecamiento del oscilador
void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    //Sino se toca la nota se sale de la funcion y no realiza lo que este por debajo de esta condicion
    if (! isVoiceActive())
        return;
    
    //Buffer del synth
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();
    
    //Buffer del OSC1
    osc1Buffer.setSize(outputBuffer.getNumChannels(), numSamples);
    osc1Buffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlockOsc1 {osc1Buffer};  //AudioBlock hace una copia del buffer de salida
    
    osc.process(juce::dsp::ProcessContextReplacing<float> (audioBlockOsc1));
    gainOsc1.process(juce::dsp::ProcessContextReplacing<float> (audioBlockOsc1));
    
    //Buffer del OSC2
    osc2Buffer.setSize(outputBuffer.getNumChannels(), numSamples);
    osc2Buffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlockOsc2 {osc2Buffer};
    
    osc2.process(juce::dsp::ProcessContextReplacing<float> (audioBlockOsc2));
    gainOsc2.process(juce::dsp::ProcessContextReplacing<float> (audioBlockOsc2));
    
    juce::dsp::AudioBlock<float> audioBlock {synthBuffer};
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        synthBuffer.addFrom(channel,0, osc1Buffer, channel,0, numSamples);
    }
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        synthBuffer.addFrom(channel,0, osc2Buffer, channel,0, numSamples);
    }
    
    //Aplica el master level y la envolvente a todo el synthbuffer
    gain.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    //Buffer de sintesis AM
    amBuffer.setSize(outputBuffer.getNumChannels(), numSamples,false,false,true);
    amBuffer.clear();
    juce::dsp::AudioBlock<float> audioBlockAM {amBuffer};
    
    oscAM.process(juce::dsp::ProcessContextReplacing<float> (audioBlockAM));
    gainAM.process(juce::dsp::ProcessContextReplacing<float> (audioBlockAM));
    
    
    //Agrega el buffer que se creo al buffer de salida en cada canal
    for (int sample = 0; sample < numSamples; ++sample)
    {
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            auto sampleOut = synthBuffer.getSample(channel, sample);
            auto sampleOutAM = sampleOut * ((amBuffer.getSample(channel, sample) + 1)/2);
            
            //auto pruebita = myCompressor.processSample(sampleOut);
            //sampleOutAM = myCompressor.processSample(sampleOutAM);
            
            //Canal L, AM on, FILTER off
            if (channel == 0 && buttonStateAM == 1 && buttonStateFilter == 0)
            {
                auto outSample = myCompressor.processSample(sampleOutAM);
                outputBuffer.addSample(channel, startSample + sample, outSample);
            }
            
            //Canal R, AM on, FILTER off
            else if (channel == 1 && buttonStateAM == 1 && buttonStateFilter == 0)
            {
                auto outSample = myCompressor.processSample(sampleOutAM);
                outputBuffer.addSample(channel, startSample + sample, outSample);
            }
            
            //Canal L, AM on, FILTER on
            else if (channel == 0 && buttonStateAM == 1 && buttonStateFilter == 1)
            {
                auto outSample = myCompressor.processSample(myfilterL.processSample(sampleOutAM));
                outputBuffer.addSample(channel, startSample + sample, outSample);
            }
            
            //Canal R, AM on, FILTER on
            else if (channel == 1 && buttonStateAM == 1 && buttonStateFilter == 1)
            {
                auto outSample = myCompressor.processSample(myfilterR.processSample(sampleOutAM));
                outputBuffer.addSample(channel, startSample + sample, outSample);
            }
                   
            //Canal L, AM off, FILTER on
            else if (channel == 0 && buttonStateAM == 0 && buttonStateFilter == 1)
            {
                auto outSample = myCompressor.processSample(myfilterL.processSample(sampleOut));
                outputBuffer.addSample(channel, startSample + sample, outSample);
            }
            
            //Canal R, AM off, FILTER on
            else if (channel == 1 && buttonStateAM == 0 && buttonStateFilter == 1)
            {
                auto outSample = myCompressor.processSample(myfilterR.processSample(sampleOut));
                outputBuffer.addSample(channel, startSample + sample, outSample);
            }
            
            //Canal L, AM off, FILTER off
            else if (channel == 0 && buttonStateAM == 0 && buttonStateFilter == 0)
            {
                auto outSample = myCompressor.processSample(sampleOut);
                outputBuffer.addSample(channel, startSample + sample, outSample);
            }
            
            //Canal R, AM off, FILTER off
            else if (channel == 1 && buttonStateAM == 0 && buttonStateFilter == 0)
            {
                auto outSample = myCompressor.processSample(sampleOut);
                outputBuffer.addSample(channel, startSample + sample, outSample);
            }
            
            if (! adsr.isActive())
            {
                clearCurrentNote();
            }
        }
    }
}
    

