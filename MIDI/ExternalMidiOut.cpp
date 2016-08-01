//
//  ExternalMidiOut.cpp
//  AlphaSphere Midi Mapper
//
//  Created by Luke Saxton on 07/03/2016.
//
//

#include "ExternalMidiOut.hpp"

ExternalMidiOut::ExternalMidiOut(String name)
{
    channel = 1;
    
    while (midiOutput == NULL)
    {
        midiOutput = midiOutput->createNewDevice(name);
    }
    
    midiOutput->startBackgroundThread();
}

ExternalMidiOut::~ExternalMidiOut()
{
    midiOutput->stopBackgroundThread();
}

void ExternalMidiOut::handleInternalMidiNote(const int note, const int vel)
{
    //DBG("Output Note : " + String(note) + "," + String(vel));
    
    MidiMessage message = MidiMessage::noteOn(channel, note, uint8(vel));
    
    midiOutput->sendMessageNow(message);
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
    midiOutput->sendMessageNow(*message);
    
}
