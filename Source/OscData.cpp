/*
  ==============================================================================

    OscData.cpp
    Created: 20 Nov 2023 4:17:03pm
    Author:  user

  ==============================================================================
*/

#include "OscData.h"

void OscData::setWaveType(const int choice)
{
    switch (choice)
    {
        case 0:
            //sine
            initialise ([](float x) {return std::sin(x);});
            break;
            
        case 1:
            //square
            initialise ([](float x) {return x < 0.0f ? -1.0f : 1.0f;});
            break;
            
        case 2:
            //triangle
            initialise ([](float x) {return std::abs(x) + 1;});
            break;
            
        case 3:
            //saw
            initialise ([](float x) {return x/juce::MathConstants<float>::pi;});
            break;
            
        default:
            jassertfalse;
            break;
    }
}
