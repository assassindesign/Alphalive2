//
//  MasterClock.hpp
//  iAlpha
//
//  Created by Luke Saxton on 14/10/2015.
//  Copyright 2015 nu desine.
//
//  iAlpha is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  iAlpha is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MasterClock_hpp
#define MasterClock_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "ThreadQueue.hpp"
#include "SimpleAudioBufferSource.hpp"

/**
    Master Beat Clock for the app. Runs off the Audio callback for sample precise timing. Subclasses can inherit from MasterClock::Listener to receive callbacks on the beat and the bar once registered. Action Broadcasters are used to dispatch messages to listeners on the message thread rather than the audio thread. Potential for implementing Syncronous callback capabilities on the audio thread in future.

        Class ported from iAlpha, where it relied on a valuetree for updates and information. Still working on proper cross thread parameter updates using threadqueues.
 */

class MasterClock : public ActionListener,
                    public AudioSource,
                    public ActionBroadcaster

{
public:
    MasterClock();

    ~MasterClock();

    class Listener
    {
    public:
        virtual ~Listener(){}

        virtual void barClockCallback() = 0;

        virtual void stepClockCallback(const int currentPositionInLoop) {}

        virtual void masterClockStopped() = 0;

        virtual void masterTempoChanged(const int beatsInLoop, const float newTempo) {}
        virtual void rawClockCallback(const int clock){};
    private:

    };

    void hiResTimerCallback();

    void addListener(MasterClock::Listener* listenerToAdd);

    void removeListener(MasterClock::Listener* listenerToRemove);

    void resetAndStopClock();
    
    void startClock();

    void setTempo(const float newTempo);

    void setBeatsInBar(const int newNumBeats);

    void actionListenerCallback (const String& message) override;


    //====Audio================================================================
    void prepareToPlay (int samplesPerBlockExpected, double newSampleRate) override;

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    float getTempo() const;

    void setMetronomeEnabled(bool enabled);

    bool getMetronomeEnabled();
    
    void handleExternalMidiClock(const MidiMessage midiMessage);
    
    void setUsingExternalClock(const bool useExternal);
    bool getUsingExternalClock();
    
private:
    void setMetronomeEnabledInternal(bool enabled);

    void setTempoInternal(const float newTempo);
    
    void tick();

    //ValueTree transportTree;
    ListenerList<MasterClock::Listener> masterClockListeners;

    Atomic<float> tempo, beatDivisions, currentSampleRate;
    Atomic<int> currentPositionInLoop, beatsInLoop, barsInLoop, beatsInBar, currentSample, samplesPerLoop, samplesPerTick;

    ScopedPointer<SimpleAudioBufferSource> tickBufferSource;
    ScopedPointer<SimpleAudioBufferSource> tockBufferSource;

    ScopedPointer<AudioTransportSource> tickTransportSource;
    ScopedPointer<AudioTransportSource> tockTransportSource;

    ScopedPointer<MixerAudioSource> mixer;
    
    ScopedPointer<TimeSliceThread> metronomeTimeslice;

    bool isRunning = false;
    bool metronomeEnabled = true;
    bool useExternalClock = false;
    //CriticalSection sharedMemory;

    ThreadQueue queue;
};



#endif /* MasterClock_hpp */
