#pragma once
#include <JuceHeader.h>
#include <cmath>

class Filter
{
public:
    Filter();
    int filterType;
    void setFc(float nFc);
    void setSampleRate(float nSampleRate);
    void setFilterType(int _filterType);
    void setQ(float _Q);
    float processSample(float xn);
    void calculateCoeff();
    float Q;
private:
    float Fc;
    float sampleRate;
    float b0, b1, b2;
    float a1, a2;
    float xn1, xn2;
    float yn1, yn2;
    float yN1, yN2;
    float yM1, yM2;
    float y;
};
