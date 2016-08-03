//
//  PlayableSphere.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
//
//

#include "PlayableSphere.hpp"
#include "MidiScales.h"
#include "AlphaLive2Engine.hpp"

PlayableSphere::PlayableSphere(const int numPads, const int _sphereID) : sphereID(_sphereID)
{
    
    destination = InternalMidiRouter::MidiDestination::MidiOut;
    
    sphereData = AppData::Instance()->getSphereData(sphereID);
    
    if (sphereData == nullptr) {
        sphereID = AppData::Instance()->createNewSphereDataObject(numPads);
    }
    
    
    if (numPads > 0)
    {
        for (int i = 0; i < numPads; i++)
        {
            playablePads.add(new PlayablePad(sphereData->getPadData(i)));
        }
        
    }
    
    if (AppData::Instance()->getEnginePointer()->getMidiRouterPointer() != 0)
    {
        router = AppData::Instance()->getEnginePointer()->getMidiRouterPointer();
        mapSphere(36, SphereData::MappedScale::Natural, SphereData::RowConfig::CenterRow);
    }
    else
    {
        jassert(false);
        //Can't route things to a router that doesn't exist..
    }
    
}


PlayableSphere::~PlayableSphere()
{
    
}

void PlayableSphere::mapSphere(const int rootNote, const SphereData::MappedScale scale, const SphereData::RowConfig config = SphereData::OneRow)
{
    if (rootNote != sphereData->getRootNote())
    {
        sphereData->setRootNote(rootNote);
    }
    
    if (scale != sphereData->getScale())
    {
        sphereData->setScale(scale);

    }
    if (config != sphereData->getRowConfig())
    {
        sphereData->setRowConfig(config);

    }
    
    sphereData->getScaleData()->setKey(sphereData->getRootNote()%12);
    sphereData->getScaleData()->setOctave((rootNote/12)-2);
    
    int octaveCount, positionInScale;
    octaveCount = positionInScale = 0;
    
    int index = 0;
    Array<int> sphereMask;
    sphereMask.clear();
    
    switch (config)
    {
        case SphereData::OneRow:
            while (SphereMask::OneRowMask[index] != -1)
            {
                sphereMask.add(SphereMask::OneRowMask[index]);
                index++;
            }
            break;
        case SphereData::TwoRow:
            while (SphereMask::TwoRowMask[index] != -1)
            {
                sphereMask.add(SphereMask::TwoRowMask[index]);
                index++;
            }
            break;
        case SphereData::CenterRow:
            while (SphereMask::TwoRowMask[index] != -1)
            {
                sphereMask.add(SphereMask::CenterRowsMask[index]);
                index++;
            }
            break;
        default:
            jassert(false); //how did you manage that?
            break;
    }
    
    
    
    if (router != nullptr)
    {
        router->panic(destination);
    }
    //DBG("Map: " + String(rootNote) + " : " + String(scale));
    
    int maskValue, octaveRoot, newMidiNote;
    for (int i = 0; i < playablePads.size(); i++)
    {
        maskValue = sphereMask.getUnchecked(i);
        
        
        octaveCount = maskValue / 7;
        positionInScale =  maskValue % 7;
        
        octaveRoot = rootNote + (octaveCount * 12);
        
        switch (scale) {
            case SphereData::Major:
                newMidiNote = octaveRoot + IntervalSpacings::Major[positionInScale];
                break;
            case SphereData::Natural:
                newMidiNote = octaveRoot + IntervalSpacings::NaturalMinor[positionInScale];
                break;
            case SphereData::Harmonic:
                newMidiNote = octaveRoot + IntervalSpacings::HarmonicMinor[positionInScale];
                break;
            case SphereData::Hungarian:
                newMidiNote = octaveRoot + IntervalSpacings::Hungarian[positionInScale];
                break;
            case SphereData::Chromatic:
                newMidiNote = i+rootNote;
            default:
                newMidiNote = 0;
                break;
        }
        if (!playablePads.getUnchecked(i)->setMidiNote(newMidiNote)) //overrun protection. If Midi note is out of range, set to max.
        {
            playablePads.getUnchecked(i)->setMidiNote(127);
        }
    }
    
}


void PlayableSphere::mapSphere(const int key, const int octave, const int scale, const int config = SphereData::OneRow)
{
    int rootNote = 36 + (key + 12*octave); //C3 = 60; C-2 = 0;
    
    mapSphere(rootNote, scale, config);
}

void PlayableSphere::setRootNote(const int newRootNote)
{
    mapSphere(newRootNote, sphereData->getScale(), sphereData->getRowConfig());
}


void PlayableSphere::setRowConfig(const SphereData::RowConfig newConfig)
{
    mapSphere(sphereData->getRootNote(), sphereData->getScale(), newConfig);
}


void PlayableSphere::hitPad(const int padID, const int vel)
{
    if (sphereData->getSphereMidiEnabled())
    {
        if (padID < playablePads.size())
        {
            playablePads[padID]->hitPad(vel);
        }
    }
    
}
void PlayableSphere::pressPad(const int padID, const float pressure)
{
    if (sphereData->getSphereMidiEnabled())
    {
        if (padID < playablePads.size())
        {
            playablePads[padID]->pressPad(pressure);
        }
    }
    
}

void PlayableSphere::midiThruToDestination (const int note, const int vel)
{
    if (sphereData->getMidiThruEnabled())
    {
        router->handleInternalMidiNoteWithDestination(destination, note, vel);

    }
}

void PlayableSphere::setDestination (const InternalMidiRouter::MidiDestination newDestination)
{
    destination = newDestination;
}


void PlayableSphere::setScale (SphereData::MappedScale newScale)
{
    mapSphere(sphereData->getRootNote(), newScale, sphereData->getRowConfig());
}

void PlayableSphere::setMidiThruEnabled (bool shouldBeEnabled)
{
    sphereData->setMidiThruEnabled(shouldBeEnabled);
    if (!shouldBeEnabled)
    {
        router->panic(destination);
    }
}

void PlayableSphere::setSphereMidiEnabled(bool shouldBeEnabled)
{
    sphereData->setSphereMidiEnabled(shouldBeEnabled);
    if (!shouldBeEnabled)
    {
        router->panic(destination);
    }
}

void PlayableSphere::setSphereID(const int newID)
{
    sphereID = newID;
}

const int PlayableSphere::getSphereID()
{
    return sphereID;
}

void PlayableSphere::transposeMidiByOctave(const int octavesToTranspose)
{
    mapSphere(sphereData->getRootNote() + (octavesToTranspose*12), sphereData->getScale(), sphereData->getRowConfig());
}

void PlayableSphere::transposeMidiByNote(const int semiTonesToTranspose)
{
    mapSphere(sphereData->getRootNote() + semiTonesToTranspose, sphereData->getScale(), sphereData->getRowConfig());

}

SphereData* PlayableSphere::getSphereDataObject()
{
    return sphereData;
}


