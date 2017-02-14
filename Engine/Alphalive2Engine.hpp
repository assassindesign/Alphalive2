//
//  MidiSequencerEngine.hpp
//  Monster Midi Sequencer
//
//  Created by Luke Saxton on 26/04/2016.
//
//
//  Alphalive 2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  Alphalive 2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Alphalive2Engine_hpp
#define Alphalive2Engine_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayableSphere.hpp"
#include "InternalMidiRouter.hpp"
#include "AlphaSphereConnection.hpp"
#include "ExternalMidiIn.hpp"
#include "ExternalMidiOut.hpp"
#include "MasterClock.hpp"
#include "AppData.hpp"
#include "AudioMeterButton.hpp"

/*
    Top level engine. Contains all functionality classes related to audio, midi and
    Alphasphere connection. AppData holds a pointer to this so it can be accessed by GUI 
    components.
 
    Main function of this class to to provide pointers to sub-classes. Should be 
    expandable to support multiple PlayableSphere(s) so enable scene-like behavior and
    have multiple spheres running at the same time.
*/

class Alphalive2Engine : public DeletedAtShutdown,
                         public AudioAppComponent
{
public:
    Alphalive2Engine();
    ~Alphalive2Engine();
    
    void initialise();
    
    void midiThru(const MidiMessage&);
    
    
    //ASConnection Listeners
    void hitPad (const int pad, const int velocity);
    void pressPad (const int pad, const int pressure);
    
    //Set which playable sphere in the array to direct HID messages to
    void setHIDLinkedSphere(const int newSphereID);
    const int getHIDLinkedSphere();
    const bool getHIDSphereConnected();
    PlayableSphere* getHIDLinkedSpherePointer();
    PlayableSphere* getSpherePointer(const int sphereID);
    PlayablePad* getPlayablePadPtr(const int sphereID, const int padID);
    MasterClock* getMasterClockPointer();
    InternalMidiRouter* getMidiRouterPointer();
    
    void killAllPads();
    void setAudioEnabled(const bool isEnabled);

    //===========================================================================
    //  Audio callbacks for MasterClock
    //===========================================================================

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    void releaseResources() override;
 
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)override;
    
    double getAudioOutputAverage(const int forChannel);
    
    void handleExternalMidiClock(const MidiMessage midiMessage);
    
private:
    void calculateAudioOutputAverage(const AudioSourceChannelInfo& buffer);
    
    
    OwnedArray<PlayableSphere> spheres;
    
    ScopedPointer<InternalMidiRouter> router;

    ScopedPointer<ExternalMidiOut> midiOut;
    
    //ScopedPointer<ExternalMidiIn> midiIn;
    
    ScopedPointer<AlphaSphereConnection> sphereConnection;
    
    ScopedPointer<MasterClock> masterClock;
    
    //Audio Classes
    MixerAudioSource masterMixer;
    
    double audioOutputAverageL, audioOutputAverageR;
    
    int hIDLinkedSphere;
};

#endif /* Alphalive2Engine_hpp */
