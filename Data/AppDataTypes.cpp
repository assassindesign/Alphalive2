//
//  AppDataTypes.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 01/08/2016.
//
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
    int moduloKey = newKey % BaseKey::B;
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
            dataLock.enter();
            key = abs(moduloKey);
            dataLock.exit();
            callListeners(DataIDs::Key, AppDataFormat::ScaleDataType);
            //jassertfalse; //value out of range
            //success = false;
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

