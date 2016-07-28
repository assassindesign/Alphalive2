//
//  ExternalMidiIn.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 23/02/2016.
//
//

#include "ExternalMidiIn.hpp"
#include "MidiSequencerEngine.hpp"

ExternalMidiIn::ExternalMidiIn(AudioDeviceManager* globalDeviceManager)
    : startTime (Time::getMillisecondCounterHiRes() * 0.001), deviceManager(globalDeviceManager), currentMidiDevice(0)
{
    
    const StringArray midiInputs (MidiInput::getDevices());
    //midiInputList.addItemList (midiInputs, 1);
    //midiInputList.addListener (this);
    

    
    // find the first enabled device and use that by default
    for (int i = 0; i < midiInputs.size(); ++i)
    {
        //if (deviceManager->isMidiInputEnabled (midiInputs[i]))
        //{
            setMidiInput (i);
            //break;
        //}
        //else
        //{
        //    DBG("No Midi Inputs Available");
        //}
    }

    engine = AppData::Instance()->getEnginePointer();
}

ExternalMidiIn::~ExternalMidiIn()
{
    
}

/** Starts listening to a MIDI input device, enabling it if necessary. */
void ExternalMidiIn::setMidiInput (int index)
{
    const StringArray list (MidiInput::getDevices());
    
    //deviceManager->removeMidiInputCallback (list[currentMidiDevice], this);
    
    const String newInput (list[index]);
    
    if (! deviceManager->isMidiInputEnabled (newInput))
        deviceManager->setMidiInputEnabled (newInput, true);
    
    deviceManager->addMidiInputCallback (newInput, this);
    //midiInputList->setSelectedId (index + 1, dontSendNotification);
    
    currentMidiDevice = index;
}

// These methods handle callbacks from the midi device + on-screen keyboard..
void ExternalMidiIn::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    engine->midiThru(message);

}
