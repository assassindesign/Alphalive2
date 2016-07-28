//
//  MidiSequencerEngine.hpp
//  Monster Midi Sequencer
//
//  Created by Luke Saxton on 26/04/2016.
//
//

#ifndef MidiSequencerEngine_hpp
#define MidiSequencerEngine_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayableSphere.hpp"
#include "InternalMidiRouter.hpp"
#include "AlphaSphereConnection.hpp"
#include "ExternalMidiIn.hpp"
#include "ExternalMidiOut.hpp"
#include "MasterClock.hpp"
#include "AppData.hpp"

class MidiSequencerEngine : public DeletedAtShutdown,
                            public AudioAppComponent
{
public:
    MidiSequencerEngine();
    ~MidiSequencerEngine();
    
    void initialise();
    
    
    void midiThru(const MidiMessage&);
    
    
    //ASConnection Listeners
    void hitPad (const int pad, const int velocity);
    void pressPad (const int pad, const int pressure);
    
    //Set which playable sphere in the array to direct HID messages to
    void setHIDLinkedSphere(const int newSphereID);
    const int getHIDLinkedSphere();

    PlayableSphere* getSpherePointer(const int sphereID);
    MasterClock* getMasterClockPointer();
    InternalMidiRouter* getMidiRouterPointer();
    
    //===========================================================================
    //  Audio callbacks for MasterClock
    //===========================================================================

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    void releaseResources() override;
 
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)override;
private:
    
    OwnedArray<PlayableSphere> spheres;
    
    ScopedPointer<InternalMidiRouter> router;

    ScopedPointer<ExternalMidiOut> midiOut;
    
    ScopedPointer<AlphaSphereConnection> sphereConnection;
    
    ScopedPointer<MasterClock> masterClock;
    
    int hIDLinkedSphere;
};

#endif /* MidiSequencerEngine_hpp */
