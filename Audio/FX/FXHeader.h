//
//  FXInits.h
//  iAlpha
//
//  Created by Luke Saxton on 13/10/2015.
//  Copyright 2015 nu desine.
//
//  Alphalive 2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  Alphalive 2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef FXInits_h
#define FXInits_h

#include "Constants.h"
#include "../JuceLibraryCode/JuceHeader.h"

//#include "FXUnit.hpp"
#define MACRO_MAX 1
#define MACRO_MIN -1

struct FXInits
{
    //Gain and Pan
    float padFxGainPanGain = 1.0;
    float padFxGainPanPan = 0.5;
    float padFxGainPanAlphaTouch = 0;
    float padFxGainPanAtReverse = 0;
    float padFxGainPanAtIntensity = 1.0;
    //LPF
    float padFxLpfMix = 1.0;
    float padFxLpfFreq = 500;
    float padFxLpfBandwidth = 5;
    float padFxLpfAlphaTouch = 1;
    float padFxLpfAtReverse = 0;
    float padFxLpfAtIntensity = 1.0;
    //HPF
    float padFxHpfMix = 1.0;
    float padFxHpfFreq = 500;
    float padFxHpfBandwidth = 5;
    float padFxHpfAlphaTouch = 1;
    float padFxHpfAtReverse = 0;
    float padFxHpfAtIntensity = 1.0;
    //BPF
    float padFxBpfMix = 1.0;
    float padFxBpfFreq = 500;
    float padFxBpfBandwidth = 5;
    float padFxBpfAlphaTouch = 1;
    float padFxBpfAtReverse = 0;
    float padFxBpfAtIntensity = 1.0;
    //Delay
    float padFxDelayMix = 0.7;
    float padFxDelayTime = 30000.0/120;
    float padFxDelayFeedback = 0.5;
    float padFxDelayLpfFreq = 20000.0;
    float padFxDelayHpfFreq = 30.0;
    float padFxDelaySync = 1;
    float padFxDelayTimeMenu = 4;
    float padFxDelayAlphaTouch = 2;
    float padFxDelayAtReverse = 0;
    float padFxDelayAtIntensity = 1.0;
    //Reverb
    float padFxReverbMix = 0.7;
    float padFxReverbRoomSize = 0.25;
    float padFxReverbDamping = 0.5;
    float padFxReverbWidth = 0.5;
    float padFxReverbFreezeMode = 0.2;
    float padFxReverbAlphaTouch = 1;
    float padFxReverbAtReverse = 0;
    float padFxReverbAtIntensity = 1.0;
    //Flanger
    float padFxFlangerMix = 0.7;
    float padFxFlangerRate = (120/60.0) * 0.25;
    float padFxFlangerFeedback = 0.9;
    float padFxFlangerIntensity = 0.1;
    float padFxFlangerSync = 1;
    float padFxFlangerRateMenu = 4;
    float padFxFlangerAlphaTouch = 3;
    float padFxFlangerAtReverse = 0;
    float padFxFlangerAtIntensity = 1.0;
    //Tremolo
    float padFxTremoloDepth = 1.0;
    float padFxTremoloRate = (120/60.0) * 1.0;
    float padFxTremoloShape = 1;
    float padFxTremoloSync = 1;
    float padFxTremoloRateMenu = 3;
    float padFxTremoloAlphaTouch = 0;
    float padFxTremoloAtReverse = 0;
    float padFxTremoloAtIntensity = 1.0;
    //Distortion
    float padFxDistortionInputGain = 1.0;
    float padFxDistortionDrive = 50;
    float padFxDistortionTone = 0.8;
    float padFxDistortionWetDryMix = 0.5;
    float padFxDistortionType = 1;
    float padFxDistortionAlphaTouch = 1;
    float padFxDistortionAtReverse = 0;
    float padFxDistortionAtIntensity = 1.0;
    //Bitcrusher
    float padFxBitcrusherInputGain = 1;
    float padFxBitcrusherDownsample = 16;
    float padFxBitcrusherCrush = 2;
    float padFxBitcrusherSmoothing = 0.92;
    float padFxBitcrusherWetDryMix = 0.64;
    float padFxBitcrusherAlphaTouch = 0;
    float padFxBitcrusherAtReverse = 0;
    float padFxBitcrusherAtIntensity = 1.0;
};

namespace FXData
{
    enum FXType
    {
        NoEffect,
        BPFEffect,
        BitcrusherEffect,
        DelayEffect,
        DistortionEffect,
        FlangerEffect,
        GainAndPanEffect,
        HPFEffect,
        LPFEffect,
        ReverbEffect,
        TremoloEffect
    };
    
    struct FXMapping
    {
        FXType fxType = NoEffect;
        String parameterName = NULLSTRING;
        float scale = 1.0;
        float shift = 0;
        bool isInverted = false;
        int aTouchParam = -1;
        bool isEnabled = true;
    };
    
    struct MinMax
    {
        float min = 0;
        float max = 1;
        int aTouchParam = -1;
    };
}


namespace FXParameters
{
    static const char * EffectNames[] = {"No Effect", "BPF","Bitcrusher", "Delay", "Distortion", "Flanger", "Gain & Pan", "HPF", "LPF", "Reverb", "Tremolo", NULL};
    static const int numEffects = 11;
    
    namespace GainAndPan
    {
        static const char * EnumStrings[] = {"Gain", "Pan", NULL};

    }

    namespace BandPassFilter
    {
        static const char * EnumStrings[] = {"Mix", "Frequency", "Bandwidth", NULL};

    }
    
    namespace Bitcrusher
    {
        static const char * EnumStrings[] = {"Input Gain", "DownSample", "Crush", "Smoothing", "Mix", NULL};

    }
    namespace Delay
    {
        static const char * EnumStrings[] = {"Mix", "Delay Time", "Feedback", "LPF Frequency", "HPF Frequency", NULL};

    }
    namespace Distortion
    {
        static const char * EnumStrings[] = {"Input Gain", "Drive", "Tone", "Mix", NULL};
//        static const char* EnumStrings[] = {NULLSTRING, NULLSTRING, "Gain", "Drive", "Tone", "Smoothing", "Mix"};
        
        static const char * DistortionTypeStrings[] = {"Soft", "Hard","HF0", "HF1", "HF2", "Digital", NULL};

    }
    namespace Flanger
    {
        static const char * EnumStrings[] = {"Mix", "Rate", "Feedback", "Intensity", NULL};

    }
    namespace HighPassFilter
    {
        static const char * EnumStrings[] = {"Mix", "Frequency", "Bandwidth", NULL};

    }
    namespace LowPassFilter
    {
        static const char * EnumStrings[] = {"Mix", "Frequency", "Bandwidth", NULL};


    }
    
    namespace Reverb
    {
        static const char * EnumStrings[] = {"Mix", "Room Size", "Damping", "Width", NULL};
        
    }
    
    namespace Tremolo
    {
        static const char * EnumStrings[] = {"Depth", "Rate", "Shape", NULL};

    }
    
}



//Pure Virtual class that all DSP Effects will inherit from, this should allow use of a single pointer in audio processing.
class WrappedAudioEffect
{
    
public:
    WrappedAudioEffect()
    {

    }
    virtual ~WrappedAudioEffect()
    {
        
    }
   
    virtual void processAudio(const AudioSourceChannelInfo& bufferToFill) = 0;
    virtual void processAlphaTouch (double pressureValue) = 0;
    //Functions that set the AlphaTouch settings
    virtual void setAlphaTouchParam (int value) =0;
    virtual void setAlphaTouchIntensity (double value)=0;
    virtual void setAlphaTouchReverse (int value)=0;
    virtual void setSampleRate (float value)=0;
    
    virtual void setDelayTime(double newValue){};
    
    virtual void setDistortionTypeMenu (int value){};
//    virtual const int getLastAlphaTouch(const int forFXType) {return 0;};

    virtual const String getEffectName() = 0;
    virtual const Array<FXData::MinMax>* getMinMax() = 0;
    
    virtual const StringArray* getParameterNames() = 0;
    virtual void setParameter(const int parameter, const double newValue) = 0;
    virtual const double getParameterValue(const int parameter) = 0;
    virtual const int getNumParameters() = 0;
};

//Blank class that inherits from WrappedAudioEffect but doesn't do anything. When an FXUnit has no liveAudioEffect, the pointer can point to an instance of this class, which allows the WrappedAudioEffect functions to be called without changing anything. Better than trying to make function calls on a deleted pointer..
class EmptyAudioEffect : public WrappedAudioEffect
{
    
public:
    EmptyAudioEffect()
    {
        
    }
    
    ~EmptyAudioEffect()
    {
        
    }
    
    void processAudio(const AudioSourceChannelInfo& bufferToFill)
    {
        
    }
    
    void processAlphaTouch (double pressureValue)
    {
        
    }
    
    //Functions that set the AlphaTouch settings
    void setAlphaTouchParam (int value)
    {
        
    }
    
    void setAlphaTouchIntensity (double value)
    {
        
    }
    void setAlphaTouchReverse (int value)
    {
        
    }
    
    void setSampleRate (float value)
    {
        
    }
    
    const String getEffectName()
    {
        return "Empty";
    }
    
    const Array<FXData::MinMax>* getMinMax()
    {
        return nullptr;
    }
    
    const StringArray* getParameterNames()
    {
        return nullptr;
    }
    void setParameter(const int parameter, const double newValue)
    {
        
    }
    const double getParameterValue(const int parameter)
    {
        return 0;
    }
    
    const int getNumParameters()
    {
        return 0;
    }

};

#endif /* FXInits_h */
