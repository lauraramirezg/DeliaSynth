#include "Filter.h"

Filter::Filter():
filterType(1), Q(0.5), Fc(500), sampleRate(44.1e3),
b0(0), b1(0), b2(0),
a1(0), a2(0),
xn1(0), xn2(0),
yn1(0), yn2(0)
{}

void Filter::setSampleRate(float nSampleRate)
{
    sampleRate = nSampleRate;
}

void Filter::calculateCoeff()
{
    const float PI = juce::MathConstants<double>::pi;
    float K = std::tan(PI * Fc / sampleRate);
    if (filterType == 0 || filterType == 1 || filterType == 2) // Pasa Bajos orden 2,4,6
    {
        b0 = ((K * K) * Q) / ((K * K * Q) + K + Q);
        b1 = (2 * K * K * Q) / ((K * K * Q) + K + Q);
        b2 = b0;
        a1 = (2 * Q) * ((K * K) - 1) / ((K * K * Q)+ K + Q);
        a2 = ((K * K * Q) - K + Q) / ((K * K * Q) + K + Q);
    }
    if(filterType == 3 || filterType == 4 || filterType == 5) //Pasa Altos orden 2,4,6
    {
        b0 = Q / ((K * K * Q) + K + Q);
        b1 = -(2 * Q) / ((K * K * Q) + K + Q);
        b2 = b0;
        a1 = (2 * Q) * ((K * K) - 1) / ((K * K * Q) + K + Q);
        a2 = ((K * K * Q) - K + Q) / ((K * K * Q) + K + Q);
    }
}

void Filter::setFc(float nFc)
{
    Fc = nFc;
    calculateCoeff();
}

void Filter::setFilterType(int _filterType)
{
    filterType = _filterType;
    calculateCoeff();
}

void Filter::setQ(float _Q)
{
    Q = _Q;
    calculateCoeff();
}

float Filter::processSample(float xn)
{
    float yn = 0;
    float yN = 0;
    float yM = 0;
    
    //Filtros 2 orden
    if(filterType == 0 || filterType == 3){
        yn = b0 * xn + b1 * xn1 + b2 * xn2 - a1 * yn1 - a2 * yn2;
        y = yn;
    }
    //Filtros 4 orden
    else if(filterType == 1 || filterType == 4){
        yn = b0 * xn + b1 * xn1 + b2 * xn2 - a1 * yn1 - a2 * yn2;
        yN = b0 * yn + b1 * yn1 + b2 * yn2 - a1 * yN1 - a2 * yN2;
        y = yN;
    }
    
    //Filtros 6 orden
    else if(filterType == 2 || filterType == 5){
        yn = b0 * xn + b1 * xn1 + b2 * xn2 - a1 * yn1 - a2 * yn2;
        yN = b0 * yn + b1 * yn1 + b2 * yn2 - a1 * yN1 - a2 * yN2;
        yM = b0 * yN + b1 * yN1 + b2 * yN2 - a1 * yM1 - a2 * yM2;
        y = yM;
    }
    
    xn2 = xn1;
    xn1 = xn;
    yn2 = yn1;
    yn1 = yn;
    yN2 = yN1;
    yN1 = yN;
    yM2 = yM1;
    yM1 = yM;
    
    return y;
}

