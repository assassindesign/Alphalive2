//
//  ExternalMidiIn.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 23/02/2016.
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

#include "ExternalMidiIn.hpp"
#include "AlphaLive2Engine.hpp"

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
            //setMidiInput (i);
            //break;
        //}
        //else
        //{
        //    DBG("No Midi Inputs Available");
        //}
        
        if (!midiInputs[i].contains("Alphalive"))
        {
            setMidiInput (i);

        }
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

StringArray ExternalMidiIn::getMidiInputs()
{
    return MidiInput::getDevices();
}

