//
//  SphereData.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 15/07/2016.
//
//

#include "SphereData.hpp"

SphereData::SphereData(int numPads, const int _sphereID) : sphereID(_sphereID)
{
    for (int i = 0 ; i < numPads; i++)
    {
        padData.add(new PadData(this));
        padData.getLast()->setPadID(i);
    }
}

SphereData::~SphereData()
{
    
}

ValueTree* SphereData::toValueTree()
{
    ScopedLock sl(dataLock);
    ValueTree* outputTree = new ValueTree("SphereData");
    outputTree->setProperty("SphereID", sphereID, 0);
    outputTree->setProperty("SphereType", int(sphereType), 0);
    
    for (int i = 0 ; i < padData.size(); i++)
    {
        outputTree->addChild(*padData[i]->toValueTree(), -1, 0);
    }
    
    return outputTree;
}

//============= GETS ===========================================
//Objects
PadData* SphereData::getPadData(const int forPad)
{
    if (forPad > -1 && forPad < padData.size())
    {
        return padData.getUnchecked(forPad);
    }
    else
    {
        return nullptr;
    }
    
}

const int SphereData::getNumPadDataObjects()
{
    return padData.size();
}

TempoData* SphereData::getTempoData()
{
    return &tempoData;
}

ScaleData* SphereData::getScaleData()
{
    return &scaleData;
}

//Variables
const int SphereData::getSphereID()
{
    return sphereID;
}

const SphereData::SphereType SphereData::getSphereType()
{
    return sphereType;
}

const int SphereData::getRootNote()
{
    return rootNote;
}

const int SphereData::getOctave()
{
    return octave;
}

const SphereData::MappedScale SphereData::getScale()
{
    return scale;
}

const SphereData::RowConfig SphereData::getRowConfig()
{
    return rowConfig;
}

const bool SphereData::getMidiThruEnabled()
{
    return midiThruEnabled;
}

const bool SphereData::getSphereMidiEnabled()
{
    return sphereMidiEnabled;
}

//============= SETS ===========================================
//Variables
bool SphereData::setSphereID(const int newID)
{
    bool success = true;
    if (newID > -1)
    {
        dataLock.enter();
        sphereID = newID;
        dataLock.exit();
        callListeners(DataIDS::SphereID);
    }
    else
    {
        success = false;
    }
    return success;
}

bool SphereData::setSphereType(const int newType)
{
    bool success = true;
    if (newType > -1 && newType < FINAL_SPHERETYPE)
    {
        dataLock.enter();
        sphereType = SphereType(newType);
        dataLock.exit();
        callListeners(DataIDS::SphereTypeData);
    }
    else
    {
        success = false;
    }
    return success;
}

bool SphereData::setRootNote(const int newRootNote)
{
    bool success = true;
    if (newRootNote) 
    {
        dataLock.enter();
        rootNote = newRootNote;
        dataLock.exit();
        callListeners(DataIDS::RootNote);
    }
    else
    {
        success = false;
    }
    return success;
}

bool SphereData::setOctave(const int newOctave)
{
    bool success = true;
    if (newOctave > -3 && newOctave < 9)
    {
        dataLock.enter();
        octave = newOctave;
        dataLock.exit();
        callListeners(DataIDS::Octave);
    }
    else
    {
        success = false;
    }
    return success;
}

bool SphereData::setScale(const int newScale)
{
    bool success = true;
    if (newScale > -1 && newScale < FINAL_SCALE)
    {
        dataLock.enter();
        scale = MappedScale(newScale);
        dataLock.exit();
        callListeners(DataIDS::SphereID);
    }
    else
    {
        success = false;
    }
    return success;
}

bool SphereData::setRowConfig(const int newConfig)
{
    bool success = true;
    if (newConfig > -1 && newConfig < FINAL_ROWCONF)
    {
        dataLock.enter();
        rowConfig = RowConfig(newConfig);
        dataLock.exit();
        callListeners(DataIDS::RowConfigData);
    }
    else
    {
        success = false;
    }
    return success;
}

void SphereData::setMidiThruEnabled(const bool isEnabled)
{
    dataLock.enter();
    midiThruEnabled = isEnabled;
    dataLock.exit();
    
}

void SphereData::setSphereMidiEnabled(const bool isEnabled)
{
    dataLock.enter();
    sphereMidiEnabled = isEnabled;
    dataLock.exit();
}
