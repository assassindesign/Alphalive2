//
//  Flanger.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 14/07/2012.
//  Copyright 2012 nu desine.
//
#include "Flanger.h"

#define MAX_BUFFER_SIZE 2000  //max number of samples needed?

Flanger::Flanger(FXInits* fxInits)
:   circBufferLeft(MAX_BUFFER_SIZE),
    circBufferRight(MAX_BUFFER_SIZE),
    wetBuffer(2, 44100) //buffer size only needs to be the size of bufferToFill below. how do i set that here?
{    
    lfo = new Oscillator (DEFAULT_SAMPLE_RATE);
    
    //delay stuff
    circBufferLeft.reset();
    circBufferRight.reset();
    wetBuffer.clear();
    
    //SET DEAFULT SETTINGS HERE FROM PAD_SETTINGS
    mix = mixPrev = mixControlValue = fxInits->padFxFlangerMix;
    rate = rateControlValue = fxInits->padFxFlangerRate;
    feedback = feedbackControlValue = fxInits->padFxFlangerFeedback;
    intensity = intensityControlValue = fxInits->padFxFlangerIntensity;
    
    alphaTouchParam = fxInits->padFxFlangerAlphaTouch;
    alphaTouchReverse = fxInits->padFxFlangerAtReverse;
    alphaTouchIntensity = fxInits->padFxFlangerAtIntensity;
    
    parameterNames = new StringArray(FXParameters::Flanger::EnumStrings);

    
    ScopedPointer<FXData::MinMax> minMax = new FXData::MinMax();
    minMax->aTouchParam = Mix;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Rate;
    minMax->min = 0.01;
    minMax->max = 20;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Feedback;
    minMax->min = 0.01;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Intensity;
    minMax->min = 0.01;
    minMaxValues.add(*minMax);
    
}

Flanger::~Flanger()
{
	delete lfo;
}

//==============================================================================


void Flanger::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    
    sharedMemory.enter();
    //float feedback = effectParam[2];
    //double depth = effectParam[3]; //intensity
    //float rate = effectParam[4] * (10.0/1.0);
    //float mix = effectParam[0];
    float delaySamples;
    
    //get first pair of sample data from incoming buffer
    float *pOutL = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    float *pOutR = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    //get the first pair of samples from the processed buffer
    float *pWetL = wetBuffer.getWritePointer (0, bufferToFill.startSample);
    float *pWetR = wetBuffer.getWritePointer (1, bufferToFill.startSample);
    
    //increment through each pair of samples
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        //set the output sample data by adding the wet buffer with a feedback value
        *pOutL = *pOutL + (*pWetL * feedback);
        *pOutR = *pOutR + (*pWetR * feedback);
        
        //set the circular buffers sample data
        circBufferLeft.put(*pOutL);
        circBufferRight.put(*pOutR);
        
        //create the modulating delay time based on the osc
        delaySamples = lfo->process(rate, 3);
        //scale and offset the osc from +1 - -1 to 500 - 0 so it can be used as an LFO
        delaySamples = (delaySamples * (intensity * 250.0)) + (intensity * 250.0);
        
        //set the wet buffer sample data with a delay value
        *pWetL = circBufferLeft.get(delaySamples);
        *pWetR = circBufferRight.get(delaySamples);
        
        //move to next pair of samples
        pOutL++;
        pOutR++;
        pWetL++;
        pWetR++;
    }
    
    sharedMemory.exit();
    
    //set wet buffer gain
    sharedMemory.enter();
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
        wetBuffer.applyGainRamp (i,
                                 bufferToFill.startSample,
                                 bufferToFill.numSamples,
                                 mixPrev,
                                 mix);
        //wetBuffer.appl
    }
    mixPrev = mix;
    sharedMemory.exit();
    
}


void Flanger::processAlphaTouch (double pressureValue)
{
    
    if (pressureValue < 0)
    {
        pressureValue = pressureValue * -1;
    }
    
    sharedMemory.enter();
    double value = ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE); //scale incoming value by aTouch intensity

    switch (alphaTouchParam)
    {
        case 0: //Mix
            if (alphaTouchReverse == false)
                //mix = mixControlValue + (pressureValue * (((1.0-mixControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
                mix = minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min));
            else
                mix = mixControlValue - (pressureValue * (((1.0-(1.0-mixControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << mix << std::endl;
            break;
            
        case 1: //rate
            
            if (alphaTouchReverse == false)
                //rate = rateControlValue + (pressureValue * (((20.0-rateControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
                rate = minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min));
            else
                rate = rateControlValue - (pressureValue * (((20.0-(20.0-rateControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << rate << std::endl;
            break;
            
        case 2: //feedback
            if (alphaTouchReverse == false)
                //feedback = feedbackControlValue + (pressureValue * (((1.0-feedbackControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
                feedback = minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min));
            else
                feedback = feedbackControlValue - (pressureValue * (((1.0-(1.0-feedbackControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << feedback << std::endl;
            break;
            
        case 3: //intensity
            if (alphaTouchReverse == false)
                intensity = intensityControlValue = minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min));

            else
                intensity = intensityControlValue - (pressureValue * (((1.0-(1.0-intensityControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << intensity << std::endl;
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
     
     
    
}

const String Flanger::getEffectName()
{
    return effectName;
}

const Array<FXData::MinMax>* Flanger::getMinMax()
{
    return &minMaxValues;
}

const StringArray* Flanger::getParameterNames()
{
    return parameterNames.get();
}

const int Flanger::getNumParameters()
{
    return numParameters;
}

void Flanger::setParameter(const int parameter, const double newValue)
{
    if (parameter >= 0 && parameter < numParameters) //if parameter is valid
    {
        FXData::MinMax minMax = minMaxValues.getUnchecked(parameter);
        
        if (newValue >= minMax.min && newValue <= minMax.max) //if value is in range
        {
            switch (parameter){
                case Mix:
                    setMix(newValue);
                    break;
                case Rate:
                    setRate(newValue);
                    break;
                case Feedback:
                    setFeedback(newValue);
                    break;
                case Intensity:
                    setIntensity(newValue);
                    break;
                default:
                    break;
            }
        }
        else
        {
            DBG("Parameter Value out of range: " + effectName + " : " + parameterNames.get()->strings[parameter] + " : " + String(newValue));
        }
        
    }
    else
    {
        jassert(false);
        //invalid parameter!
    }
    
}

const double Flanger::getParameterValue(const int parameter)
{
    switch (parameter){
        case Mix:
            return mix;
            break;
        case Rate:
            return rate;
            break;
        case Feedback:
            return feedback;
            break;
        case Intensity:
            return intensity;
            break;
        default:
            jassert(false);
            //Invalid parameter!
            return 0;
            break;
    }
}

void Flanger::restart()
{
    //restarts the lfo's cycle. This is called when the audio file is first triggered.
    //would be good to have the option to sync the lfo to the global clock too.
    lfo->restart();
}

void Flanger::resetBuffers()
{
    circBufferLeft.reset();
    circBufferRight.reset();
    wetBuffer.clear();
}

void Flanger::setMix (double value)
{
    sharedMemory.enter();
    mix = mixControlValue =  value;
    sharedMemory.exit();
}

void Flanger::setRate (double value)
{
    sharedMemory.enter();
    rate = rateControlValue = value;
    sharedMemory.exit();
}

void Flanger::setFeedback (double value)
{
    sharedMemory.enter();
    feedback = feedbackControlValue = value;
    sharedMemory.exit();
}

void Flanger::setIntensity (double value)
{
    sharedMemory.enter();
    intensity = intensityControlValue = value;
    sharedMemory.exit();
}


void Flanger::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void Flanger::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void Flanger::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void Flanger::setSampleRate(float value)
{
    sharedMemory.enter();
    sampleRate = value;
    lfo->setSampleRate(value);
    sharedMemory.exit();
}
