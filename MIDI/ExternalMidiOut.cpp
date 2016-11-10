//
//  ExternalMidiOut.cpp
//  AlphaSphere Midi Mapper
//
//  Created by Luke Saxton on 07/03/2016.
//
//

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
