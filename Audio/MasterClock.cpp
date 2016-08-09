//
//  MasterClock.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 17/02/2016.
//  Copyright 2015 nu desine.
//

#include "MasterClock.hpp"
#include "bond.hpp"

MasterClock::MasterClock() : metronomeTimeslice(new TimeSliceThread("Metro Timeslice"))
{
    
    currentPositionInLoop = currentSample = 0;
    currentSampleRate = 44100;
    beatsInBar = 4;
    
    addActionListener(this);
    
    //open threadqueue
    queue.open();
    
    //settempo
    setTempoInternal(80);
    
    
    tickBufferSource = new SimpleAudioBufferSource();
    int fileSize;
    tickBufferSource->loadBinaryResource(BinaryData::getNamedResource("Tick_wav", fileSize), fileSize);
    tickTransportSource = new AudioTransportSource();
    tickTransportSource->setSource(tickBufferSource, 65536, metronomeTimeslice, currentSampleRate.get());
    
    tockBufferSource = new SimpleAudioBufferSource();
    tockBufferSource->loadBinaryResource(BinaryData::getNamedResource("Tock_wav", fileSize), fileSize);
    tockTransportSource = new AudioTransportSource();
    tockTransportSource->setSource(tockBufferSource, 65536, metronomeTimeslice, currentSampleRate.get());
    
    mixer = new MixerAudioSource();
    mixer->addInputSource(tickTransportSource, false);
    mixer->addInputSource(tockTransportSource, false);
    
    metronomeEnabled = true;
    
    isRunning = false;
    //isRunning = true;
}

MasterClock::~MasterClock()
{
    queue.close();
    //stopTimer();
}


void MasterClock::setTempo(const float newTempo)
{
    queue.call(bond(&MasterClock::setTempoInternal, this, newTempo));
}


void MasterClock::setTempoInternal(const float newTempo) // Maybe an alert window here if the player is running when the tempo change is supposed to happen
{

    //isRunning = true;
    
    tempo.set(newTempo);
    beatDivisions.set(4);
    beatsInBar.set(4);
    barsInLoop.set(4);
    beatsInLoop = beatsInBar.get() * barsInLoop.get();
    
    samplesPerTick.set(currentSampleRate.get() * (60.0 /tempo.get())); /// beatDivisions.get()); beat divisions currently not useful as quantizing to the bar;
    
    
    currentPositionInLoop.set(0);
    currentSample.set(0);
    
    sendActionMessage("TempoUpdate");
}

void MasterClock::setBeatsInBar(const int newNumBeats)
{
    beatsInBar.set(newNumBeats);
}



void MasterClock::actionListenerCallback (const String& message)
{
    
    if (message == "TICK")
    {
        //DBG("tick");

        masterClockListeners.call(&MasterClock::Listener::stepClockCallback, (int) currentPositionInLoop.get());
        
        
        if ((currentPositionInLoop.get() % beatsInBar.get() == 0))
        {
            masterClockListeners.call(&MasterClock::Listener::barClockCallback);
            DBG("BAR");
        }

    }
    else if (message == "TempoUpdate")
    {
        masterClockListeners.call(&MasterClock::Listener::masterTempoChanged, beatsInLoop.get(), tempo.get());

    }

   
}

void MasterClock::addListener(MasterClock::Listener* listenerToAdd)
{
    masterClockListeners.add(listenerToAdd);
}

void MasterClock::removeListener(MasterClock::Listener* listenerToRemove)
{
    masterClockListeners.remove(listenerToRemove);
}

void MasterClock::resetAndStopClock()
{
    masterClockListeners.call(&MasterClock::Listener::masterClockStopped);
    
    isRunning = false;
    
    currentPositionInLoop = currentSample = 0;

}

void MasterClock::startClock()
{
    isRunning = true;
    
    if (metronomeEnabled)
    {
        tickTransportSource->setPosition(0);
        tickTransportSource->start();
    }
    sendActionMessage("TICK");
}

float MasterClock::getTempo() const
{
    return tempo.get();
    
}


//====Audio================================================================
void MasterClock::prepareToPlay (int samplesPerBlockExpected, double newSampleRate)
{
    metronomeTimeslice->startThread();
    
    if (newSampleRate > 0)
    {
        currentSampleRate = newSampleRate;
        setTempo(getTempo());
        DBG("Sample Rate : " + String(newSampleRate));
    }
    
    mixer->prepareToPlay(samplesPerBlockExpected, newSampleRate);
}

void MasterClock::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    queue.process();
    
    if (isRunning)
    {
        for (int i = 0; i < bufferToFill.numSamples; i++)
        {
            currentSample = currentSample.get() + 1;
            if (currentSample.get() > samplesPerTick.get())
            {
                currentSample = 0;
                currentPositionInLoop.set(currentPositionInLoop.get()+1);
                if (metronomeEnabled)
                {
                    //audioBufferSource->setNextReadPosition(0);
                    if (currentPositionInLoop.get() % beatsInBar.get() == 0)
                    {
                        tickTransportSource->setPosition(0);
                        tickTransportSource->start();
                        currentPositionInLoop.set(0);
                    }
                    else
                    {
                        tockTransportSource->setPosition(0);
                        tockTransportSource->start();
                    }
                    
                }
                sendActionMessage ("TICK");

            }
        }
    }
    bufferToFill.clearActiveBufferRegion();
    
    mixer->getNextAudioBlock(bufferToFill);
    //tockTransportSource->getNextAudioBlock(bufferToFill);

    //audioBufferSource->getNextAudioBlock(bufferToFill);
}

void MasterClock::releaseResources()
{
    mixer->releaseResources();
    
    metronomeTimeslice->stopThread(500);
}

void MasterClock::setMetronomeEnabled(bool enabled)
{
    queue.call(bond(&MasterClock::setMetronomeEnabledInternal, this, enabled));
}

void MasterClock::setMetronomeEnabledInternal(bool enabled)
{
    metronomeEnabled = enabled;

}

bool MasterClock::getMetronomeEnabled()
{
    return metronomeEnabled;
}