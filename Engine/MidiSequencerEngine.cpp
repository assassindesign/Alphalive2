//
//  MidiSequencerEngine.cpp
//  Monster Midi Sequencer
//
//  Created by Luke Saxton on 26/04/2016.
//
//

#include "MidiSequencerEngine.hpp"


MidiSequencerEngine::MidiSequencerEngine()
{
    
}

MidiSequencerEngine::~MidiSequencerEngine()
{
    shutdownAudio();
}

void MidiSequencerEngine::initialise()
{
    midiOut = new ExternalMidiOut("Monster Sequencer");
    
    router = new InternalMidiRouter(midiOut);
    
    spheres.add(new PlayableSphere(48, 0));
    
    sphereConnection = new AlphaSphereConnection();
    
    setHIDLinkedSphere(0);
    
    masterClock = new MasterClock();
    
    setAudioChannels(0, 2);
    
    //masterClock->startClock();
    
}





void MidiSequencerEngine::midiThru(const MidiMessage& message)
{
    //DBG("MIDI MESSAGE IN");
    router->sendMidiToDestination(InternalMidiRouter::MidiDestination::MidiOut, &message);
    
}


void MidiSequencerEngine::hitPad (const int pad, const int velocity)
{
    //DBG("Hit Pad : " + String(pad) + " : " + String(velocity));
    spheres[hIDLinkedSphere]->hitPad(pad, velocity);

}

void MidiSequencerEngine::pressPad (const int pad, const int pressure)
{
    //DBG("Press Pad : " + String(pad) + " : " + String(pressure));
    
    spheres[hIDLinkedSphere]->pressPad(pad, pressure);

}


void MidiSequencerEngine::setHIDLinkedSphere(const int newSphereID)
{
    if (newSphereID < spheres.size())
    {
        if (spheres[newSphereID] != nullptr)
        {
            hIDLinkedSphere = newSphereID;
            
        }
    }
}

const int MidiSequencerEngine::getHIDLinkedSphere()
{
    return hIDLinkedSphere;
}


PlayableSphere* MidiSequencerEngine::getSpherePointer(const int sphereID)
{
    for (int i = 0; i < spheres.size(); i++)
    {
        if (spheres[i]->getSphereID() == sphereID)
        {
            return spheres[i];
        }
    }
    return nullptr;
}

MasterClock* MidiSequencerEngine::getMasterClockPointer()
{
    return masterClock.get();
}

InternalMidiRouter* MidiSequencerEngine::getMidiRouterPointer()
{
    return router.get();
}


void MidiSequencerEngine::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    masterClock->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MidiSequencerEngine::releaseResources()
{
    masterClock->releaseResources();
}

void MidiSequencerEngine::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    masterClock->getNextAudioBlock(bufferToFill);
}

