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

void PadData::setPadID (const int newPadID)
{
    dataLock.enter();
    if (newPadID >= 0)
    {
        padID = newPadID;
    }
    else{
        jassertfalse; //value out of range
    }
    dataLock.exit();
    callListeners(DataIDs::PadID);
    
}

bool PadData::setMidiNote (const int newNote)
{
    bool success = true;
    dataLock.enter();
    if (newNote >= 0 && newNote < 128)
    {
        if (midiNotes.size() == 0)
        {
            midiNotes.add(*new MidiNote());
        }
        midiNotes.getReference(0).noteNumber = newNote;

    }
    else{
        jassertfalse; //value out of range
        success = false;
    }
    dataLock.exit();
    callListeners(DataIDs::MidiNotes);
    
    return success;
}

bool PadData::setMidiNote (const int newNote, const int velPercentage)
{
    bool success;
    dataLock.enter();
    if (newNote >= 0 && newNote < 128)
    {
        if (midiNotes.size() == 0)
        {
            midiNotes.add(*new MidiNote());
        }
        else
        {
            success = false;
        }
        
        midiNotes.getReference(0).noteNumber = newNote;
        
        if (velPercentage > -1 && velPercentage < 101)
        {
            midiNotes.getReference(0).velocityPercentage = velPercentage;
        }
        else
        {
            success = false;
        }
        

    }
    else{
        jassertfalse; //value out of range
        success = false;
    }
    dataLock.exit();
    callListeners(DataIDs::MidiNotes);
    
    return success;
}

void PadData::setPadFunction (const int newFunction)
{
    dataLock.enter();
    if (newFunction >= 0 && newFunction <= Controller)
    {
        padFunction = newFunction;
    }
    else{
        jassertfalse; //value out of range
    }
    dataLock.exit();
    callListeners(DataIDs::PadFunction);
    
}

bool PadData::setVelocity (const int newVel)
{
    bool succeed = true;
    dataLock.enter();
    if (newVel >= 0 && newVel < 128)
    {
        velocity = newVel;
    }
    else{
        //jassertfalse; //value out of range
        succeed = false;
    }
    dataLock.exit();
    //callListeners(DataIDs::Velocity);
    callRepaintListeners();
    return succeed;
}

bool PadData::setPadPressure (const float newPressure)
{
    bool succeed = true;

    dataLock.enter();
    if (newPressure >= 0 && newPressure < 128)
    {
        padPressure = newPressure;
    }
    else{
        jassertfalse; //value out of range
        succeed = false;
    }
    dataLock.exit();
    //callListeners(DataIDs::Pressure);
    callRepaintListeners();
    return succeed;

    
}

void PadData::setMidiChannel (const int newChannel)
{
    dataLock.enter();
    if (newChannel >= 1 && newChannel <= 16)
    {
        midiChannel = newChannel;
    }
    else{
        jassertfalse; //value out of range
    }
    dataLock.exit();
    callListeners(DataIDs::MidiChannel);
    
}

void PadData::setPressureMode(const int newMode)
{
    dataLock.enter();
    if (newMode >= 0 && newMode <= Sequence)
    {
        pressureMode = newMode;
    }
    else{
        jassertfalse; //value out of range
    }
    dataLock.exit();
    callListeners(DataIDs::PressureMode);
    
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
    
void PadData::setVelocityCurve(const int newCurve)
{
    dataLock.enter();
    if (newCurve >= 0 && newCurve <= CurveType::StaticCurve)
    {
        pressureMode = newCurve;
    }
    else{
        jassertfalse; //value out of range
    }
    dataLock.exit();
    callListeners(DataIDs::VelocityCurve);
    
}

void PadData::setPressureCurve(const int newCurve){
    dataLock.enter();
    if (newCurve >= 0 && newCurve <= CurveType::Logarithmic)
    {
        pressureMode = newCurve;
    }
    else{
        jassertfalse; //value out of range
    }
    dataLock.exit();
    callListeners(DataIDs::PressureCurve);
    
}
void PadData::setPadGroup(const int newGroup){
    dataLock.enter();
    if (newGroup >= 0 && newGroup <= MAX_GROUPS)
    {
        pressureMode = newGroup;
    }
    else{
        jassertfalse; //value out of range
    }
    dataLock.exit();
    callListeners(DataIDs::PadGroup);
    
}

void PadData::setMidiDestination(const int newDestination)
{
    dataLock.enter();
    
    pressureMode = newDestination;
    
    dataLock.exit();
    callListeners(DataIDs::MidiDestination);
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
    bool success;
    dataLock.enter();
    if (newNote >= 0 && newNote < 128)
    {
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
        
    }
    else{
        jassertfalse; //value out of range
        success = false;
    }
    dataLock.exit();
    callListeners(DataIDs::MidiNotes);
    
    return success;
}

void PadData::removeMidiNote(const int noteToRemove)
{
    dataLock.enter();
    
    for (int i = 0; i < midiNotes.size(); i++)
    {
        if (midiNotes[i].noteNumber == noteToRemove)
        {
            midiNotes.remove(i);
        }
    }
    
    dataLock.exit();
    callListeners(DataIDs::MidiNotes);
}

void PadData::clearAllMidiNotes()
{
    dataLock.enter();
    
    midiNotes.clear();
    
    dataLock.exit();
    callListeners(DataIDs::MidiNotes);
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



