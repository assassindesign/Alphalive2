//
//  ExternalMidiOut.hpp
//  AlphaSphere Midi Mapper
//
//  Created by Luke Saxton on 07/03/2016.
//
//

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
