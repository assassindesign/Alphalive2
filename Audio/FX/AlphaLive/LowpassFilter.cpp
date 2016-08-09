/*
 *  LowpassFilter.cpp
 *  AlphaLive
 *
 *  Created by Liam Lacey on 15/12/2011.
 *  Copyright 2010 nu desine.
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
 *
 */

#include "LowpassFilter.h"


LowpassFilter::LowpassFilter(FXInits* fxInits)
{
    filter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2> (1024);
    
    // sample rate
    params[0] = DEFAULT_SAMPLE_RATE;
    // cutoff frequency
    params[1] = frequencyControlValue = fxInits->padFxLpfFreq;
    // Q/Bandwidth
    params[2] = bandwidthControlValue = fxInits->padFxLpfBandwidth;
    
    mix = mixPrev = mixControlValue =  fxInits->padFxLpfMix;
    
    alphaTouchParam = fxInits->padFxLpfAlphaTouch;
    alphaTouchReverse = fxInits->padFxLpfAtReverse;
    alphaTouchIntensity = fxInits->padFxLpfAtIntensity;
    
    parameterNames = new StringArray(FXParameters::LowPassFilter::EnumStrings);

    
    ScopedPointer<FXData::MinMax> minMax = new FXData::MinMax();
    minMax->aTouchParam = Mix;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Frequency;
    minMax->min = 30.0;
    minMax->max = 20000.0;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Bandwidth;
    minMax->min = 1;
    minMax->max = 100.0;
    minMaxValues.add(*minMax);
}

LowpassFilter::~LowpassFilter()
{
	delete filter;
}

//==============================================================================


void LowpassFilter::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    /*
    //get audio channel data
    float *channels[2];
    channels[0] = bufferToFill.buffer->getArrayOfWritePointers()[0] + bufferToFill.startSample;
    channels[1] = bufferToFill.buffer->getArrayOfWritePointers()[1] + bufferToFill.startSample;
    //create audio buffers
    AudioSampleBuffer wetBuffer (channels, bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
     */
    
    //create a COPY of bufferToFill's buffer that will be used to hold processed audio data
    //bufferToFill.buffer will stay unprocessed
    int numChans = bufferToFill.buffer->getNumChannels();
    int numSamps = bufferToFill.numSamples;
    
    AudioSampleBuffer wetBuffer(numChans, numSamps);
    
    for (int i = 0; i < numChans; i++)
        wetBuffer.copyFrom(i, 0, *bufferToFill.buffer, 1, 0, numSamps);
    
    //process filtering
    sharedMemory.enter();
    filter->setParams (params);
    filter->process (wetBuffer.getNumSamples(), wetBuffer.getArrayOfWritePointers());
    sharedMemory.exit();
    
    //set the relative gains of the processed and unprocessed buffers using the 'mix' value
    sharedMemory.enter();
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
        bufferToFill.buffer->applyGainRamp (i,
                                            bufferToFill.startSample,
                                            bufferToFill.numSamples,
                                            1.0 - mixPrev,
                                            1.0 - mix);
        wetBuffer.applyGainRamp (i,
                                 0,
                                 wetBuffer.getNumSamples(),
                                 mixPrev,
                                 mix);
        
    }
    
    mixPrev = mix;
    
    //add the processed buffer to the unprcocessed buffer to create wet/dry mix
    bufferToFill.buffer->addFrom(0, 0, wetBuffer, 0, 0, wetBuffer.getNumSamples());
    if (bufferToFill.buffer->getNumChannels() == 2) 
         bufferToFill.buffer->addFrom(1, 0, wetBuffer, 1, 0, wetBuffer.getNumSamples());
    
    sharedMemory.exit();
}


void LowpassFilter::processAlphaTouch (double pressureValue)
{
    
    if (pressureValue < 0 )
    {
        pressureValue *= -1;
    }
    sharedMemory.enter();
    double value = ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE); //scale incoming value by aTouch intensity

    switch (alphaTouchParam)
    {
        case 0: //Mix
            if (alphaTouchReverse == false)
                //mix = mixControlValue + (pressureValue * (((1.0-mixControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
                mix = value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min);
            else
                mix = mixControlValue - (pressureValue * (((1.0-(1.0-mixControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << mix << std::endl;
            break;
            
        case 1: //Frequency
            if (alphaTouchReverse == false)
            {
                //params[1] = frequencyControlValue + (pressureValue * (((20000.0-frequencyControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
                params[1] =  minMaxValues[alphaTouchParam].min + (value * value * value) * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min);
            }
            else
                params[1] = frequencyControlValue - (pressureValue * (((19970.0-(20000.0 - frequencyControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << params[1] << std::endl;
            break;
            
        case 2: //Bandwidth
            if (alphaTouchReverse == false)
            {
                //params[2] = bandwidthControlValue + (pressureValue * (((100.0-bandwidthControlValue)*alphaTouchIntensity)/MAX_PRESSURE));
                params[2] =  minMaxValues[alphaTouchParam].min + (value * value * value) * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min);
            }
            else
                params[2] = bandwidthControlValue - (pressureValue * (((99.0-(100.0 - bandwidthControlValue))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << params[2] << std::endl;
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
}

const String LowpassFilter::getEffectName()
{
    return effectName;
}

const Array<FXData::MinMax>* LowpassFilter::getMinMax()
{
    return &minMaxValues;
}

const StringArray* LowpassFilter::getParameterNames()
{
    return parameterNames.get();
}

const int LowpassFilter::getNumParameters()
{
    return numParameters;
}

void LowpassFilter::setParameter(const int parameter, const double newValue)
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
                case Frequency:
                    setFrequency(newValue);
                    break;
                case Bandwidth:
                    setBandwidth(newValue);
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

const double LowpassFilter::getParameterValue(const int parameter)
{
    switch (parameter){
        case Mix:
            return mix;
            break;
        case Frequency:
            return params[1];
            break;
        case Bandwidth:
            return params[2];
            break;
        default:
            jassert(false);
            //Invalid parameter!
            return 0;
            break;
    }
}


void LowpassFilter::setMix (double value)
{
    sharedMemory.enter();
    mix = mixControlValue = value * value * value;
    sharedMemory.exit();
}

void LowpassFilter::setFrequency (double value)
{
    sharedMemory.enter();
    params[1] = frequencyControlValue = value;
    sharedMemory.exit();
}

void LowpassFilter::setBandwidth(double value)
{
    sharedMemory.enter();
    params[2] = bandwidthControlValue = value;
    sharedMemory.exit();
}



void LowpassFilter::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void LowpassFilter::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void LowpassFilter::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void LowpassFilter::setSampleRate(float value)
{
    sharedMemory.enter();
    params[0] = value;
    sharedMemory.exit();
}