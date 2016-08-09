//
//  Gain&Pan.h
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

#ifndef H_GAINPAN
#define H_GAINPAN

#include "FXHeader.h"
#include "PanControl.h"

class GainAndPan : public WrappedAudioEffect
{
public:
    enum Parameters
    {
        Gain = 0,
        Pan
    };
    static const int numParameters = 2;
    
	GainAndPan(FXInits* fxInits);
	~GainAndPan();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill) override;
    //Function that takes in a pressure value and modulates the audio depending on the set AlphaTouch parameter
    void processAlphaTouch (double pressureValue) override;
    
    //functions to set effect parameters
    void setGain (double value);
    void setPan (double value);
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value) override;
    void setAlphaTouchIntensity (double value) override;
    void setAlphaTouchReverse (int value) override;
    
    void setSampleRate (float value) override;
    
    //Added with Macro Ability
    void setParameter(const int parameter, const double newValue) override;
    const String getEffectName() override;
    const Array<FXData::MinMax>* getMinMax() override;
    const StringArray* getParameterNames() override;
    const double getParameterValue(const int parameter) override;
    const int getNumParameters() override;

    const String effectName = "Gain And Pan";
    

private:

    //we need the ...Control variables as the actual gain or pan won't always be
    //equal to the value of their GUI control as the pressure could be used to
    //modulate the value
    double gain, gainPrev, gainControl;
    double pan, panPrev, panControl;

    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    //int padNumber;
    int sampleRate;
    
    Array<FXData::MinMax> minMaxValues;
    ScopedPointer<StringArray> parameterNames;
    
};

#endif //H_GAINPAN