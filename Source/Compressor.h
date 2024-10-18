#pragma once
#include <JuceHeader.h>
#include <cmath>

class Compressor
{
public:
    Compressor();
    
    //juce::AudioBuffer<float> inputWorkBuffer; //buffer de la signal de entrada a comprimir
    void setSampleRate (float _sampleRate);
    void updateParamsComp (float _threshold, float _ratio, float _Tattk, float _Trls, float _makeUp);
    float processSample(float sampleIn);
    float threshold;
    float ratio;
    float Tattk; //tiempo ataque
    float Trls; // tiempo release
    float makeUp; 
    
    
private:
    float sampleRate; //prepareToPlay
    float alphaAttk; //constantes filtro deteccion
    float alphaRls; //constantes filtro deteccion
    float yn1; //muestra anterior (inicializar)
    float xn;; //muestra in (inicialiar)
    float xg; //muestra in log (inicializar)
    float yg; //muestra out log (inicialziar)
    float xl; //muestra lineal
    float yl; //muestra lineal salida (inicialziar)
    float c; //control signal lineal
    float sampleOut;
};
