//
//  PadData.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 15/07/2016.
//
//

#include "PadData.hpp"
#include "SphereData.hpp"

PadData::PadData(SphereData* _parent) : parent(_parent)
{
    masterReference.getSharedPointer(this);
    midiChannels.add(1);
    midiNotes.add(*new MidiNote);
}

PadData::~PadData()
{
    masterReference.clear();
}

ValueTree* PadData::toValueTree()
{
    ValueTree* outputTree = new ValueTree("PadData");
    ValueTree* midiNoteTree = new ValueTree("MidiNotes");

    dataLock.enter();
    
//    outputTree->setProperty("padID", padID.get(), 0);
//    outputTree->setProperty("padFunction", padFunction, 0);
//    outputTree->setProperty("velocity", velocity, 0);
//    outputTree->setProperty("padPressure", padPressure, 0);
//    outputTree->setProperty("pressureMode", pressureMode, 0);
//    outputTree->setProperty("sticky", sticky, 0);
//    outputTree->setProperty("noteEnabled", noteEnabled, 0);
//    outputTree->setProperty("pressureEnabled", pressureEnabled, 0);
//    outputTree->setProperty("quantiseEnabled", quantiseEnabled, 0);
//    outputTree->setProperty("reversePressure", reversePressure, 0);
//    outputTree->setProperty("velocityCurve", velocityCurve, 0);
//    outputTree->setProperty("pressureCurve", pressureCurve, 0);
//    outputTree->setProperty("padGroup", padGroup, 0);
//    
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


//SphereData* PadData::getParentSphere()
//{
//    return parent;
//}




bool PadData::setPadID (const int newPadID)
{
    bool success = false;
    if (newPadID >= 0)
    {
        padID.set(newPadID);
        
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
        padFunction.set(newFunction);
        
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
        padAudioFunction.set(newFunction);
        
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
        padMidiFunction.set(newFunction);;
        
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
        padSystemFunction.set(newFunction);
        
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
        noteTriggerMode.set(newMode);
        
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
        multiNoteMode.set(newMode);
        
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
        velocity.set(newVel);
        
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

    if (newPressure >= 0 && newPressure < 512)
    {
        padPressure.set(newPressure);

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
        midiChannels.set(0, newChannel);
        dataLock.exit();
        
        callListeners(DataIDs::MidiChannel, AppDataFormat::PadDataType);
        success = true;

    }
    else{
        jassertfalse; //value out of range
    }
    return success;
}

bool PadData::addMidChannel(const int newChannel)
{
    bool success = true;
    if (newChannel >= 1 && newChannel <= 16)
    {
        dataLock.enter();
        
        midiChannels.add(newChannel);
    
        dataLock.exit();
        callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
        
    }
    else{
        jassertfalse; //value out of range
        success = false;
    }
    
    return success;
}

bool PadData::removeMidiChannel(const int channelToRemove)
{
    bool success = false;
    dataLock.enter();
    
    for (int i = 0; i < midiChannels.size(); i++)
    {
        if (midiChannels[i] == channelToRemove)
        {
            midiChannels.remove(i);
            success = true;
        }
    }
    
    dataLock.exit();
    
    if (success)
        callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
    
    return success;
}

void PadData::clearAllMidiChannels()
{
    dataLock.enter();
    
    midiChannels.clearQuick();
    
    dataLock.exit();
    callListeners(DataIDs::MidiNotes, AppDataFormat::PadDataType);
}

bool PadData::setPressureMode(const int newMode)
{
    bool success = false;
    if (newMode >= 0 && newMode <= FINAL_PRESSUREMODE)
    {
        pressureMode.set(newMode);
        
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
        pressureDestination.set(newDestination);

        callListeners(DataIDs::PressureDestination, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    
    return success;
}

bool PadData::setPressureRange(const float newMin, const float newMax)
{
    bool success = false;
    if (newMin >= 0 && newMin <= 1.0)
    {
        if (newMax >= newMin && newMax >= 0 && newMax <= 1.0)
        {
            pressureMin.set(newMin);
            pressureRange.set(newMax-newMin);
            
            callListeners(DataIDs::PressureRange, AppDataFormat::PadDataType);
            success = true;
        }
    }
    
    return success;
}

void PadData::setSticky(const bool shouldBeSticky)
{
    sticky.set(shouldBeSticky);

    callListeners(DataIDs::Sticky, AppDataFormat::PadDataType);
}

void PadData::setNoteEnabled(const bool shouldBeEnabled)
{
    noteEnabled.set(shouldBeEnabled);
    
    callListeners(DataIDs::NoteEnabled, AppDataFormat::PadDataType);
    
}

void PadData::setPressureEnabled(const bool shouldBeEnabled)
{
    pressureEnabled.set(shouldBeEnabled);
    
    callListeners(DataIDs::PressureEnabled, AppDataFormat::PadDataType);
}


void PadData::setQuantiseEnabled(const bool shouldBeQuantised)
{
    quantiseEnabled.set(shouldBeQuantised);
    
    callListeners(DataIDs::QuantiseEnabled, AppDataFormat::PadDataType);
}

void PadData::setReversePressure(const bool shouldReversePressure)
{
    reversePressure.set(shouldReversePressure);
    
    callListeners(DataIDs::ReversePressure, AppDataFormat::PadDataType);
}
    
bool PadData::setVelocityCurve(const int newCurve)
{
    bool success = false;
    if (newCurve >= 0 && newCurve <= CurveTypes::StaticCurve)
    {
        pressureMode.set(newCurve);
        
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
        pressureMode.set(newCurve);

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
        padGroup.set(newGroup);

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
        midiDestination.set(newDestination);
        
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

bool PadData::setAllMidiNotes(Array<PadData::MidiNote> newNotes)
{
    int successfulAdds = 0;
    
    clearAllMidiNotes();
    
    for (int i = 0; newNotes.size(); ++i)
    {
        if (addMidiNote(newNotes[i].noteNumber, newNotes[i].velocityPercentage))
        {
            ++successfulAdds;
        }
    }
    
    if (successfulAdds == newNotes.size())
    {
        return true;
    }
    else
    {
        return false;
    }
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
        padColour.set(newColour);
        
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
        padColour.set(newType);
        
        callListeners(DataIDs::PadColour, AppDataFormat::PadDataType);
        success = true;
    }
    
    return success;
}

void PadData::setDynamicMidiChannel(const bool enabled)
{
    dynamicMidiChannel.set(enabled);
    
    callListeners(DataIDs::DynamicMidiChannel, AppDataFormat::PadDataType);
}

void PadData::setPadEnabled(const bool enabled)
{
    padEnabled.set(enabled);
    
    callListeners(DataIDs::DynamicMidiChannel, AppDataFormat::PadDataType);
}

bool PadData::setMidiCC(const int newCC)
{
    bool success = false;
    if (newCC >= 0 && newCC <= 128)
    {
        midiCC.set(newCC);

        callListeners(DataIDs::MidiCCType, AppDataFormat::PadDataType);
        success = true;
    }
    else{
        jassertfalse; //value out of range
    }
    return success;
    
}


//============= GETS ===========================================
int PadData::getPadID()
{
    return padID.get();
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

int PadData::getNumMidiNotes()
{
    return midiNotes.size();
}

int PadData::getPadFunction()
{
    return padFunction.get();
}

int PadData::getPadAudioFunction()
{
    return padAudioFunction.get();
}

int PadData::getPadMidiFunction()
{
    return padMidiFunction.get();
}

int PadData::getPadSystemFunction()
{
    return padSystemFunction.get();
}

int PadData::getNoteTriggerMode()
{
    return noteTriggerMode.get();
}

int PadData::getMultiNoteMode()
{
    return multiNoteMode.get();
}

int PadData::getVelocity()
{
    return velocity.get();
}

float PadData::getPadPressure()
{
    return padPressure.get();
}

int PadData::getMidiChannel()
{
    return midiChannels[0];
}

Array<int> PadData::getMidiChannels()
{
    return midiChannels;
}

int PadData::getNumMidiChannels()
{
    return midiChannels.size();
}

int PadData::getPressureMode()
{
    return pressureMode.get();
}
int PadData::getPressureDestination()
{
    return pressureDestination.get();
}

float PadData::getPressureMin()
{
    return pressureMin.get();
}

float PadData::getPressureRange()
{
    return pressureRange.get();
}

bool PadData::getSticky()
{
    return sticky.get();
}

bool PadData::getNoteEnabled()
{
    return noteEnabled.get();
}

bool PadData::getPressureEnabled()
{
    return pressureEnabled.get();
}

bool PadData::getQuantiseEnabled()
{
    return quantiseEnabled.get();
}

bool PadData::getReversePressure()
{
    return reversePressure.get();
}

int PadData::getVelocityCurve()
{
    return velocityCurve.get();
}

int PadData::getPressureCurve()
{
    return pressureCurve.get();
}

int PadData::getPadGroup()
{
    return padGroup.get();
}

int PadData::getMidiDestination()
{
    return midiDestination.get();
}

int PadData::getLFOCurveType()
{
    return lFOCurveType.get();
}

bool PadData::getDynamicMidiChannel()
{
    return dynamicMidiChannel.get();
}

bool PadData::getPadEnabled()
{
    return padEnabled.get();
}

int PadData::getParentSphereID()
{
    if (parent != nullptr)
    {
        return parent->getSphereID();
    }
    else
    {
        return -1;
    }
}

int PadData::getMidiCC()
{
    return midiCC.get();
}

