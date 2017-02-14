//
//  ExternalMidiOut.cpp
//  AlphaSphere Midi Mapper
//
//  Created by Luke Saxton on 07/03/2016.
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

#include "ExternalMidiOut.hpp"

ExternalMidiOut::ExternalMidiOut(String name, AlphaSphereConnection* alphaSphere = nullptr)
{
    while (midiOutput == NULL)
    {
        midiOutput = midiOutput->createNewDevice(name);
    }
    
    midiOutput->startBackgroundThread();
    
    sphereConnection = alphaSphere;
    
    if (sphereConnection != nullptr)
    {
        outputThruSphere = true;
    }
}

ExternalMidiOut::~ExternalMidiOut()
{
    midiOutput->stopBackgroundThread();
}

void ExternalMidiOut::handleInternalMidiNote(const int note, const int vel)
{
    //DBG("Output Note : " + String(note) + "," + String(vel));
    
    MidiMessage message = MidiMessage::noteOn(channel, note, uint8(vel));
    handleMidiMessage(&message);
    
}

void ExternalMidiOut::setMidiChannel(const int newChannel)
{
    if (newChannel > 0 && newChannel <= 16)
    {
        channel = newChannel;

    }
}

void ExternalMidiOut::handleMidiMessage(const MidiMessage* message)
{
    
    if (outputThruSphere)
    {
        sphereConnection->sendMidiMessage(*message);
    }
    else
    {
        midiOutput->sendMessageNow(*message);
    }
}
