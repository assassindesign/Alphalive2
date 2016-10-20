//
//  Reverb.h
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
//

#ifndef H_REVERB
#define H_REVERB

//how should I handle the wet/dry mix for reverb?
//Currently I have done it exactly the same as with the filter - processing a copy of the input buffer
//and then mixing this copy back in with the original unprocessed buffer to create a wet/dry mix
//However the Juce Reverb Class as seperate Dry and Wet paramaters that could be used to change the mix.
//This would probably be easier - will it change the interface and how 'modular' the effect is to the rest of the application?

//IS THIS EFFECT CURRENTLY JUST IN MONO? CHECK IT OUT AND MAKE IT STEREO!
#include "FXHeader.h"

class ReverbClass : public WrappedAudioEffect
{
public:
    enum Parameters
    {
        Mix = 0,
        RoomSize,
        Damping,
        Width
    };
    static const int numParameters = 4;
    
	ReverbClass(FXInits* fxInits);
	~ReverbClass();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill) override;
    //Function that takes in a pressure value and modulates the reverb depending on the set AlphaTouch parameter
    void processAlphaTouch (double pressureValue) override;
    
    //functions to set effect parameters
    void setMix (float value);
    void setRoomSize (float value);
    void setDamping (float value);
    void setWidth (float value);
    void setFreezeMode (float value);
    
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

   
    const String effectName = "Reverb";
    
private:
    
    //the reverb DSP object
    Reverb reverb;
    Reverb::Parameters params;
    
    double mix, mixPrev, mixControlValue;
    double roomSizeControl;
    double dampingControl;
    double widthControl;
    double freezeModeControl;
    
    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    int padNumber;
    
    Array<FXData::MinMax> minMaxValues;
    ScopedPointer<StringArray> parameterNames;
};

#endif //H_REVERB