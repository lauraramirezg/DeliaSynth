/*
  ==============================================================================

    SynthSound.h
    Created: 18 Nov 2023 6:27:03pm
    Author:  user

  ==============================================================================
*/

//Clase que describe los sonidos del synth, se era de la clase SynthesiserSound y lee la informacion midi y el canal

#pragma once
#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override {return true;}
    bool appliesToChannel (int midiChannel) override {return true;}
};
