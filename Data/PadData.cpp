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
    outputTree->setProperty("enabled", enabled, 0);
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
                else if (pName == "enabled")
                {
                    setEnabled(treeToImport->getProperty(pName));
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



//============= SETS ===========================================

bool PadData::setPadID (const int newPadID)
{
    bool success = false;
    if (newPadID >= 0)
    {
        dataLock.enter();
        padID = newPadID;
        dataLock.exit();
        
        callListeners(DataIDs::PadID);
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

        callListeners(DataIDs::MidiNotes);
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
        callListeners(DataIDs::MidiNotes);

    }
    else{
        jassertfalse; //value out of range
        success = false;
    }
    
    return success;
}

bool PadData::setPadFunction (const int newFunction)
{
    bool success = false;
    if (newFunction >= 0 && newFunction <= Controller)
    {
        dataLock.enter();
        padFunction = newFunction;
        dataLock.exit();
        
        callListeners(DataIDs::PadFunction);
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
    //callListeners(DataIDs::Velocity);
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
        
        callListeners(DataIDs::MidiChannel);
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
    if (newMode >= 0 && newMode <= Sequence)
    {
        dataLock.enter();
        pressureMode = newMode;
        dataLock.exit();
        callListeners(DataIDs::PressureMode);
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
    callListeners(DataIDs::Sticky);
    
}

void PadData::setEnabled(const bool shouldBeEnabled)
{
    dataLock.enter();
    enabled = shouldBeEnabled;
    dataLock.exit();
    callListeners(DataIDs::Enabled);
    
}
void PadData::setQuantiseEnabled(const bool shouldBeQuantised)
{
    dataLock.enter();
    quantiseEnabled = shouldBeQuantised;
    dataLock.exit();
    callListeners(DataIDs::QuantiseEnabled);
    
}
void PadData::setReversePressure(const bool shouldReversePressure)
{
    dataLock.enter();
    reversePressure = shouldReversePressure;
    dataLock.exit();
    callListeners(DataIDs::ReversePressure);
    
}
    
bool PadData::setVelocityCurve(const int newCurve)
{
    bool success = false;
    if (newCurve >= 0 && newCurve <= CurveType::StaticCurve)
    {
        dataLock.enter();
        pressureMode = newCurve;
        dataLock.exit();
        callListeners(DataIDs::VelocityCurve);
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
    if (newCurve >= 0 && newCurve <= CurveType::Logarithmic)
    {
        dataLock.enter();
        pressureMode = newCurve;
        dataLock.exit();
        callListeners(DataIDs::PressureCurve);
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
        callListeners(DataIDs::PadGroup);
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
        callListeners(DataIDs::MidiDestination);
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
    callListeners(DataIDs::MidiNotes);
    
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
        callListeners(DataIDs::MidiNotes);
        
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
        callListeners(DataIDs::MidiNotes);
    
    return success;
}

void PadData::clearAllMidiNotes()
{
    dataLock.enter();
    
    midiNotes.clear();
    
    dataLock.exit();
    callListeners(DataIDs::MidiNotes);
}

bool PadData::setPadColour(const int newColour)
{
    bool success = false;
    
    if (newColour > -1 && newColour < NUM_PAD_COLOURS)
    {
        dataLock.enter();
        padColour = newColour;
        dataLock.exit();
        
        callListeners(DataIDs::PadColour);
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

bool PadData::getSticky()
{
    return sticky;
}

bool PadData::getEnabled()
{
    return enabled;
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



