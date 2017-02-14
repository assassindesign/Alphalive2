//
//  SphereData.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 15/07/2016.
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

#include "SphereData.hpp"
#include "bond.hpp"

#include <functional>


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
        callListeners(DataIDS::SphereID, AppDataFormat::SphereDataType);
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
        callListeners(DataIDS::SphereTypeData, AppDataFormat::SphereDataType);
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
        callListeners(DataIDS::RootNote, AppDataFormat::SphereDataType);
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
        callListeners(DataIDS::Octave, AppDataFormat::SphereDataType);
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
        callListeners(DataIDS::SphereID, AppDataFormat::SphereDataType);
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
        callListeners(DataIDS::RowConfigData, AppDataFormat::SphereDataType);
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

bool SphereData::editPadGroup(const int groupID, const PadData::DataIDs dataID, const var changedData)
{
#if JUCE_DEBUG
    if (Thread::getCurrentThreadId() != MessageManager::getInstance()->getCurrentMessageThread())
    {
        jassertfalse;
        // Naughty! this function should not used to access anything on a time critical thread.
        // Access the gets and sets directly.
    }
    
#endif
    
   // var function = var::NativeFunction(&PadData::setPressureEnabled);
    
    if (!changedData.isVoid())
    {
        Array<var> editingGroup;
        
        for (int i = 0; i < padData.size(); i++)
        {
            if (padData[i]->getPadGroup() == groupID)
            {
                editingGroup.add(padData[i]);
            }
        }
        
        //std::bind
        
//        bool success = false;
//        
//        if (changedData.isInt())
//        {
//            for (int i = 0; i < editingGroup.size(); i++)
//            {
//                switch (dataID) {
//                    case PadData::PadID:
//                        success = editingGroup[i]->setPadID(changedData);
//                        break;
//                    case PadData::PadFunction:
//                        success = editingGroup[i]->setPadFunction(changedData);
//                        break;
//                    case PadData::PadAudioFunction:
//                        success = editingGroup[i]->setPadAudioFunction(changedData);
//                        break;
//                    case PadData::PadMidiFunction:
//                        success = editingGroup[i]->setPadMidiFunction(changedData);
//                        break;
//                    case PadData::PadSystemFunction:
//                        success = editingGroup[i]->setPadSystemFunction(changedData);
//                        break;
//                    case PadData::NoteTriggerMode:
//                        success = editingGroup[i]->setNoteTriggerMode(changedData);
//                        break;
//                    case PadData::MultiNoteMode:
//                        success = editingGroup[i]->setNoteTriggerMode(changedData);
//                        break;
//                    case PadData::Velocity:
//                        success = editingGroup[i]->setVelocity(changedData);
//                        break;
//                  
//                    case PadData::MidiChannel:
//                        success = editingGroup[i]->setMidiChannel(changedData);
//                        break;
//                    case PadData::PressureMode:
//                        success = editingGroup[i]->setPressureMode(changedData);
//                        break;
//                    case PadData::PressureDestination:
//                        success = editingGroup[i]->setPressureDestination(changedData);
//                        break;
//                    default:
//                        break;
//                }
//                
//            }
//        }
//        else if (changedData.isBool())
//        {
//            for (int i = 0; i < editingGroup.size(); i++)
//            {
//                switch (dataID) {
//                    case PadData::Sticky:
//                        editingGroup[i]->setSticky(changedData);
//                        break;
//                    case PadData::NoteEnabled:
//                        editingGroup[i]->setNoteEnabled(changedData);
//                        break;
//                    case PadData::PressureEnabled:
//                        editingGroup[i]->setPressureEnabled(changedData);
//                        break;
//                    default:
//                        break;
//                }
//                
//            }
//        }
//        else if (changedData.isDouble())
//        {
//            for (int i = 0; i < editingGroup.size(); i++)
//            {
//                switch (dataID) {
//                    case PadData::Pressure:
//                        success = editingGroup[i]->setPadPressure(changedData);
//                        break;
//                    default:
//                        break;
//                }
//                
//            }
//        }
//        else if (changedData.isObject())
//        {
//            for (int i = 0; i < editingGroup.size(); i++)
//            {
//                switch (dataID) {
//                    case PadData::PadID:
//                        success = editingGroup[i]->setPadID(changedData);
//                        break;
//                    case PadData::MidiNotes:
//                        success = editingGroup[i]->setAllMidiNotes(Array<PadData::MidiNote>(changedData.getBinaryData()));
//                        break;
//                    case PadData::PadFunction:
//                        success = editingGroup[i]->setPadFunction(changedData);
//                        break;
//                    case PadData::PadAudioFunction:
//                        success = editingGroup[i]->setPadAudioFunction(changedData);
//                        break;
//                    case PadData::PadMidiFunction:
//                        success = editingGroup[i]->setPadMidiFunction(changedData);
//                        break;
//                    case PadData::PadSystemFunction:
//                        success = editingGroup[i]->setPadSystemFunction(changedData);
//                        break;
//                    case PadData::NoteTriggerMode:
//                        success = editingGroup[i]->setNoteTriggerMode(changedData);
//                        break;
//                    case PadData::MultiNoteMode:
//                        success = editingGroup[i]->setNoteTriggerMode(changedData);
//                        break;
//                    case PadData::Velocity:
//                        success = editingGroup[i]->setVelocity(changedData);
//                        break;
//                    case PadData::Pressure:
//                        success = editingGroup[i]->setPadPressure(changedData);
//                        break;
//                    case PadData::MidiChannel:
//                        success = editingGroup[i]->setMidiChannel(changedData);
//                        break;
//                    case PadData::PressureMode:
//                        success = editingGroup[i]->setPressureMode(changedData);
//                        break;
//                    case PadData::PressureDestination:
//                        success = editingGroup[i]->setPressureDestination(changedData);
//                        break;
//                    case PadData::Sticky:
//                        editingGroup[i]->setSticky(changedData);
//                        break;
//                    case PadData::NoteEnabled:
//                        editingGroup[i]->setNoteEnabled(changedData);
//                        break;
//                    case PadData::PressureEnabled:
//                        editingGroup[i]->setPressureEnabled(changedData);
//                        break;
//                    default:
//                        break;
//                }
//                
//            }
//        }
//        else
//        {
//            jassertfalse; // check changedData, what data type is it?
//        }
        

    }
    else
    {
        return false;
    }
    
    
    //editingGroup.call(&PadData::setVariable, dataID, changedData);
}

