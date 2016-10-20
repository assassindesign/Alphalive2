//
//  Bitcrusher.cpp
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

#include "Bitcrusher.h"


Bitcrusher::Bitcrusher(FXInits* fxInits)
{    
	//===Smoothing-Filter===
    //filter
    smoothingFilter = new Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2> (1024);
    // sample rate
    paramsSmoothingFilter[0] = DEFAULT_SAMPLE_RATE;
    // centre frequency
    paramsSmoothingFilter[1] = fxInits->padFxBitcrusherSmoothing * 10000;
    // Q/Bandwidth (static)
    paramsSmoothingFilter[2] = 2.0;
    
    smoothingControl = fxInits->padFxBitcrusherSmoothing;
	
	//input gain
    inputGain = inputGainPrev = inputGainControl = fxInits->padFxBitcrusherInputGain;
	
	//crush value
	crush = crushControlValue = fxInits->padFxBitcrusherCrush;
	
	downsample = downsampleControl = fxInits->padFxBitcrusherDownsample;
	
    //wet/dry mix ratio
    wetDryMixPrev = wetDryMix = wetDryMixControl = fxInits->padFxBitcrusherWetDryMix;
    
    alphaTouchParam = fxInits->padFxBitcrusherAlphaTouch;
    alphaTouchReverse = fxInits->padFxBitcrusherAtReverse;
    alphaTouchIntensity = fxInits->padFxBitcrusherAtIntensity;

    sampleL = 0.0;
    sampleR = 0.0;
    counter = downsample;
    
    ScopedPointer<FXData::MinMax> minMax = new FXData::MinMax();

    
    minMax->aTouchParam = InputGain;
    minMax->min = 0.01;
    minMax->max = 1;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = DownSample;
    minMax->min = 1;
    minMax->max = 16;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Crush;
    minMax->min = 1;
    minMax->max = 8;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Smoothing;
    minMax->min = 0.01;
    minMax->max = 1;
    minMaxValues.add(*minMax);
    
    minMax = new FXData::MinMax();
    minMax->aTouchParam = Mix;
    minMax->min = 0.01;
    minMaxValues.add(*minMax);
    
    parameterNames = new StringArray(FXParameters::Bitcrusher::EnumStrings);

}

Bitcrusher::~Bitcrusher()
{
    delete smoothingFilter;
}

//==============================================================================


void Bitcrusher::processAudio (const AudioSourceChannelInfo& bufferToFill)
{
    //==============================================================================
    //======================AUDIO PROCESSING CODE GOES IN HERE======================
    //bufferToFill.buffer is the audio buffer you will want to use.
    //Use the effectParam array of floats as the variables within the DSP code that you want
    //controllable by the GUI. The slider numbers match the index numbers of the array
    //e.g. Slider 0 = effectParam[0], Slider 1 = effectParam[1] and so on.
    //==============================================================================
    
	sharedMemory.enter();
    //get audio channel data
	pIn[0] = bufferToFill.buffer->getArrayOfWritePointers()[0] + bufferToFill.startSample;
	pIn[1] = bufferToFill.buffer->getArrayOfWritePointers()[1] + bufferToFill.startSample;
    
	AudioSampleBuffer dryBuffer(pIn, bufferToFill.buffer->getNumChannels(),bufferToFill.numSamples);
	AudioSampleBuffer wetBuffer(bufferToFill.buffer->getNumChannels(),bufferToFill.numSamples);
	sharedMemory.exit();
	
	wetBuffer.copyFrom(0, 0, dryBuffer, 0, 0, wetBuffer.getNumSamples());
	wetBuffer.copyFrom(1, 0, dryBuffer, 1, 0, wetBuffer.getNumSamples());
	
	sharedMemory.enter();
    //===apply pre-gain===
	for (int i = 0; i < bufferToFill.buffer->getNumChannels(); ++i)
	{
		wetBuffer.applyGainRamp(i, 0, wetBuffer.getNumSamples(), inputGain, inputGainPrev);	
	}	
	
	sharedMemory.exit();
	inputGainPrev = inputGain;
	
	leftChannel = rightChannel = 0;
	
	//link wetBuffer to wet pointer variables
	leftChannel = wetBuffer.getWritePointer(0); 
	rightChannel = wetBuffer.getWritePointer(1); 
	
	sharedMemory.enter();
    //increment through each pair of samples (left channel and right channel) in the current block of the audio buffer
    for (int i = 0; i < wetBuffer.getNumSamples(); ++i)
    {		
		if (--counter == 0) 
		{
			sampleL = *leftChannel;
			sampleR = *rightChannel;
			
			counter = downsample;
		}
		
		int sL = sampleL * 32768, sR = sampleR * 32768, index = 16 - crush;
		
		sL = sL >> index;
		sR = sR >> index;
		sL = sL << index;
		sR = sR << index;
		
		*leftChannel = float(sL) / 32768.0;
		*rightChannel = float(sR) / 32768.0;
		
		*leftChannel *= 0.6;
		*rightChannel *= 0.6;
		
		//move to next pair of samples
		leftChannel++;
		rightChannel++;
    }
	sharedMemory.exit();
	
	sharedMemory.enter();
    //===apply smoothing-filter1===
    smoothingFilter->setParams(paramsSmoothingFilter);
    smoothingFilter->process(wetBuffer.getNumSamples(), wetBuffer.getArrayOfWritePointers());
	sharedMemory.exit();
	
    //set the relative gains of the processed and unprocessed buffers using the 'mix' value
	dryBuffer.applyGainRamp(0, 0, bufferToFill.buffer->getNumSamples(), (1.0-wetDryMix), (1.0-wetDryMixPrev));
	dryBuffer.applyGainRamp(1, 0, bufferToFill.buffer->getNumSamples(), (1.0-wetDryMix), (1.0-wetDryMixPrev));
	
	wetBuffer.applyGainRamp(0, 0, wetBuffer.getNumSamples(), wetDryMix, wetDryMixPrev);
	wetBuffer.applyGainRamp(1, 0, wetBuffer.getNumSamples(), wetDryMix, wetDryMixPrev);
	
	wetDryMixPrev = wetDryMix;
	
	sharedMemory.enter();
	//add the processed buffer to the unprcocessed buffer to create wet/dry mix
	bufferToFill.buffer->addFrom(0, 0, wetBuffer, 0, 0, wetBuffer.getNumSamples());
    bufferToFill.buffer->addFrom(1, 0, wetBuffer, 1, 0, wetBuffer.getNumSamples());
    sharedMemory.exit();
}


void Bitcrusher::processAlphaTouch (double pressureValue)
{
    //std::cout << pressureValue << std::endl;
        
    
    sharedMemory.enter();
    //double value = ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE); //scale incoming value by aTouch intensity

    switch (alphaTouchParam)
    {

		case 0: //Input gain
            if (alphaTouchReverse == false)
            {
                //inputGain = inputGainControl + (pressureValue * (((1.0-inputGainControl)*alphaTouchIntensity)/MAX_PRESSURE));
                inputGain =  minMaxValues[alphaTouchParam].min + ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE) * (minMaxValues[alphaTouchParam].max -  minMaxValues[alphaTouchParam].min);
			}
            else
            {
                inputGain = inputGainControl - (pressureValue * (((1.0-(1.0-inputGainControl))*alphaTouchIntensity)/MAX_PRESSURE));
			}
            
            inputGain = inputGain * inputGain * inputGain;
			
			//std::cout << inputGain << std::endl;
			break;
			
		case 1: //downsample
            if (alphaTouchReverse == false)
            {
                //downsample = roundToInt (downsampleControl + (pressureValue * (((16.0 - downsampleControl)*alphaTouchIntensity)/MAX_PRESSURE)));
                downsample =  roundToInt(minMaxValues[alphaTouchParam].min + ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE) * (minMaxValues[alphaTouchParam].max -  minMaxValues[alphaTouchParam].min));
            }
			else
            {
                downsample = roundToInt (downsampleControl - (pressureValue * (((15.0-(16.0-downsampleControl))*alphaTouchIntensity)/MAX_PRESSURE)));
            }
            
			//std::cout << downsample << std::endl;
			break;
			
        case 2: //crush
            if (alphaTouchReverse == false)
            {
                //crush = roundToInt (crushControlValue + (pressureValue * (((8.0 - crushControlValue)*alphaTouchIntensity)/MAX_PRESSURE)));
                crush =  roundToInt(minMaxValues[alphaTouchParam].min + ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE) * (minMaxValues[alphaTouchParam].max -  minMaxValues[alphaTouchParam].min));

            }
			else
            {
                crush = roundToInt (crushControlValue - (pressureValue * (((7.0 - (8.0 - crushControlValue))*alphaTouchIntensity)/MAX_PRESSURE)));
            }
            
			//std::cout << crush << std::endl;
			
			break;
			
		case 3: //Smoothing
            if (alphaTouchReverse == false)
            {
                //paramsSmoothingFilter[1] = 10000 * (smoothingControl + (pressureValue * (((1.0-smoothingControl)*alphaTouchIntensity)/MAX_PRESSURE)));
                setSmoothing((minMaxValues[alphaTouchParam].min + ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE) * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min)));
                
            }
			else
                paramsSmoothingFilter[1] = 10000 * (smoothingControl - (pressureValue * (((0.99-(1.0-smoothingControl))*alphaTouchIntensity)/MAX_PRESSURE)));
           
			//std::cout << paramsSmoothingFilter[1] << std::endl;
			break;
			
        case 4: //Mix
            if (alphaTouchReverse == false)
            {
                //wetDryMix = wetDryMixControl + (pressureValue * (((1.0-wetDryMixControl)*alphaTouchIntensity)/MAX_PRESSURE));
                wetDryMix =  minMaxValues[alphaTouchParam].min + ((pressureValue*alphaTouchIntensity )/MAX_PRESSURE) * (minMaxValues[alphaTouchParam].max - minMaxValues[alphaTouchParam].min);

            }
			else
            {
                wetDryMix = wetDryMixControl - (pressureValue * (((1.0-(1.0-wetDryMixControl))*alphaTouchIntensity)/MAX_PRESSURE));
			}
			
            //wetDryMix = wetDryMix * wetDryMix * wetDryMix;
            
			//std::cout << wetDryMix << std::endl;
			break;

        default:
            break;
    }
    sharedMemory.exit();
	
}

const String Bitcrusher::getEffectName()
{
    return effectName;
}

const Array<FXData::MinMax>* Bitcrusher::getMinMax()
{
    return &minMaxValues;
}

const StringArray* Bitcrusher::getParameterNames()
{
    return parameterNames.get();
}

void Bitcrusher::setParameter(const int parameter, const double newValue)
{
    if (parameter >= 0 && parameter < numParameters) //if parameter is valid
    {
        FXData::MinMax minMax = minMaxValues.getUnchecked(parameter);
        
        if (newValue >= minMax.min && newValue <= minMax.max) //if value is in range
        {
            switch (parameter){
                case InputGain:
                    setInputGain(newValue);
                    break;
                case DownSample:
                    setDownsample(newValue);
                    break;
                case Crush :
                    setCrush(newValue);
                    break;
                case Smoothing:
                    setSmoothing(newValue);
                    break;
                case Mix:
                    setMix(newValue);
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

const double Bitcrusher::getParameterValue(const int parameter)
{
    switch (parameter){
        case InputGain:
            return inputGain;
            break;
        case DownSample:
            return downsample;
            break;
        case Crush :
            return crush;
            break;
        case Smoothing:
            return smoothingControl;
            break;
        case Mix:
            return wetDryMix;
            break;
        default:
            jassert(false);
            //invalid parameter!
            return 0;
            break;
    }
    

}

const int Bitcrusher::getNumParameters()
{
    return numParameters;
}

void Bitcrusher::setInputGain (double value)
{
    sharedMemory.enter();
    value = value*value*value;
    inputGain = inputGainControl = value;
    sharedMemory.exit();
}

void Bitcrusher::setCrush(int value)
{

	sharedMemory.enter();
    crush = crushControlValue = value;
    sharedMemory.exit();
}

void Bitcrusher::setDownsample(int value)
{
	sharedMemory.enter();
	downsample = downsampleControl = value;
	sharedMemory.exit();
}

void Bitcrusher::setMix (double value)
{
    sharedMemory.enter();
    value = value*value*value;
    wetDryMix = wetDryMixPrev = wetDryMixControl = value;
    sharedMemory.exit();
}

void Bitcrusher::setSmoothing (double value)
{
    sharedMemory.enter();
	paramsSmoothingFilter[1] = value * 10000;
    smoothingControl = value;
    sharedMemory.exit();
    
}

void Bitcrusher::setAlphaTouchParam (int value)
{
    sharedMemory.enter();
    alphaTouchParam = value;
    sharedMemory.exit();
}

void Bitcrusher::setAlphaTouchIntensity (double value)
{
    sharedMemory.enter();
    alphaTouchIntensity = value;
    sharedMemory.exit();
}

void Bitcrusher::setAlphaTouchReverse (int value)
{
    sharedMemory.enter();
    alphaTouchReverse = value;
    sharedMemory.exit();
}

void Bitcrusher::setSampleRate(float value)
{
    sharedMemory.enter();
    paramsSmoothingFilter[0] = value;
    sharedMemory.exit();
}
