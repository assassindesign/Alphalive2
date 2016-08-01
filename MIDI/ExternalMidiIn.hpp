//
//  ExternalMidiIn.hpp
//  Alphalive Midi
//
//  Created by Luke Saxton on 23/02/2016.
//
//

#ifndef ExternalMidiIn_hpp
#define ExternalMidiIn_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"


class ExternalMidiIn : public MidiInputCallback
{
public:
    ExternalMidiIn(AudioDeviceManager* globalDeviceManager);
    ~ExternalMidiIn();
    
    void setMidiInput (int index);
    
    
    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override;
    
private:
    
    double startTime;
    AudioDeviceManager* deviceManager;
    int currentMidiDevice;
    
    Alphalive2Engine* engine;
};

#endif /* ExternalMidiIn_hpp */
