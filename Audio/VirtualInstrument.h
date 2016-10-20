//
//  VirtualInstrument.h
//  AlphaLearn
//
//  Created by Luke Saxton on 12/02/2016.
//
//

#ifndef VirtualInstrument_h
#define VirtualInstrument_h

#include "../JuceLibraryCode/JuceHeader.h"

#include "InternalMidiRouter.hpp"
#include "Constants.h"
#include "ThreadQueue.hpp"
#include "bond.hpp"

/**
 Base class for virtual instruments. Accepts midi in and returns audio out.
 
 */
class VirtualInstrument : public AudioSource,
                          public InternalMidiIn
{
public:
    VirtualInstrument()
    {
        mixer = new MixerAudioSource();
        actionQueue.open();

    }
    
    ~VirtualInstrument()
    {
        actionQueue.close();
    }
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        mixer->prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        actionQueue.process();
        mixer->getNextAudioBlock(bufferToFill);
    }
    
    void releaseResources() override
    {
        mixer->releaseResources();
    }
    
    virtual void handleInternalMidiNote(const int note, const int vel) override = 0;

    
protected:
    ThreadQueue actionQueue;

    MixerAudioSource* getMixer()
    {
        return mixer;
    }
    
private:
   
    
    ScopedPointer<MixerAudioSource> mixer;
    
    
};

#endif /* VirtualInstrument_h */
