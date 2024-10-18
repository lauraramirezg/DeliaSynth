/*
  ==============================================================================

    OscData.h
    Created: 20 Nov 2023 4:17:03pm
    Author:  user

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void setWaveType(const int choice);
    
    
private:
    
};
