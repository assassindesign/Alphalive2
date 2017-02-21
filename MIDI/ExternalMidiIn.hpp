//
//  ExternalMidiIn.hpp
//  Alphalive Midi
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

#ifndef ExternalMidiIn_hpp
#define ExternalMidiIn_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"

/* 
    Simple class that registers all available midi devices as possible input to the software.
    needs expanding to support selecting certain devices only and to handle connects/disconnects
    during runtime.
 */

class ExternalMidiIn : public MidiInputCallback
{
public:
    ExternalMidiIn(AudioDeviceManager* globalDeviceManager);
    ~ExternalMidiIn();
    
    void setMidiInput (int index);
    
    
    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override;
    
    StringArray getMidiInputs();
    
private:
    
    double startTime;
    AudioDeviceManager* deviceManager;
    int currentMidiDevice;
    
    Alphalive2Engine* engine;
};

#endif /* ExternalMidiIn_hpp */
