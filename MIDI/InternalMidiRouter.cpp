//
//  InternalMidiRouter.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
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

#include "InternalMidiRouter.hpp"
#include "bond.hpp"

InternalMidiRouter::InternalMidiRouter(InternalMidiIn* midiOut) : keyboardState(new MidiKeyboardState())
{
    
    destinations.add(midiOut);
    //destinations.add(piano);

    
    addActionListener(this);
    
    midiNoteQueue = new ThreadQueue();
    midiNoteQueue->open();
}

InternalMidiRouter::~InternalMidiRouter()
{
    midiNoteQueue->close();
    midiNoteQueue = nullptr;
}

MidiKeyboardState* InternalMidiRouter::getKeyboardStateObject()
{
    return keyboardState;
}


void InternalMidiRouter::handleInternalMidiNoteWithDestination(MidiDestination dest, const int note, const int vel)
{
    //DBG("RouteMidi: " + String(dest) + ":" + String(note) + ":" + String(vel));
   
    
    switch (dest) {
        case MidiOut:
            destinations[0]->handleInternalMidiNote(note, vel);
            break;
        //case Piano:
            //destinations[1]->handleInternalMidiNote(note, vel);
         //   break;
        default:
            break;
    }
    
    midiNoteQueue->call(bond(&InternalMidiRouter::updateKeyboardState, this, note, vel));
    sendActionMessage("Q");
}

void InternalMidiRouter::sendMidiToDestination (const int dest, const MidiMessage* message)
{
    switch (dest) {
        case MidiOut:
            destinations[MidiOut]->handleMidiMessage(message);
            break;
        default:
            break;
    }
    
}

 void InternalMidiRouter::actionListenerCallback (const String& message)
{
    midiNoteQueue->process();
}

void InternalMidiRouter::updateKeyboardState(const int note, const int vel)
{
    if (vel > 0)
    {
        keyboardState->noteOn(1, note, vel);
    }
    else
    {
        keyboardState->noteOff(1, note, vel);
    }
}

void InternalMidiRouter::panic(MidiDestination destination)
{
    if (destination < destinations.size())
    {
        if (destinations.getUnchecked(destination) != 0)
        {
            for (int i = 0; i < 128; i++)
            {
                destinations[destination]->handleInternalMidiNote(i, 0);
                
            }
            
        }

    }
}

