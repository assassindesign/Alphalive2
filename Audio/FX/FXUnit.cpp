//
//  FXUnit.cpp
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

#include "FXUnit.hpp"


FXUnit::FXUnit()
{
    sampleRate = DEFAULT_SAMPLE_RATE;

    fxType =  FXData::NoEffect;
    
    masterGainAndPan = new GainAndPan(&fxInits);
    
    emptyAudioEffect = new EmptyAudioEffect();

    liveAudioEffect = emptyAudioEffect.get();
    liveEffectName = emptyAudioEffect->getEffectName();
    
    const char * EffectNames[] = {"No Effect", "BPF","Bitcrusher", "Delay", "Distortion", "Flanger", "Gain & Pan", "HPF", "LPF", "Reverb", "Tremolo", NULL};
    effectNames = new StringArray(EffectNames);
    
    flanger = new Flanger(&fxInits);
}

FXUnit::~FXUnit()
{
    
}

//void FXUnit::setFXType(Identifier _FXType)
//{
//    //FXData::FXType newFxType = getFXEnumFromIdentifier(_FXType);
//    
//    if (newFxType != fxType)
//    {
//        setFXType(newFxType);
//    }
//   
//
//}

const int FXUnit::getFXType()
{
    return fxType;
}

void FXUnit::setFXType(const int newFXType)
{
    if (fxType != newFXType)
    {
        prevAudioEffect = liveAudioEffect;
        
        switch (newFXType)
        {
            case FXData::GainAndPanEffect:
                fxType = FXData::GainAndPanEffect;
                gainAndPan = new GainAndPan(&fxInits);
                liveAudioEffect = gainAndPan;
                break;
            case FXData::ReverbEffect:
                fxType = FXData::ReverbEffect;
                reverb = new ReverbClass(&fxInits);
                liveAudioEffect = reverb;
                break;
            case FXData::BPFEffect:
                fxType = FXData::BPFEffect;
                bandPassFilter = new BandPassFilter(&fxInits);
                liveAudioEffect = bandPassFilter;
                break;
            case FXData::BitcrusherEffect:
                //jassertfalse;
                //AlphaTouchParams not working.
                
                fxType = FXData::BitcrusherEffect;
                bitcrusher = new Bitcrusher(&fxInits);
                liveAudioEffect = bitcrusher;
                break;
            case FXData::DelayEffect:
                fxType = FXData::DelayEffect;
                delay = new Delay(&fxInits);
                liveAudioEffect = delay;
                break;
            case FXData::FlangerEffect:
                fxType = FXData::FlangerEffect;
                flanger = new Flanger(&fxInits);
                liveAudioEffect = flanger;
                break;
            case FXData::HPFEffect:
                fxType = FXData::HPFEffect;
                highPassFilter = new HighPassFilter(&fxInits);
                liveAudioEffect = highPassFilter;
                break;
            case FXData::LPFEffect:
                fxType = FXData::LPFEffect;
                lowPassFilter = new LowpassFilter(&fxInits);
                liveAudioEffect = lowPassFilter;
                break;
            case FXData::TremoloEffect:
                fxType = FXData::TremoloEffect;
                tremolo = new Tremolo(&fxInits);
                liveAudioEffect = tremolo;
                break;
            case FXData::DistortionEffect:
                fxType = FXData::DistortionEffect;
                distortion = new Distortion(&fxInits, sampleRate);
                liveAudioEffect = distortion;
                break;
            case FXData::NoEffect:
                fxType = FXData::NoEffect;
                liveAudioEffect = emptyAudioEffect.get();
                break;
            default:
                
                break;
        }
        
        liveAudioEffect->setSampleRate(sampleRate);
        
        if (liveAudioEffect != emptyAudioEffect.get())
        {
            prevAudioEffect = nullptr;
        }
        
        liveEffectName = liveAudioEffect->getEffectName();
        clearMappings(false);
        
//        if (liveAudioEffect != emptyAudioEffect.get())
//        {
//            for (int i = 0; i < liveAudioEffect->getParameterNames()->size(); i++)
//            {
//                currentMappings.add(new FXData::FXMapping());
//                currentMappings.getLast()->fxType = fxType;
//                currentMappings.getLast()->aTouchParam = i;
//                currentMappings.getLast()->isInverted = false;
//                currentMappings.getLast()->isEnabled = false;
//            }
//        }
     
    }
    
    
    

}

bool FXUnit::addMacro (Array<FXData::FXMapping>* newMacro)
{
    bool success = true;
    for (int i = 0; i < newMacro->size(); i++)
    {
        if (!addMapping(newMacro->getUnchecked(i)))
        {
            success = false;
        }
    }
    
    return success;
}

bool FXUnit::addMapping (FXData::FXMapping newMapping)
{
    if (newMapping.aTouchParam < 0)
    {
        newMapping.aTouchParam = -1;
        ScopedPointer<StringArray> enumStrings;
        switch (newMapping.fxType)
        {
            case FXData::GainAndPanEffect:
                enumStrings = new StringArray(FXParameters::GainAndPan::EnumStrings);
                break;
            case FXData::ReverbEffect:
                enumStrings = new StringArray(FXParameters::Reverb::EnumStrings);
                break;
            case FXData::BPFEffect:
                enumStrings = new StringArray(FXParameters::BandPassFilter::EnumStrings);
                break;
            case FXData::BitcrusherEffect:
                enumStrings = new StringArray(FXParameters::Bitcrusher::EnumStrings);
                break;
            case FXData::DelayEffect:
                enumStrings = new StringArray(FXParameters::Delay::EnumStrings);
                break;
            case FXData::FlangerEffect:
                enumStrings = new StringArray(FXParameters::Flanger::EnumStrings);
                break;
            case FXData::HPFEffect:
                enumStrings = new StringArray(FXParameters::HighPassFilter::EnumStrings);
                break;
            case FXData::LPFEffect:
                enumStrings = new StringArray(FXParameters::LowPassFilter::EnumStrings);
                break;
            case FXData::TremoloEffect:
                enumStrings = new StringArray(FXParameters::Tremolo::EnumStrings);
                break;
            case FXData::DistortionEffect:
                enumStrings = new StringArray(FXParameters::Distortion::EnumStrings);
                break;
            case FXData::NoEffect:
                break;
            default:
                break;
        }
        
        
        if (enumStrings.get())
        {
            newMapping.aTouchParam = enumStrings->indexOf(newMapping.parameterName);
        }
    }
    
    

    
    if (newMapping.aTouchParam >= 0)
    {
        for (int i = 0; i < currentMappings.size(); i++)
        {
            if ((currentMappings[i]->fxType == fxType && currentMappings[i]->FXMapping::aTouchParam == newMapping.aTouchParam))
            {
                currentMappings.remove(i);
            }
        }

        currentMappings.addIfNotAlreadyThere(new FXData::FXMapping(newMapping));
        return true;
    }
    else
    {
        DBG("Mapping for " + newMapping.parameterName + " is not valid - Parameter does not exist for this effect : " + liveAudioEffect->getEffectName());
        return false;
    }
    
}
bool FXUnit::invertMappingForParameter(const int parameter, const bool shouldBeInverted)
{
    bool success = false;
    for (int i = 0; i < currentMappings.size(); i++)
    {
        if (currentMappings[i]->aTouchParam == parameter)
        {
            currentMappings[i]->isInverted = shouldBeInverted;
            success = true;
        }
    }
    
    return success;
}

bool FXUnit::setMappingEnabledForParameter(const int parameter, const bool isEnabled)
{
    bool success = false;
    for (int i = 0; i < currentMappings.size(); i++)
    {
        if (currentMappings[i]->aTouchParam == parameter)
        {
            currentMappings[i]->isEnabled = isEnabled;
            success = true;
        }
    }
    
    if (!success && isEnabled)
    {
        FXData::FXMapping newMapping;
        newMapping.aTouchParam = parameter;
        newMapping.fxType = fxType;
        addMapping(newMapping);
    }
    
    return success;
}

void FXUnit::updateMappingForParameter(const int parameter, const float newScale, const float newShift)
{
    bool mappingFound = false;
    
    if (newScale >= 0 && newScale <= 1) //if new scale is in range
    {
        if (newShift >= 0 && newShift <= 1)
        {
            for (int i = 0; i < currentMappings.size(); i++) // search through current mappings to find mapping for parameter
            {
                if (currentMappings[i]->aTouchParam == parameter)
                {
                    currentMappings[i]->scale = newScale; //update mapping if found
                    currentMappings[i]->shift = newShift;

                    mappingFound = true;
                }
            }
            
            if (!mappingFound) // if mapping not found, create mapping for parameter with new scale.
            {
                FXData::FXMapping newMapping;
                newMapping.aTouchParam = parameter;
                newMapping.scale = newScale;
                newMapping.shift = newShift;
                addMapping(newMapping);
            }
        }
        else
        {
            DBG("Trying to set mapping with out of range Shift : " + String(newShift));
        }
    }
    else
    {
        DBG("Trying to set mapping with out of range Scale : " + String(newScale));
    }

}


void FXUnit::processAlphaTouch(double pressureValue)
{
    if (currentMappings.size() <= 0) //if there are no mappings
    {
        //liveAudioEffect->processAlphaTouch(pressureValue);
    }
    else
    {
        //ScopedLock sl(SharedMemory);
        for (int i = 0; i < currentMappings.size(); i++) //cycle through all mappings
        {
            if (currentMappings[i]->fxType == fxType) //if a mapping matches the current fxtype
            {
                if (currentMappings[i]->aTouchParam >= 0 && currentMappings[i]->isEnabled) //and the parameter is valid and enabled
                {
                    liveAudioEffect->setAlphaTouchParam(currentMappings[i]->aTouchParam);
        
                    static double scaledPressureValue;
                    
                    if (currentMappings[i]->isInverted)
                    {
                        scaledPressureValue = double(MAX_PRESSURE) - pressureValue;
                    }
                    else
                    {
                        scaledPressureValue = pressureValue;
                    }
                    
                    
                   scaledPressureValue = (currentMappings[i]->shift * MAX_PRESSURE) + (scaledPressureValue * currentMappings[i]->scale);
                    
                    if (scaledPressureValue > MAX_PRESSURE)
                    {
                        DBG("Pressure Value Out of Range : " + liveAudioEffect->getEffectName() + " : " + getParameterNames()->strings.getUnchecked(currentMappings[i]->aTouchParam) + ":" + String(scaledPressureValue));
                        scaledPressureValue = MAX_PRESSURE;
                        
                    }
                    
                    if (scaledPressureValue < 0)
                    {
                        DBG("Pressure Value Out of Range : " + liveAudioEffect->getEffectName() + " : " + getParameterNames()->strings.getUnchecked(currentMappings[i]->aTouchParam) + ":" + String(scaledPressureValue));
                        scaledPressureValue = 0;
                    }
                    
                    //DBG(String(currentMappings[i]->shift) + ":" +String(scaledPressureValue));
                    
                    //liveAudioEffect->processAlphaTouch(scaledPressureValue);
                    liveAudioEffect->processAlphaTouch(scaledPressureValue);
                }

            }
            else
            {
                //jassert(false);
                DBG("Attempting to use Macro for the wrong effect..");
            }
            
        }
    }
    
}

const String FXUnit::getEffectName()
{
    return liveEffectName;
}

const Array<FXData::MinMax>* FXUnit::getMinMax()
{
    return liveAudioEffect->getMinMax();
}

const StringArray* FXUnit::getParameterNames()
{
    return liveAudioEffect->getParameterNames();
}

void FXUnit::setParameter(const int parameter, const double newValue)
{
    liveAudioEffect->setParameter(parameter, newValue);
}

const double FXUnit::getParameterValue(const int parameter)
{
    return liveAudioEffect->getParameterValue(parameter);
}

void FXUnit::setAlphaTouchParam (int value)
{
    if (value >= 0)
    {
        liveAudioEffect->setAlphaTouchParam(value);
    }
}



void FXUnit::setGain(double newGain)
{
    currentGain = newGain;
    masterGainAndPan->setParameter(GainAndPan::Gain, newGain);
}


void FXUnit::prepareToPlay (int samplesPerBlockExpected, double _sampleRate)
{
    sampleRate = _sampleRate;
    
    liveAudioEffect->setSampleRate(sampleRate);

    //flanger->resetBuffers();
    flanger->restart();
}
void FXUnit::releaseResources()
{
    
}

void FXUnit::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    masterGainAndPan->processAudio(bufferToFill);
    liveAudioEffect->processAudio(bufferToFill);
}

void FXUnit::setDelayTime(double newValue)
{
    if (fxType == FXData::DelayEffect)
    {
        if (delay)
        {
            delay->setDelayTime(newValue);
        }
    }
    else
    {
        jassertfalse;
        //Can't set the delay time if this FXUnit isn't a delay!
    }
}


void FXUnit::setDistortionType(const int distortionType)
{
    if (fxType == FXData::DistortionEffect)
    {
        if (distortionType > 0 && distortionType <= Distortion::numDistortionTypes)
        {
            distortion->setDistortionTypeMenu(distortionType);
        }
        
    }
    else
    {
        jassert(false);
        //Can't set the distortion type if this FXUnit isn't a distortion!
    }
}

const int FXUnit::getDistortionType()
{
    if (fxType == FXData::DistortionEffect);
    {
        return distortion->getDistortionType();
    }
}


void FXUnit::clearMappings(bool keepValidMaps)
{
    ScopedLock sl(SharedMemory);
    if (keepValidMaps)
    {
        for (int i = 0; i < currentMappings.size(); i++)
        {
            if (!(currentMappings[i]->fxType == fxType))
            {
                currentMappings.remove(i);
            }
        }
    }
    else
    {
        currentMappings.clearQuick(true);
    }
}

const Array<FXData::FXMapping> FXUnit::getCurrentMappings()
{
    Array<FXData::FXMapping> mappingsToReturn;

    for (int i = 0; i < currentMappings.size(); i++)
    {
        mappingsToReturn.add(*currentMappings.getUnchecked(i));
    }

    return mappingsToReturn;
    //return *new Array<FXData::FXMapping>(currentMappings.getRawDataPointer());
}

const ValueTree FXUnit::getCurrentMappingsAsTree()
{
    /**
    ScopedPointer<ValueTree> returnTree = new ValueTree(FXMappingPresetType);
    
    for (int param = 0; param < liveAudioEffect->getNumParameters(); param++)
    {
//        FXType fxType = NoEffect;
//        String parameterName = NULLSTRING;
//        float scale = 1.0;
//        float shift = 0;
//        bool isInverted = false;
//        int aTouchParam = -1;
//        bool isEnabled = true;
        
        //  returnTree->addChild(*new ValueTree(FXParameterMappingType), param, 0);
        
     
        int mappingIndex = -1;
        for (int mapping = 0; mapping < currentMappings.size(); mapping++)
        {
            if ((currentMappings[mapping]->aTouchParam == param) && currentMappings[mapping]->isEnabled)
            {
                mappingIndex = mapping;
            }
        }
        
        
        if (mappingIndex >= 0) //if mapping is found
        {
            returnTree->addChild(*new ValueTree(FXParameterMappingType), param, 0);

            returnTree->getChild(param).setProperty(FXTypeEnumType, currentMappings[mappingIndex]->fxType, 0);
            returnTree->getChild(param).setProperty(FXParameterNameType, liveAudioEffect->getParameterNames()->strings[param], 0);
            returnTree->getChild(param).setProperty(FXMappingScaleType, currentMappings[mappingIndex]->scale, 0);
            returnTree->getChild(param).setProperty(FXMappingShiftType, currentMappings[mappingIndex]->shift, 0);
            returnTree->getChild(param).setProperty(FXMappingIsInvertedType, currentMappings[mappingIndex]->isInverted, 0);
            returnTree->getChild(param).setProperty(FXMappingATouchParamType, currentMappings[mappingIndex]->aTouchParam, 0);
            returnTree->getChild(param).setProperty(FXMappingIsEnabledType , currentMappings[mappingIndex]->isEnabled, 0);
            //returnTree->getChild(param).setProperty(FXParameterSettingValueType , getParameterValue(param), 0);
            
        }
        else if (mappingIndex == -1)//no mapping for this param
        {
            returnTree->addChild(*new ValueTree(FXParameterSettingType), param, 0);

            returnTree->getChild(param).setProperty(FXTypeEnumType, fxType, 0);
            returnTree->getChild(param).setProperty(FXParameterSettingValueType, getParameterValue(param), 0);
            returnTree->getChild(param).setProperty(FXParameterNameType, liveAudioEffect->getParameterNames()->strings[param], 0);
            returnTree->getChild(param).setProperty(FXMappingATouchParamType, param, 0);
        }

    }
    
    return *returnTree.get();
     */
    
}

bool FXUnit::setMappingsFromTree(const ValueTree* mappingTree)
{
    /**
    ValueTree mappingInfoTree = *mappingTree;
    
    if (mappingTree->getType() == FXMappingPresetType)
    {
        if (mappingTree->getChildWithName(FXMappingPresetType).isValid())
        {
            jassert(false);
            //value tree is not valid - contains more than one fxmapping preset;
            mappingInfoTree = mappingInfoTree.getChildWithName(FXMappingPresetType);
            
        }
        
        clearMappings(false);
        
        ValueTree currentBranch;
        
        for (int i = 0; i < mappingInfoTree.getNumChildren(); i++)
        {
            currentBranch = mappingInfoTree.getChild(i);
            
            if (currentBranch.getType() == FXParameterMappingType)
            {
                
                FXData::FXMapping newMapping;
                
                setFXType(currentBranch.getProperty(FXTypeEnumType, 0));
                
                newMapping.fxType = fxType;
                newMapping.parameterName = currentBranch.getProperty(FXParameterNameType, NULLSTRING);
                newMapping.scale = currentBranch.getProperty(FXMappingScaleType, newMapping.scale);
                newMapping.shift = currentBranch.getProperty(FXMappingShiftType, newMapping.shift);
                newMapping.isInverted = currentBranch.getProperty(FXMappingIsInvertedType, newMapping.isInverted);
                newMapping.aTouchParam = currentBranch.getProperty(FXMappingATouchParamType, newMapping.aTouchParam);
                newMapping.isEnabled = currentBranch.getProperty(FXMappingIsEnabledType, newMapping.isEnabled);
                
                addMapping(newMapping);
                
                
            }
            else if (currentBranch.getType() == FXParameterSettingType)
            {
                setFXType(currentBranch.getProperty(FXTypeEnumType, 0));
                
                setParameter(int(currentBranch.getProperty(FXMappingATouchParamType, -1)), double(currentBranch.getProperty(FXParameterSettingValueType, -1)));
                
            }

        }
        
        return true;
    }
    else
    {
        jassert(false);
        //That's not an FXMappingPreset!
        return false;
    }
    */
}



const String FXUnit::getCurrentMappingsAsXML()
{
    return getCurrentMappingsAsTree().toXmlString();
}

