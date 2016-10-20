//
//  Bitcrusher.h
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


/*
 This seems like a good article on DSP Bitcrusher http://music.columbia.edu/cmc/music-dsp/FAQs/guitar_Bitcrusher_FAQ.html .
 It states that the processing chain of a good 'overdrive' Bitcrusher effect should be in the following format:
 
 Pre-EQ (BPF) -> Pre-gain -> Nonlinear waveshaping function -> Post-EQ (LPF) -> Post-gain
 
 In the simplest form the user could be allowed to control both filters centre/cutoff frequencies ('Tone 1' and 'Tone 2'), pre-gain 
 ('Overdrive?') and post-gain (Gain), though post-gain probably won't need to be an AlphaTouch parameter.
 
 In a more advanced Bitcrusher effect, the user could have access certain parameters of the waveshaping function in order
 to have more control over the exact Bitcrusher sound. Also they could have control over the bandwidths of the filters.
 
 In an even more advanced Bitcrusher effect, multiple BPF could be used as part of the pre-EQ with all centre frequencies
 and bandwidths being controllable.
 
 
 Things to consider
 - also seen a LPF as the pre-EQ, would this be better or needed as well?
 - what BPF from the dspfiltercpp library should I used? BandPass1 or BandPass2?
 
 */

#ifndef H_Bitcrusher
#define H_Bitcrusher

#include "Dsp.h"
#include "FXHeader.h"

class Bitcrusher : public WrappedAudioEffect
{
public:
    enum Parameters
    {
        InputGain = 0,
        DownSample,
        Crush,
        Smoothing,
        Mix
    };
    static const int numParameters = 5;

    
	Bitcrusher(FXInits* fxInits);
	~Bitcrusher();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill) override;
    //Function that takes in a pressure value and modulates the audio depending on the set AlphaTouch parameter
    void processAlphaTouch (double pressureValue) override;
    
    //Functions that set the effect parameters
    void setInputGain (double value);
    void setCrush (int value);
	void setDownsample(int value);
    void setMix (double value);
	void setSmoothing (double value);
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value) override;
    void setAlphaTouchIntensity (double value) override;
    void setAlphaTouchReverse (int value) override;
    
    void setGain (double value);
    void setSampleRate (float value) override;

    
    //Added with Macro Ability
    void setParameter(const int parameter, const double newValue) override;
    const String getEffectName() override;
    const Array<FXData::MinMax>* getMinMax() override;
    const StringArray* getParameterNames() override;
    const double getParameterValue(const int parameter) override;
    const int getNumParameters() override;
    
    const String effectName = "BitCrusher";
    
private:

    
    //The filters
    Dsp::Filter *smoothingFilter;
    Dsp::Params paramsSmoothingFilter;
	
	float sampleL, sampleR;
	int downsample, downsampleControl, counter;
	
    double wetDryMixControl, wetDryMix, wetDryMixPrev;
	int crush, crushControlValue;
	double smoothingControl;
    double inputGain, inputGainPrev, inputGainControl;
	
	float *pIn[2], *leftChannel, *rightChannel;

    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
	
	CriticalSection sharedMemory;
    
    Array<FXData::MinMax> minMaxValues;
    ScopedPointer<StringArray> parameterNames;
};
#endif //H_Bitcrusher