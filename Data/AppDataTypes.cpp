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
    dataLock.enter();
    if (newKey != key)
    {
        if (newKey > -1 && newKey < BaseKey::B)
            key = newKey;
        else
            success = false;
        
    }
    else{
        success = false;
    }
    dataLock.exit();
    
    if(success)
        callListeners(DataIDs::Key);
    
    return success;
}

bool ScaleData::setOctave(const int newOctave)
{
    bool success = true;
    dataLock.enter();
    if (newOctave != octave)
    {
        if (newOctave > -3 && newOctave < 6)
            octave = newOctave;
        else
            success = false;
        
    }
    else
    {
        success = false;
    }
    
    dataLock.exit();
    
    if (success)
        callListeners(DataIDs::Key);
    
    return success;
}

bool ScaleData::setScale(const int newScale)
{
    bool success = true;
    dataLock.enter();
    if (newScale != scale)
    {
        scale = newScale;
    }
    else
    {
        jassertfalse; //value out of range
        success = false;
    }
    dataLock.exit();
    
    if (success)
        callListeners(DataIDs::Key);
    
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

