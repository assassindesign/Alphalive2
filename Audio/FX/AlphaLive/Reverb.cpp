//
//  Reverb.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 21/12/2011.
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

#include "Reverb.h"

ReverbClass::ReverbClass(FXInits* fxInits)
{
    
    mix = mixPrev = mixControlValue = fxInits->padFxReverbMix;
    params.roomSize = roomSizeControl = fxInits->padFxReverbRoomSize;
    params.damping = dampingControl =  fxInits->padFxReverbDamping;
    params.wetLevel = 1.0; //static - mix is controlled by my own algorithm
    params.dryLevel = 0; //same as above...
    params.width = widthControl =  fxInits->padFxReverbWidth;
    params.freezeMode = freezeModeControl =  fxInits->padFxReverbFreezeMode;
    
    alphaTouchParam = fxInits->padFxReverbAlphaTouch;
    alphaTouchReverse = fxInits->padFxReverbAtReverse;
    alphaTouchIntensity = fxInits->padFxReverbAtIntensity;
    
    reverb.setSampleRate(DEFAULT_SAMPLE_RATE);
    
    parameterNames = new StringArray(FXParameters::Reverb::EnumStrings);
    
    ScopedPointer<FXData::MinMax> minMax = new FXData::MinMax();
    minMax->aTouchParam = Mix;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = RoomSize;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Damping;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Width;
    minMaxValues.add(*minMax);
}

ReverbClass::~ReverbClass()
{
    
}

//Function that processes the audio
void ReverbClass::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    //create a COPY of bufferToFill's buffer that will be used to hold processed audio data
    //bufferToFill.buffer will stay unprocessed
    int numChans = bufferToFill.buffer->getNumChannels();
    int numSamps = bufferToFill.numSamples;
    
    AudioSampleBuffer wetBuffer(numChans, numSamps);
    
    for (int i = 0; i < numChans; i++)
        wetBuffer.copyFrom(i, 0, *bufferToFill.buffer, 1, 0, numSamps);
    
    //get audio channel data
    float *channels[2];
    channels[0] = wetBuffer.getArrayOfWritePointers()[0] + bufferToFill.startSample;
    channels[1] = wetBuffer.getArrayOfWritePointers()[1] + bufferToFill.startSample;
    
    //process reverb
    sharedMemory.enter();
    reverb.setParameters(params);
    reverb.processStereo(channels[0], channels[1], wetBuffer.getNumSamples());
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

//Function that takes in a pressure value and modulates the reverb depending on the set AlphaTouch parameter
void ReverbClass::processAlphaTouch (double pressureValue)
{
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
            
        case 1: //Room Size
            if (alphaTouchReverse == false)
                //params.roomSize = roomSizeControl + (pressureValue * (((1.0-roomSizeControl)*alphaTouchIntensity)/MAX_PRESSURE));
                params.roomSize = minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min));

            else
                params.roomSize = roomSizeControl - (pressureValue * (((1.0-(1.0-roomSizeControl))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << params.roomSize << std::endl;
            break;
            
        case 2: //Damping
            if (alphaTouchReverse == false)
                //params.damping = dampingControl + (pressureValue * (((1.0-dampingControl)*alphaTouchIntensity)/MAX_PRESSURE));
                params.damping = value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min);

            else
                params.damping = dampingControl - (pressureValue * (((1.0-(1.0-dampingControl))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << params.damping << std::endl;
            break;
            
        case 3: //Width
            if (alphaTouchReverse == false)
                //params.width = widthControl + (pressureValue * (((1.0-widthControl)*alphaTouchIntensity)/MAX_PRESSURE));
                params.width =  minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min));

            else
                params.width = widthControl - (pressureValue * (((1.0-(1.0-widthControl))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << params.width << std::endl;
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
}

const String ReverbClass::getEffectName()
{
    return effectName;
}

const Array<FXData::MinMax>* ReverbClass::getMinMax()
{
    return &minMaxValues;
}

const StringArray* ReverbClass::getParameterNames()
{
    return parameterNames.get();
}

const int ReverbClass::getNumParameters()
{
    return numParameters;
}

void ReverbClass::setParameter(const int parameter, const double newValue)
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
                case RoomSize:
                    setRoomSize(newValue);
                    break;
                case Damping:
                    setDamping(newValue);
                    break;
                case Width:
                    setWidth(newValue);
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

const double ReverbClass::getParameterValue(const int parameter)
{
    switch (parameter){
        case Mix:
            return mix;
            break;
        case RoomSize:
            return params.roomSize;
            break;
        case Damping:
            return params.damping;
            break;
        case Width:
            return params.width;
            break;
        default:
            jassert(false);
            //Invalid parameter!
            return 0;
            break;
    }
}

void ReverbClass::setMix (float value)
{
    sharedMemory.enter();
    mix = mixControlValue = value * value * value;
    sharedMemory.exit();
    
}
void ReverbClass::setRoomSize (float value)
{
    sharedMemory.enter();
    params.roomSize = roomSizeControl = value;
    sharedMemory.exit();
}
void ReverbClass::setDamping (float value)
{
    sharedMemory.enter();
    params.damping =  dampingControl = value;
    sharedMemory.exit();
}
void ReverbClass::setWidth (float value)
{
    sharedMemory.enter();
    params.width = widthControl = value;
    sharedMemory.exit();
}
void ReverbClass::setFreezeMode (float value)
{
    sharedMemory.enter();
    params.freezeMode =  freezeModeControl = value;
    sharedMemory.exit();
}

void ReverbClass::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}
void ReverbClass::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}
void ReverbClass::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse= value;
    sharedMemory.exit();
}

void ReverbClass::setSampleRate (float value)
{
    reverb.setSampleRate(value);
}
