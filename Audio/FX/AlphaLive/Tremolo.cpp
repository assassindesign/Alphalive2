//
//  Tremolo.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 13/07/2012.
//  Updated for new Framework by Luke Saxton on 13/10/2015
//  Copyright 2015 nu desine.
//
//  This file is part of AlphaLive.
//
//  AlphaLive is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  AlphaLive is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Tremolo.h"


Tremolo::Tremolo(FXInits* fxInits)
{
    lfo = new Oscillator (DEFAULT_SAMPLE_RATE);
    
    //setSampleRate(sampleRate_); //do actually i need to do this here?
    
    depth = depthControlValue =  fxInits->padFxTremoloDepth;
    rate = rateControlValue = fxInits->padFxTremoloRate;
    shape = shapeControlValue = fxInits->padFxTremoloShape;
    
    alphaTouchParam = fxInits->padFxTremoloAlphaTouch;
    alphaTouchReverse = fxInits->padFxTremoloAtReverse;
    alphaTouchIntensity = fxInits->padFxTremoloAtIntensity;
    
    parameterNames = new StringArray(FXParameters::BandPassFilter::EnumStrings);
    
    ScopedPointer<FXData::MinMax> minMax = new FXData::MinMax();

    minMax->aTouchParam = Depth;
    minMax->min = 0;
    minMax->max = 1;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Rate;
    minMax->min = 0;
    minMax->max = 50;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Shape;
    minMax->min = 1;
    minMax->max = 5;
    minMaxValues.add(*minMax);
    
    
}

Tremolo::~Tremolo()
{
	delete lfo;
}

//==============================================================================


void Tremolo::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    
    sharedMemory.enter();
    
    //double rate = effectParam[0] * (20.0/1.0);
    //double depth = effectParam[1];
    //depth = pow(depth, 0.3); //use this to give finer controller over the higher depths. The lower the number exponent the finer.
    //int waveShape = 1.0 + (effectParam[2] * (4.0/1.0));
    float lfoValue;
    
    //get first pair of sample data from incoming buffer
    float *pOutL = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    float *pOutR = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    
    //increment through each pair of samples
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        //changing the depth below needs to be done with a gain ramp for smoothness
        
        //process LFO
        lfoValue = lfo->process(rate, shape);
        //scale and offset output so the osc can be used as an LFO
        lfoValue = (lfoValue * (depth/2.0)) + (1 - (depth/2.0)); 
        //apply LFO object to sample data
        *pOutL = *pOutL * lfoValue;
        *pOutR = *pOutR * lfoValue;
        
        //move to next pair of samples
        pOutL++;
        pOutR++;
        
    }
    
    sharedMemory.exit();
    
}


void Tremolo::processAlphaTouch (double pressureValue)
{
    
    sharedMemory.enter();
    double value = ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE); //scale incoming value by aTouch intensity

    switch (alphaTouchParam)
    {
        case 0: //Depth
            if (alphaTouchReverse == false)
                //depth = depthControlValue + (pressureValue * (((1.0-depthControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
                depth = value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min);

            else
                depth = depthControlValue - (pressureValue * (((1.0-(1.0-depthControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << depth << std::endl;
            break;
            
        case 1: //rate
            
            if (alphaTouchReverse == false)
                //rate = rateControlValue + (pressureValue * (((50.0-rateControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
                rate = minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min));

            else
                rate = rateControlValue - (pressureValue * (((50.0-(50.0-rateControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << rate << std::endl;
            break;
            
        case 2: //shape
            if (alphaTouchReverse == false)
                //shape = roundToInt (shapeControlValue + (pressureValue * (((5.0-shapeControlValue)*alphaTouchIntensity)/MAX_PRESSURE)));
                shape = roundToInt(minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min)));

            else
                shape = roundToInt (shapeControlValue - (pressureValue * (((4.0-(5.0-shapeControlValue))*alphaTouchIntensity)/MAX_PRESSURE))); 
            
            
            //std::cout << shape << std::endl;
            break;
            
            
        default:
            break;
    }
    sharedMemory.exit();
     
}

const String Tremolo::getEffectName()
{
    return effectName;
}

const Array<FXData::MinMax>* Tremolo::getMinMax()
{
    return &minMaxValues;
}

const StringArray* Tremolo::getParameterNames()
{
    return parameterNames.get();
}

const int Tremolo::getNumParameters()
{
    return numParameters;
}

void Tremolo::setParameter(const int parameter, const double newValue)
{
    if (parameter >= 0 && parameter < numParameters) //if parameter is valid
    {
        FXData::MinMax minMax = minMaxValues.getUnchecked(parameter);
        
        if (newValue >= minMax.min && newValue <= minMax.max) //if value is in range
        {
            switch (parameter){
                case Depth:
                    setDepth(newValue);
                    break;
                case Rate:
                    setRate(newValue);
                    break;
                case Shape:
                    setShape(newValue);
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

const double Tremolo::getParameterValue(const int parameter)
{
    switch (parameter){
        case Depth:
            return depth;
            break;
        case Rate:
            return rate;
            break;
        case Shape:
            return shape;
            break;
        default:
            jassert(false);
            //Invalid parameter!
            return 0;
            break;
    }
}



void Tremolo::restart()
{
    //restarts the lfo's cycle. This is called when the audio file is first triggered.
    //would be good to have the option to sync the lfo to the global clock too.
    lfo->restart();
}

void Tremolo::setDepth (double value)
{
    sharedMemory.enter();
    depth = depthControlValue =  value;
    sharedMemory.exit();
}

void Tremolo::setRate (double value)
{
    sharedMemory.enter();
    rate = rateControlValue = value;
    sharedMemory.exit();
}

void Tremolo::setShape (int value)
{
    sharedMemory.enter();
    shape = shapeControlValue = value;
    sharedMemory.exit();
}


void Tremolo::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void Tremolo::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void Tremolo::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void Tremolo::setSampleRate(float value)
{
    sharedMemory.enter();
    sampleRate = value;
    lfo->setSampleRate(value);
    sharedMemory.exit();
}

