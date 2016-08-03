//
//  AppDataTypes.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 01/08/2016.
//
//

#include "AppDataTypes.h"
//========================================================================================
//      ScaleData
//========================================================================================
#include "SphereData.hpp"

//==GETS=======================================================

const int ScaleData::getKey()
{
    return key;
}

const int ScaleData::getOctave()
{
    return octave;
}

const int ScaleData::getScale()
{
    return scale;
}

//==SETS=======================================================

bool ScaleData::setKey(const int newKey)
{
    bool success = true;
    if (newKey != key)
    {
        if (newKey > -1 && newKey < BaseKey::B)
        {
            dataLock.enter();
            key = newKey;
            dataLock.exit();
            callListeners(DataIDs::Key, AppDataFormat::ScaleDataType);
        }
        else
        {
            //jassertfalse; //value out of range
            success = false;
        }
        
    }
    return success;
}

bool ScaleData::setOctave(const int newOctave)
{
    bool success = true;
    dataLock.enter();
    if (newOctave != octave)
    {
        if (newOctave > -3 && newOctave < 6)
        {
            octave = newOctave;
            callListeners(DataIDs::Key, AppDataFormat::ScaleDataType);
        }
        else
        {
            //jassertfalse; //value out of range
            success = false;

        }
        
    }
    
    dataLock.exit();
    
    return success;
}

bool ScaleData::setScale(const int newScale)
{
    bool success = true;
    if (newScale > -1 && newScale < SphereData::FINAL_SCALE)
    {
        dataLock.enter();
        scale = newScale;
        callListeners(DataIDs::Scale, AppDataFormat::ScaleDataType);
        dataLock.exit();
    }
    else
    {
        //jassertfalse; //value out of range
        success = false;
    }
    
    
    return success;
}

//========================================================================================
//      TempoData
//========================================================================================

//==GETS=======================================================
void getTempo()
{
    
}

void getBeatsPerBar()
{
    
}


//==SETS=======================================================
bool setTempo(const float newTempo);
bool setBeatsPerBar(const int newBPB);

