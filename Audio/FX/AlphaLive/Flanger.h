//
//  Flanger.h
//  AlphaLive
//
//  Created by Liam Meredith-Lacey on 14/07/2012.
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

#ifndef H_FLANGER
#define H_FLANGER


#include "jucetice_CircularBuffer.h"
#include "Oscillator.h"
#include "FXHeader.h"

class Flanger : public WrappedAudioEffect
{
public:
    enum Parameters
    {
        Mix = 0,
        Rate,
        Feedback,
        Intensity
    };
    static const int numParameters = 4;
    
	Flanger(FXInits* fxInits);
	~Flanger();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill) override;
    //Function that takes in a pressure value and modulates the effect depending on the set AlphaTouch parameter
    void processAlphaTouch (double pressureValue) override;
    
    //Functions that set the effect parameters
    void setMix (double value);
    void setFeedback (double value);
    void setRate (double value);
    void setIntensity (double value); //LFO depth
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value) override;
    void setAlphaTouchIntensity (double value) override;
    void setAlphaTouchReverse (int value) override;
    
    void setSampleRate (float value) override;
    
    void restart();
    void resetBuffers();
    
    //Added with Macro Ability
    void setParameter(const int parameter, const double newValue) override;
    const String getEffectName() override;
    const Array<FXData::MinMax>* getMinMax() override;
    const StringArray* getParameterNames() override;
    const double getParameterValue(const int parameter) override;
    const int getNumParameters() override;

    const String effectName = "Flanger";

private:

    
    
    //Delay stuff
    CircularBuffer<float> circBufferLeft;
    CircularBuffer<float> circBufferRight;
    AudioSampleBuffer wetBuffer;
    
    Oscillator *lfo;
    
    double mix, mixPrev, mixControlValue;
    double intensity, intensityControlValue;
    double rate, rateControlValue;
    double feedback, feedbackControlValue;
    
    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    float sampleRate; // do i need this?
    
    Array<FXData::MinMax> minMaxValues;
    ScopedPointer<StringArray> parameterNames;
};

#endif //H_FLANGER