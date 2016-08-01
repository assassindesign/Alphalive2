//
//  MidiSequencerEngine.cpp
//  Monster Midi Sequencer
//
//  Created by Luke Saxton on 26/04/2016.
//
//

#include "Alphalive2Engine.hpp"


Alphalive2Engine::Alphalive2Engine()
{
    
}

Alphalive2Engine::~Alphalive2Engine()
{
    shutdownAudio();
}

void Alphalive2Engine::initialise()
{
    midiOut = new ExternalMidiOut("Alphalive 2");
    
    router = new InternalMidiRouter(midiOut);
    
    spheres.add(new PlayableSphere(48, 0));
    
    sphereConnection = new AlphaSphereConnection();
    
    setHIDLinkedSphere(0);
    
    masterClock = new MasterClock();
    
    setAudioChannels(0, 2);
    
    //masterClock->startClock();
    
}





void Alphalive2Engine::midiThru(const MidiMessage& message)
{
    //DBG("MIDI MESSAGE IN");
    router->sendMidiToDestination(InternalMidiRouter::MidiDestination::MidiOut, &message);
    
}


void Alphalive2Engine::hitPad (const int pad, const int velocity)
{
    //DBG("Hit Pad : " + String(pad) + " : " + String(velocity));
    spheres[hIDLinkedSphere]->hitPad(pad, velocity);

}

void Alphalive2Engine::pressPad (const int pad, const int pressure)
{
    //DBG("Press Pad : " + String(pad) + " : " + String(pressure));
    
    spheres[hIDLinkedSphere]->pressPad(pad, pressure);

}


void Alphalive2Engine::setHIDLinkedSphere(const int newSphereID)
{
    if (newSphereID < spheres.size())
    {
        if (spheres[newSphereID] != nullptr)
        {
            hIDLinkedSphere = newSphereID;
            
        }
    }
}

const int Alphalive2Engine::getHIDLinkedSphere()
{
    return hIDLinkedSphere;
}


PlayableSphere* Alphalive2Engine::getSpherePointer(const int sphereID)
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

MasterClock* Alphalive2Engine::getMasterClockPointer()
{
    return masterClock.get();
}

InternalMidiRouter* Alphalive2Engine::getMidiRouterPointer()
{
    return router.get();
}


void Alphalive2Engine::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    masterClock->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void Alphalive2Engine::releaseResources()
{
    masterClock->releaseResources();
}

void Alphalive2Engine::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    masterClock->getNextAudioBlock(bufferToFill);
}

