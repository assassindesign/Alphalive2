//
//  Gain&Pan.cpp
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 22/12/2011.
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
//

#include "GainAndPan.h"
//#include "../../File and Settings/AppSettings.h"

//#define PAD_SETTINGS AppSettings::Instance()->padSettings[padNumber]

GainAndPan::GainAndPan(FXInits* fxInits)
{
    gain = gainPrev =  gainControl = fxInits->padFxGainPanGain;
    pan = panPrev = panControl = fxInits->padFxGainPanPan;
    //pan.setPanControlValue(panControl);
    
    alphaTouchParam = fxInits->padFxGainPanAlphaTouch;
    alphaTouchReverse = fxInits->padFxGainPanAtReverse;
    alphaTouchIntensity = fxInits->padFxGainPanAtIntensity;
    
    sampleRate = DEFAULT_SAMPLE_RATE;
    
    parameterNames = new StringArray(FXParameters::GainAndPan::EnumStrings);

    
    ScopedPointer<FXData::MinMax> minMax = new FXData::MinMax();
    minMax->aTouchParam = Gain;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Pan;
    minMaxValues.add(*minMax);
    
   
    
}

GainAndPan::~GainAndPan()
{
    
}

//Function that processes the audio
void GainAndPan::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    /*
    //get first pair of sample data from audio buffer
    float *pOutL = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    float *pOutR = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    
    sharedMemory.enter();
    //increment through each pair of samples (left channel and right channel) in the current block of the audio buffer
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        //pan and gain
        *pOutL = panObject.leftChanPan(*pOutL) * gain;
        *pOutR = panObject.rightChanPan(*pOutR) * gain;
        
        //move to next pair of samples
        pOutL++;
        pOutR++;
    }
    sharedMemory.exit();
     */
    
    //gain and pan
    sharedMemory.enter();
    
    for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
    {
        if (i == 0) //left chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                PanControl::leftChanPan_(panPrev) * gainPrev,
                                                PanControl::leftChanPan_(pan) * gain);
        else if (i == 1) // right chan
            bufferToFill.buffer->applyGainRamp (i,
                                                bufferToFill.startSample,
                                                bufferToFill.numSamples,
                                                PanControl::rightChanPan_(panPrev) * gainPrev,
                                                PanControl::rightChanPan_(pan) * gain);
    }
    
    gainPrev = gain;
    panPrev = pan;
    sharedMemory.exit();
    
}

//Function that takes in a pressure value and modulates the gain/pan depending on the set AlphaTouch parameter
void GainAndPan::processAlphaTouch (double pressureValue)
{
    
    sharedMemory.enter();
    double value = ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE); //scale incoming value by aTouch intensity

    switch (alphaTouchParam)
    {
        case 0: //Gain
            if (alphaTouchReverse == false)
                //gain = gainControl + (pressureValue * (((1.0-gainControl)*alphaTouchIntensity)/MAX_PRESSURE));
                gain = minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min));
            else
                gain = gainControl - (pressureValue * (((1.0-(1.0-gainControl))*alphaTouchIntensity)/MAX_PRESSURE));
            
            //std::cout << gain << std::endl;
            break;
            
        case 1: //Pan
            if (alphaTouchReverse == false)
                //pan = panControl + (pressureValue * (((1.0-panControl)*alphaTouchIntensity)/MAX_PRESSURE));
                pan = minMaxValues[alphaTouchParam].min + (value * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min));

            else
                pan = panControl - (pressureValue * (((1.0-(1.0-panControl))*alphaTouchIntensity)/MAX_PRESSURE));
            
            break;
            
        default:
            break;
    }
    sharedMemory.exit();
     
}
const String GainAndPan::getEffectName()
{
    return effectName;
}

const Array<FXData::MinMax>* GainAndPan::getMinMax()
{
    return &minMaxValues;
}

const StringArray* GainAndPan::getParameterNames()
{
    return parameterNames.get();
}

const int GainAndPan::getNumParameters()
{
    return numParameters;
}

void GainAndPan::setParameter(const int parameter, const double newValue)
{
    if (parameter >= 0 && parameter < numParameters) //if parameter is valid
    {
        FXData::MinMax minMax = minMaxValues.getUnchecked(parameter);
        
        if (newValue >= minMax.min && newValue <= minMax.max) //if value is in range
        {
            switch (parameter){
                case Gain:
                    setGain(newValue);
                    break;
                case Pan:
                    setPan(newValue);
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

const double GainAndPan::getParameterValue(const int parameter)
{
    switch (parameter){
        case Gain:
            return gain;
            break;
        case Pan:
            return pan;
            break;
        default:
            jassert(false);
            //Invalid parameter!
            return 0;
            break;
    }
}

void GainAndPan::setGain (double value)
{
    sharedMemory.enter();
    
    if (value <= 1.0)
    {
        gain = value * value * value;
        gainControl = gain;
    }
    else
        gain = gainControl = value;
    
    sharedMemory.exit();
    
}
void GainAndPan::setPan (double value)
{
    sharedMemory.enter();
    pan = panControl = value;
    sharedMemory.exit();
}

void GainAndPan::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}
void GainAndPan::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}
void GainAndPan::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse= value;
    sharedMemory.exit();
}

void GainAndPan::setSampleRate (float value)
{
    //don;t actually need this function of variable - pan or gain don't need to know the sample rate
    sampleRate = value;
}
