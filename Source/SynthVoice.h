/*
  ==============================================================================

    SynthVoice.h
    Created: 18 Nov 2023 6:26:37pm
    Author:  user

  ==============================================================================
*/

//Clase que representa la voz de un sintetizador, era de SynthesiserVoice y se usa para que suene un sonido de la clase SynthesiserSound o la personal que cremos SynthSound

//Los metodos que tiene override son de la propia clase y en ellos sobrescribimos segun las necesidades del plugin

//La clase prepareToPlay se creo para inicialziar parametros del oscilador

//Se creo el objeto osc de la clase Oscillator, es el oscilador
//Se creo el objeto gain de la clase Gain para controlar el nivel del osc

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "OscData.h"
#include "Filter.h"
#include "Compressor.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    //bool isVoiceActive () const;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void updateADSR (const float attack, const float decay, const float sustain, const float release);
    void updateGAIN (const float _gain);
    void updateGainOsc1(const float _gain);
    void updateGainOsc2(const float _gain);
    void updateAM(const float _gain, const float _freq);
    void updateFilter(const float _cutoff, const float _Q, const int _filterType);
    void updateComp (float _thresh, float _ratio, float _Tattk, float _Trels, float _makeUp);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    OscData& getOscillator(){return osc;}
    OscData& getOscillator2(){return osc2;}
    OscData& getOscillatorAM(){return oscAM;}
    
    int buttonStateAM;
    int buttonStateFilter;
    float detuningOsc1;
    float detuningOsc2;
    float frequency;
    float newFrequencyOsc1;
    float newFrequencyOsc2;

private:
    
    //creacion objeto adsr
    juce::ADSR adsr;
    
    //crea el objeto adsrParamas que permitira modificar los parametros de la envolvente
    juce::ADSR::Parameters adsrParams;
    
    //crea un buffer para el synth y que entre en sync con el de salida
    juce::AudioBuffer<float> synthBuffer;
    juce::AudioBuffer<float> osc1Buffer;
    juce::AudioBuffer<float> osc2Buffer;
    
    //Buffer para procesar aparte la sintesis AM
    juce::AudioBuffer<float> amBuffer;
    
    //Crea los objetos osc de la clase creada OscData que herada de la clase Oscillator para cada uno de los osciladores
    OscData osc;
    OscData osc2;
    OscData oscAM;
    
    //Crea el objeto gain de la clase Gain del modulo de dsp, permite controlar al amplitud del buffer, se crea uno para cada oscilador y para el master
    juce::dsp::Gain<float> gain;
    juce::dsp::Gain<float> gainOsc1;
    juce::dsp::Gain<float> gainOsc2;
    juce::dsp::Gain<float> gainAM;
    
    //Se crean los objetos filtros de la clase personalizada
    Filter myfilterL;
    Filter myfilterR;
    
    //Objeto compressor
    Compressor myCompressor;
};
