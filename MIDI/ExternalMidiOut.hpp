//
//  ExternalMidiOut.hpp
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

#ifndef ExternalMidiOut_hpp
#define ExternalMidiOut_hpp

#include "InternalMidiRouter.hpp"
#include "AlphaSphereConnection.hpp"

/** 
    Super simple class to accept an internal midi note and output the note to an external device.
 */
class ExternalMidiOut : public InternalMidiIn
{
public:
    ExternalMidiOut(String name, AlphaSphereConnection* alphaSphere);
    
    ~ExternalMidiOut();
    
    void handleInternalMidiNote(const int note, const int vel) override;
    
    void setMidiChannel(const int newChannel);

    void handleMidiMessage(const MidiMessage* message) override;

    
private:
    ScopedPointer<MidiOutput> midiOutput;
    AlphaSphereConnection* sphereConnection;
    int channel = 1;
    bool outputThruSphere = false;
};

#endif /* ExternalMidiOut_hpp */
