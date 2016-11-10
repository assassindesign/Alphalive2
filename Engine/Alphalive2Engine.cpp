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
    sphereConnection = new AlphaSphereConnection();

    midiOut = new ExternalMidiOut("Alphalive 2", sphereConnection);
    
    //midiIn = new ExternalMidiIn(&deviceManager);
    
    router = new InternalMidiRouter(midiOut);
    
    spheres.add(new PlayableSphere(48, 0));
    
    
    setHIDLinkedSphere(0);
    
    masterClock = new MasterClock();
    
    setAudioChannels(0, 2);
    
    masterMixer.addInputSource(masterClock, false);
    
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
    if (AppData::Instance()->getPadPressSwitchesInspectedPad() == true)
    {
        if (velocity > 0)
        {
            static AppData::PadReference currentReference;
            currentReference = AppData::Instance()->getcurrentlyInspectingPad();
            
            if (currentReference.sphereID != hIDLinkedSphere || currentReference.padID != pad)
            {
                AppData::Instance()->setCurrentlyInspectingPad(hIDLinkedSphere, pad);
            }

        }
    }
    
    spheres[hIDLinkedSphere]->hitPad(pad, velocity);

}

void Alphalive2Engine::pressPad (const int pad, const int pressure)
{
    //DBG("Press Pad : " + String(pad) + " : " + String(pressure));
    
    spheres[hIDLinkedSphere]->pressPad(pad, pressure);

}


void Alphalive2Engine::setHIDLinkedSphere(const int newSphereID)
{
    if (newSphereID < spheres.size() && newSphereID > -1)
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

const bool Alphalive2Engine::getHIDSphereConnected()
{
    return sphereConnection->getDeviceStatus();
}

PlayableSphere* Alphalive2Engine::getHIDLinkedSpherePointer()
{
    return spheres[hIDLinkedSphere];
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
    jassertfalse; //invalid sphereID
    return nullptr;
}

PlayablePad* Alphalive2Engine::getPlayablePadPtr(const int sphereID, const int padID)
{
    PlayableSphere* workingSphere= getSpherePointer(sphereID);
    if (workingSphere != nullptr)
    {
        return workingSphere->getPad(padID);
    }
    else
    {
        jassertfalse;
        return nullptr;
    }
    
}

MasterClock* Alphalive2Engine::getMasterClockPointer()
{
    return masterClock.get();
}

InternalMidiRouter* Alphalive2Engine::getMidiRouterPointer()
{
    return router.get();
}


void Alphalive2Engine::killAllPads()
{
    for (int i = 0; i < spheres.size(); i++)
    {
        spheres[i]->killAllPads();
    }
}

void Alphalive2Engine::setAudioEnabled(const bool isEnabled)
{
    DBG("Audio Enabled: " + String(isEnabled));
}

void Alphalive2Engine::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    masterMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void Alphalive2Engine::releaseResources()
{
    masterMixer.releaseResources();
}

void Alphalive2Engine::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    //masterMixer.getNextAudioBlock(bufferToFill);

    static Random random;
    
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        // Get a pointer to the start sample in the buffer for this audio output channel
        float* const buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
        
        // Fill the required number of samples with noise betweem -0.125 and +0.125
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            buffer[sample] += random.nextFloat() * 2.0f - 1.0f;
    }
    
    
    calculateAudioOutputAverage(bufferToFill);
    
    bufferToFill.clearActiveBufferRegion();
    
    masterMixer.getNextAudioBlock(bufferToFill);
}

double Alphalive2Engine::getAudioOutputAverage(const int forChannel)
{
    
    switch (forChannel) {
        case 0:
            return audioOutputAverageL;
            break;
        case 1:
            return audioOutputAverageR;
            break;
        default:
            return 0;
            break;
    }
}


void Alphalive2Engine::calculateAudioOutputAverage(const AudioSourceChannelInfo& buffer)
{
    static float avgL, avgR;
    avgL = avgR = 0;
    
    for (int i = buffer.startSample; i < (buffer.startSample+buffer.numSamples); i++)
    {
        avgL += pow(fabsf(buffer.buffer->getSample(0, i)), 2.0);
        avgR += pow(fabsf(buffer.buffer->getSample(1, i)), 2.0);

    }
    
    audioOutputAverageL = sqrt(avgL / buffer.numSamples);
    audioOutputAverageR = sqrt(avgR / buffer.numSamples);

}





