#include "Compressor.h"

Compressor::Compressor():
threshold(0), ratio(0), Tattk(0), Trls(0), makeUp(0), sampleRate(44.1e3), alphaAttk(0), alphaRls(0),
yn1(0), xn(0), xg(0), yg(0), xl(0), yl(0), c(0), sampleOut(0)
{}

void Compressor::setSampleRate(float _sampleRate)
{
    sampleRate = _sampleRate;
}

void Compressor::updateParamsComp (float _threshold, float _ratio, float _Tattk, float _Trls, float _makeUp)
{
    threshold = _threshold;
    ratio = _ratio;
    Tattk = _Tattk * std::pow(10, -3);
    Trls = _Trls * std::pow(10, -3);
    makeUp = _makeUp;
    
    alphaAttk = std::exp(-1/(Tattk*sampleRate));
    alphaRls = std::exp(-1/(Trls*sampleRate));
}

float Compressor::processSample(float sampleIn)
{
    //Etapa deteccion pico
    xn = std::abs(sampleIn);
    
    //Etapa conversion a dB
    if (xn < 0.001)
    {
        xg = -120;
    }
    else
    {
        xg = 20*std::log10(xn);
    }
    
    //Etapa calculo reduccion
    if (xg >= threshold)
    {
        yg = threshold + (xg - threshold)/ratio;
    }
    else
    {
        yg = xg;
    }
    
    //x de salida comprimido en dB
    xl = xg - yg;
    
    //Etapa suavizado señal
    if (xl > yn1)
    {
        yl = alphaAttk * yn1 + (1 - alphaAttk) * xl;
    }
    else
    {
        yl = alphaRls*yn1 + (1 - alphaRls)* xl;
    }
    
    c = std::pow(10, (makeUp - yl)/20); //control signal
    sampleOut = sampleIn*c;
    yn1 = yl;
    
    return sampleOut;
}
