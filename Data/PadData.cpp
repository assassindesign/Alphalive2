//
//  PadData.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 15/07/2016.
//
//

#include "PadData.hpp"

PadData::PadData(SphereData* _parent) : parent(_parent)
{
    midiNotes.add(*new MidiNote());
}

PadData::~PadData()
{
    
}

ValueTree* PadData::toValueTree()
{
    ValueTree* outputTree = new ValueTree("PadData");
    ValueTree* midiNoteTree = new ValueTree("MidiNotes");

    dataLock.enter();
    
    outputTree->setProperty("padID", padID, 0);
    outputTree->setProperty("padFunction", padFunction, 0);
    outputTree->setProperty("velocity", velocity, 0);
    outputTree->setProperty("padPressure", padPressure, 0);
    outputTree->setProperty("pressureMode", pressureMode, 0);
    outputTree->setProperty("sticky", sticky, 0);
    outputTree->setProperty("noteEnabled", noteEnabled, 0);
    outputTree->setProperty("pressureEnabled", pressureEnabled, 0);
    outputTree->setProperty("quantiseEnabled", quantiseEnabled, 0);
    outputTree->setProperty("reversePressure", reversePressure, 0);
    outputTree->setProperty("velocityCurve", velocityCurve, 0);
    outputTree->setProperty("pressureCurve", pressureCurve, 0);
    outputTree->setProperty("padGroup", padGroup, 0);
    
    for (int i = 0; i < midiNoteTree->getNumChildren(); i++)
    {
        midiNoteTree->setProperty("MidiNote" + String(i), midiNotes[i].noteNumber, 0);
        midiNoteTree->setProperty("VelocityPercentage" + String(i), midiNotes[i].velocityPercentage, 0);

    }
    outputTree->addChild(*midiNoteTree, -1, 0);
    dataLock.exit();
    
    return outputTree;
}

bool PadData::fromValueTree(ValueTree* treeToImport)
{
    if (treeToImport != nullptr)
    {
        if (treeToImport->getType().toString() == "PadData")
        {
            String pName;
            for (int i = 0; i < treeToImport->getNumProperties(); i++)
            {
                pName = treeToImport->getPropertyName(i).toString();
                
                if (pName == "padID")
                {
                    setPadID(treeToImport->getProperty(pName));
                }
                else if (pName == "padFunction")
                {
                    setPadFunction(treeToImport->getProperty(pName));
                }
                else if (pName == "velocity")
                {
                    setVelocity(treeToImport->getProperty(pName));
                }
                else if (pName == "padPressure")
                {
                    setPadPressure(treeToImport->getProperty(pName));
                }
                else if (pName == "sticky")
                {
                    setSticky(treeToImport->getProperty(pName));
                }
                else if (pName == "noteEnabled")
                {
                    setNoteEnabled(treeToImport->getProperty(pName));
                }
                else if (pName == "pressureEnabled")
                {
                    setPressureEnabled(treeToImport->getProperty(pName));
                }
                else if (pName == "quantiseEnabled")
                {
                    setQuantiseEnabled(treeToImport->getProperty(pName));
                }
                else if (pName == "reversePressure")
                {
                    setReversePressure(treeToImport->getProperty(pName));
                }
                else if (pName == "velocityCurve")
                {
                    setVelocity(treeToImport->getProperty(pName));
                }
                else if (pName == "pressureCurve")
                {
                    setPressureCurve(treeToImport->getProperty(pName));
                }
                else if (pName == "padGroup")
                {
                    setPadGroup(treeToImport->getProperty(pName));
                }
            
            }
            
            for (int i = 0; i < treeToImport->getNumChildren(); i++)
            {
                ValueTree midiNoteTree = treeToImport->getChild(i);
                if (midiNoteTree.getType().toString() == "MidiNotes")
                {
                    midiNotes.clearQuick();
                    for (int i = 0; i < midiNoteTree.getNumProperties(); i++)
                    {
                        pName = midiNoteTree.getPropertyName(i).toString();

                        if (pName.contains("MidiNote"))
                        {
                            addMidiNote(midiNoteTree.getProperty(pName));
                        }
                    }
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else{
        return false;
    }
}


SphereData* PadData::getParentSphere()
{
    return parent;
}

//============= SETS ===========================================

bool PadData::setPadID (const int newPadID)
{
    bool success = false;
    if (newPadID >= 0)
    {
        dataLock.enter();
        padID = newPadID;
        dataLock.exit();
        
        callListeners(DataIDs::PadID, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    
    return success;
    
}

bool PadData::setMidiNote (const int newNote)
{
    bool success = false;
    if (newNote >= 0 && newNote < 128)
    {
        dataLock.enter();
        if (midiNotes.size() == 0)
        {
            midiNotes.add(*new MidiNote());
        }
        midiNotes.getReference(0).noteNumber = newNote;
        dataLock.exit();

        callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        //jassertfalse; //value out of range
    }
    
    return success;
}

bool PadData::setMidiNote (const int newNote, const int velPercentage)
{
    bool success = false;
    if (newNote >= 0 && newNote < 128)
    {
        dataLock.enter();
        if (midiNotes.size() == 0)
        {
            midiNotes.add(*new MidiNote());
            success = true;
        }
        
        midiNotes.getReference(0).noteNumber = newNote;
        
        if (velPercentage > -1 && velPercentage < 101)
        {
            midiNotes.getReference(0).velocityPercentage = velPercentage;
        }
        else
        {
            jassertfalse; //percentages are between 0-100...
            success = false;
        }
        
        dataLock.exit();
        callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);

    }
    else{
        jassertfalse; //value out of range
        success = false;
    }
    
    return success;
}

bool PadData::setMidiNote (const int arrayIndex, const int newNote, const int velPercentage = 100)
{
    bool success = false;
    if (arrayIndex > -1 && arrayIndex < midiNotes.size()) //if array index is in range
    {
        if (newNote >= 0 && newNote < 128) //and note value is in range
        {
            dataLock.enter();
            
            midiNotes.getReference(arrayIndex).noteNumber = newNote;
            success = true;
            
            if (velPercentage > -1 && velPercentage < 101)
            {
                midiNotes.getReference(arrayIndex).velocityPercentage = velPercentage;
            }
            else
            {
                jassertfalse; //percentages are between 0-100...
                success = false;
            }
            
            dataLock.exit();
            callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
            
        }
        else{
            jassertfalse; //value out of range
            success = false;
        }
    }
    
    return success;
}

bool PadData::setVelPercentForNote(const int noteToFind, const int newVelPercentage)
{
    bool success = false;
    
    if (newVelPercentage > -1 && newVelPercentage < 101)
    {
        if (noteToFind > -1 && noteToFind < 128)
        {
            dataLock.enter();
            for (int i = 0 ; i < midiNotes.size(); i++)
            {
                if (midiNotes[i].noteNumber == noteToFind)
                {
                    midiNotes.getReference(i).velocityPercentage = newVelPercentage;
                    success = true;
                    break;
                }
            }
            
            dataLock.exit();
            if (success)
            {
                callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
            }
        }
       
    }
    
    return success;
}

bool PadData::setPadFunction (const int newFunction)
{
    bool success = false;
    if (newFunction >= 0 && newFunction <= FINAL_PADFUNCTION)
    {
        dataLock.enter();
        padFunction = newFunction;
        dataLock.exit();
        
        callListeners(DataIDs::PadFunction, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
}

bool PadData::setPadAudioFunction (const int newFunction)
{
    bool success = false;
    if (newFunction >= 0 && newFunction <= FINAL_PADAUDIOFUNCTION)
    {
        dataLock.enter();
        padAudioFunction = newFunction;
        dataLock.exit();
        
        callListeners(DataIDs::PadAudioFunction, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
}

bool PadData::setPadMidiFunction (const int newFunction)
{
    bool success = false;
    if (newFunction >= 0 && newFunction <= FINAL_PADMIDIFUNCTION)
    {
        dataLock.enter();
        padMidiFunction = newFunction;
        dataLock.exit();
        
        callListeners(DataIDs::PadMidiFunction, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
}

bool PadData::setPadSystemFunction (const int newFunction)
{
    bool success = false;
    if (newFunction >= 0 && newFunction <= FINAL_PADSYSFUNCTION)
    {
        dataLock.enter();
        padSystemFunction = newFunction;
        dataLock.exit();
        
        callListeners(DataIDs::PadSystemFunction, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
}

bool PadData::setNoteTriggerMode(const int newMode)
{
    bool success = false;
    if (newMode >= 0 && newMode <= FINAL_SINGLENOTEMODE)
    {
        dataLock.enter();
        noteTriggerMode = newMode;
        dataLock.exit();
        
        callListeners(DataIDs::NoteTriggerMode, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
}

bool PadData::setMultiNoteMode(const int newMode)
{
    bool success = false;
    if (newMode >= 0 && newMode <= FINAL_MULTINOTEMODE)
    {
        dataLock.enter();
        multiNoteMode = newMode;
        dataLock.exit();
        
        callListeners(DataIDs::MultiNoteMode, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
}

bool PadData::setVelocity (const int newVel)
{
    bool success = false;
    if (newVel >= 0 && newVel < 128)
    {
        dataLock.enter();
        velocity = newVel;
        dataLock.exit();
        
        callRepaintListeners();
        success = true;
        
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
}

bool PadData::setPadPressure (const float newPressure)
{
    bool succeed = false;

    if (newPressure >= 0 && newPressure < 128)
    {
        dataLock.enter();
        padPressure = newPressure;
        dataLock.exit();
        //callListeners(DataIDs::Pressure);
        callRepaintListeners();
        succeed = true;
    }
    else{
        jassertfalse; //value out of range
    }
    
    return succeed;

    
}

bool PadData::setMidiChannel (const int newChannel)
{
    bool success = false;
    if (newChannel >= 1 && newChannel <= 16)
    {
        dataLock.enter();
        midiChannel = newChannel;
        dataLock.exit();
        
        callListeners(DataIDs::MidiChannel, AppDataFormat::PadDataType);
        success = true;

    }
    else{
        jassertfalse; //value out of range
    }
    return success;
}

bool PadData::setPressureMode(const int newMode)
{
    bool success = false;
    if (newMode >= 0 && newMode <= FINAL_PRESSUREMODE)
    {
        dataLock.enter();
        pressureMode = newMode;
        dataLock.exit();
        callListeners(DataIDs::PressureMode, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }

    return success;
}

bool PadData::setPressureDestination(const int newDestination)
{
    bool success = false;
    if (newDestination >= 0 && newDestination <= FINAL_PRESSUREDESTINATION)
    {
        dataLock.enter();
        pressureDestination = newDestination;
        dataLock.exit();
        callListeners(DataIDs::PressureDestination, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    
    return success;
}

void PadData::setSticky(const bool shouldBeSticky)
{
    dataLock.enter();
    sticky = shouldBeSticky;
    dataLock.exit();
    callListeners(DataIDs::Sticky, AppDataFormat::PadDataType);
    
}

void PadData::setNoteEnabled(const bool shouldBeEnabled)
{
    dataLock.enter();
    noteEnabled = shouldBeEnabled;
    dataLock.exit();
    callListeners(DataIDs::NoteEnabled, AppDataFormat::PadDataType);
    
}

void PadData::setPressureEnabled(const bool shouldBeEnabled)
{
    dataLock.enter();
    pressureEnabled = shouldBeEnabled;
    dataLock.exit();
    callListeners(DataIDs::PressureEnabled, AppDataFormat::PadDataType);
    
}

void PadData::setQuantiseEnabled(const bool shouldBeQuantised)
{
    dataLock.enter();
    quantiseEnabled = shouldBeQuantised;
    dataLock.exit();
    callListeners(DataIDs::QuantiseEnabled, AppDataFormat::PadDataType);
    
}
void PadData::setReversePressure(const bool shouldReversePressure)
{
    dataLock.enter();
    reversePressure = shouldReversePressure;
    dataLock.exit();
    callListeners(DataIDs::ReversePressure, AppDataFormat::PadDataType);
    
}
    
bool PadData::setVelocityCurve(const int newCurve)
{
    bool success = false;
    if (newCurve >= 0 && newCurve <= CurveTypes::StaticCurve)
    {
        dataLock.enter();
        pressureMode = newCurve;
        dataLock.exit();
        callListeners(DataIDs::VelocityCurve, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
    
}

bool PadData::setPressureCurve(const int newCurve)
{
    bool success = false;
    if (newCurve >= 0 && newCurve <= FINAL_CURVETYPE)
    {
        dataLock.enter();
        pressureMode = newCurve;
        dataLock.exit();
        callListeners(DataIDs::PressureCurve, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
    
}
bool PadData::setPadGroup(const int newGroup)
{
    bool success = false;
    if (newGroup >= 0 && newGroup <= MAX_GROUPS)
    {
        dataLock.enter();
        pressureMode = newGroup;
        dataLock.exit();
        callListeners(DataIDs::PadGroup, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
    
}

bool PadData::setMidiDestination(const int newDestination)
{
    bool success = false;
    
    if (newDestination >= 0)
    {
        dataLock.enter();
        midiDestination = newDestination;
        dataLock.exit();
        callListeners(DataIDs::MidiDestination, AppDataFormat::PadDataType);
        success = true;
    }
    else
    {
        jassertfalse; //value out of range
    }
    return success;
}


bool PadData::addMidiNote(const int newNote)
{
    bool success;
    dataLock.enter();
    if (newNote >= 0 && newNote < 128)
    {
        midiNotes.add(*new MidiNote());
        midiNotes.getReference(midiNotes.size()-1).noteNumber = newNote;
    }
    else{
        jassertfalse; //value out of range
        success = false;
    }
    dataLock.exit();
    callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
    
    return success;
}

bool PadData::addMidiNote(const int newNote, const int velPercentage)
{
    bool success = true;
    if (newNote >= 0 && newNote < 128)
    {
        dataLock.enter();

        midiNotes.add(*new MidiNote());
        midiNotes.getReference(midiNotes.size()-1).noteNumber = newNote;
        
        if (velPercentage > -1 && velPercentage < 101)
        {
            midiNotes.getReference(midiNotes.size()-1).velocityPercentage = velPercentage;
        }
        else
        {
            success = false;
        }
        dataLock.exit();
        callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
        
    }
    else{
        jassertfalse; //value out of range
        success = false;
    }

    
    return success;
}

bool PadData::removeMidiNote(const int noteToRemove)
{
    bool success = false;
    dataLock.enter();
    
    for (int i = 0; i < midiNotes.size(); i++)
    {
        if (midiNotes[i].noteNumber == noteToRemove)
        {
            midiNotes.remove(i);
            success = true;
        }
    }
    
    dataLock.exit();
    
    if (success)
        callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
    
    return success;
}

void PadData::clearAllMidiNotes()
{
    dataLock.enter();
    
    midiNotes.clearQuick();
    
    dataLock.exit();
    callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
}

bool PadData::setPadColour(const int newColour)
{
    bool success = false;
    
    if (newColour > -1 && newColour < NUM_PAD_COLOURS)
    {
        dataLock.enter();
        padColour = newColour;
        dataLock.exit();
        
        callListeners(DataIDs::PadColour, AppDataFormat::PadDataType);
        success = true;
    }

    return success;
}

bool PadData::setLFOCurveType(const int newType)
{
    bool success = false;
    
    if (newType > -1 && newType < FINAL_LFOCURVETYPE)
    {
        dataLock.enter();
        padColour = newType;
        dataLock.exit();
        
        callListeners(DataIDs::PadColour, AppDataFormat::PadDataType);
        success = true;
    }
    
    return success;
}

//============= GETS ===========================================
int PadData::getPadID()
{
    return padID;
}

int PadData::getMidiNote()
{
    if (midiNotes.size() > 0)
    {
        return midiNotes[0].noteNumber;
    }
    else
    {
        return -1;
    }
}

Array<PadData::MidiNote> PadData::getMidiNotes()
{
    return midiNotes;
}


int PadData::getPadFunction()
{
    return padFunction;
}

int PadData::getPadAudioFunction()
{
    return padAudioFunction;
}

int PadData::getPadMidiFunction()
{
    return padMidiFunction;
}

int PadData::getPadSystemFunction()
{
    return padSystemFunction;
}

int PadData::getNoteTriggerMode()
{
    return noteTriggerMode;
}

int PadData::getMultiNoteMode()
{
    return multiNoteMode;
}

int PadData::getVelocity()
{
    return velocity;
}

float PadData::getPadPressure()
{
    return padPressure;
}

int PadData::getMidiChannel()
{
    return midiChannel;
}

int PadData::getPressureMode()
{
    return pressureMode;
}
int PadData::getPressureDestination()
{
    return pressureDestination;
}

bool PadData::getSticky()
{
    return sticky;
}

bool PadData::getNoteEnabled()
{
    return noteEnabled;
}

bool PadData::getPressureEnabled()
{
    return pressureEnabled;
}

bool PadData::getQuantiseEnabled()
{
    return quantiseEnabled;
}

bool PadData::getReversePressure()
{
    return reversePressure;
}

int PadData::getVelocityCurve()
{
    return velocityCurve;
}

int PadData::getPressureCurve()
{
    return pressureCurve;
}

int PadData::getPadGroup()
{
    return padGroup;
}

int PadData::getMidiDestination()
{
    return midiDestination;
}

int PadData::getLFOCurveType()
{
    return lFOCurveType;
}



