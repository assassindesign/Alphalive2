//
//  FXUnit.hpp
//  iAlpha
//
//  Created by Luke Saxton on 13/10/2015.
//  Copyright 2015 nu desine.
//
//  iAlpha is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  iAlpha is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef FXUnit_hpp
#define FXUnit_hpp

#include "FXHeader.h"
#include "GainAndPan.h"
#include "Reverb.h"
#include "BandPassFilter.h"
#include "Bitcrusher.h"
#include "Delay.h"
#include "Flanger.h"
#include "HighPassFilter.h"
#include "LowpassFilter.h"
#include "Tremolo.h"
#include "Distortion.h"


// Wrapper class that provides a unified interface for the selection of audio fx available.
// Parent class will be able set ann FX type, and the FXUnit will create an audio effect of that type
// and begin passing audio through it. Effect mappings can be controlled via a single parameter
// using the processAlphaTouch function (Values from -512 to 511)
class FXUnit : public AudioSource
{
public:
    FXUnit();
    
    ~FXUnit();
    
    //void setFXType(Identifier _FXType);
    void setFXType(const int newFXType);
    const int getFXType();
    
    
    //=Audio Source==============================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    void processAudio(const AudioSourceChannelInfo& bufferToFill);
    
    void processAlphaTouch(double pressureValue);
    
    void setAlphaTouchParam (int value);
    
    void setGain(double newGain);
    
    void setDelayTime(double newValue);
    
    void setDistortionType(const int distortionType);
    const int getDistortionType();
    StringArray getDistortionTypeNames();
    
    
    bool addMapping (FXData::FXMapping newMapping);
    bool addMacro (Array<FXData::FXMapping>* newMacro);
    bool invertMappingForParameter(const int parameter, const bool shouldBeInverted);
    void updateMappingForParameter(const int parameter, const float newScale = 1, const float newShift = 0);	

    const int getLastAlphaTouch(const int forFXType);
    
    // Clear all current alpha touch mappings, option to keep mappings that could be used with current live FX
    void clearMappings(bool keepValidMaps);
    bool setMappingEnabledForParameter(const int parameter, const bool isEnabled);
    
    /** Get currently stored mappings as an array */
    const Array<FXData::FXMapping> getCurrentMappings();
    
    /** Get currently Stored Mappings and parameters as a Value Tree for easy conversion to an xml file */
    const ValueTree getCurrentMappingsAsTree();
    
    /** use a tree of type FXMappingPreset to set up mappings. Structure of this tree is in Mapping Tree Layout omnigraffle.*/
    bool setMappingsFromTree(const ValueTree* mappingTree);
    
    /** convert the current mappings and parameters to a value tree and return this as an XML string.*/
    const String getCurrentMappingsAsXML();
    

    //Added with Macro Ability
    
    /** set any parameter for the live effect. Wrapped effects */
    void setParameter(const int parameter, const double newValue);
    const String getEffectName();
    const Array<FXData::MinMax>* getMinMax();
    const StringArray* getParameterNames();
    const double getParameterValue(const int parameter);
    
    
    const StringArray* getEffectNames();
    
private:
    
    FXData::FXType fxType;
    
    FXInits fxInits;
    
    WrappedAudioEffect* liveAudioEffect;
    WrappedAudioEffect* prevAudioEffect;

    OwnedArray<FXData::FXMapping> currentMappings;
    
    ScopedPointer<GainAndPan> masterGainAndPan;
    ScopedPointer<EmptyAudioEffect> emptyAudioEffect;
    ScopedPointer<GainAndPan> gainAndPan;
    ScopedPointer<ReverbClass> reverb;
    ScopedPointer<BandPassFilter> bandPassFilter;
    ScopedPointer<Bitcrusher> bitcrusher;
    ScopedPointer<Delay> delay;
    ScopedPointer<Flanger> flanger;
    ScopedPointer<HighPassFilter> highPassFilter;
    ScopedPointer<LowpassFilter> lowPassFilter;
    ScopedPointer<Tremolo> tremolo;
    ScopedPointer<Distortion> distortion;

    CriticalSection SharedMemory;
    
    ScopedPointer<StringArray> effectNames;
    String liveEffectName;
    double sampleRate, currentGain;
    
    const int numEffects = 11;
};

#endif /* FXUnit_hpp */
