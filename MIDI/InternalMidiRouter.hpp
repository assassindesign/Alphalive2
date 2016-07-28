//
//  InternalMidiRouter.hpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
//
//

#ifndef InternalMidiRouter_hpp
#define InternalMidiRouter_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "ThreadQueue.hpp"




/**
    Abstract class to be inherited by any class which wants to be a destination for the InternalMidiRouter
 
    @see InternalMidiRouter
 */
class InternalMidiIn
{
public:
    InternalMidiIn() {}
    virtual ~InternalMidiIn() {};
    virtual void handleInternalMidiNote(const int note, const int vel) = 0;
    virtual void handleInternalPolyAfterTouch(const int note, const int channel, const int value){}
    virtual void handleMidiMessage(const MidiMessage* message){}
private:
};

/** 
    Class for routing midi messages from sources to destinations. Holds a pointer to various possible destinations.
 
    Potential in future for making destinations more dynamic by allowing any class that is an InternalMidiIn to be 
    registered as a destination. 
 
 
 */

class InternalMidiRouter : public ActionBroadcaster,
                           public ActionListener
{
public:
    enum MidiDestination
    {
        MidiOut = 0,
    };
    
    
    InternalMidiRouter(InternalMidiIn* midiOut);
    
    ~InternalMidiRouter();
    
    MidiKeyboardState* getKeyboardStateObject();
    
    void handleInternalMidiNoteWithDestination(MidiDestination dest, const int note, const int vel);
    
    void actionListenerCallback (const String& message) override;
    
    void sendMidiToDestination (const int dest, const MidiMessage* message);

    void panic(MidiDestination);
    
private:
    void updateKeyboardState(const int note, const int vel);
    
    Array<InternalMidiIn*> destinations;
    ScopedPointer<MidiKeyboardState> keyboardState;
    ScopedPointer<ThreadQueue> midiNoteQueue;
    
};



#endif /* InternalMidiRouter_hpp */
