//
//  PlayableSphere.hpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
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

#ifndef PlayableSphere_hpp
#define PlayableSphere_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayablePad.hpp"
#include "InternalMidiRouter.hpp"
#include "AppData.hpp"

/*
    This class is the main concept behind Alphalive 2. It is a 'Virtual Sphere', basically a virtual
    representation of an Alphasphere that can be 'hit' or 'pressed' just like the real version. This
    allows for multiple inputs and for the HID input from the physical controller to be switched 
    between different virtual spheres, allowing for mutliple scenes to be played at the same time.
    This should open up different ways of playing the Alphasphere, mapping the whole controller to a
    single instrument and switching between virtual spheres to change functionality with no lag.
 
    Support to add multiple spheres or other midi devices should all be possible in the future,
 
    Contains algorithmic mapping of midi notes to the sphere, see MidiScales.h for more info on 
    supported scales and note layouts, this allows quick switching of key/scales/layouts with a
    single function call rather than having to remap from scratch. functionality is limited at the 
    moment to 7 note scales, though it should be fairly straightforward to add support for more 
    scales and layouts.
 
*/
class PlayableSphere : public AudioSource
{
public:
    
    
    PlayableSphere(const int numPads = 48, const int _sphereID = 0);
    
    ~PlayableSphere();
    
    void hitPad(const int padID, const int vel);
    
    void pressPad(const int padID, const float pressure);
    
    void killPad(const int padID);
    
    void killAllPads();
        
    void midiThruToDestination (const int note, const int vel);
    
    void setDestination (const InternalMidiRouter::MidiDestination);
    /** 
        Function that resets the sphere
     */
    
    void setRootNote(const int newRootNote);
    
    
    const SphereData::MappedScale getCurrentScale();
    
    void setScale (SphereData::MappedScale newScale);
    
    void mapSphere(const int rootNote, const SphereData::MappedScale scale, const SphereData::RowConfig config = SphereData::OneRow);

    void setMidiThruEnabled (bool shouldBeEnabled);
    
    void setSphereMidiEnabled(bool shouldBeEnabled);
    
    void setRowConfig(const SphereData::RowConfig newConfig);
    
    void setSphereID(const int newID);
    
    const int getSphereID();
    
    void transposeMidiByOctave(const int octavesToTranspose);
    
    void transposeMidiByNote(const int semiTonesToTranspose);
    
    SphereData* getSphereDataObject();
    
    PlayablePad* getPad(const int padToGet);

    //=====================================================================
    //  Audio callbacks
    //=====================================================================
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    void releaseResources() override;
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)override;
    
private:
    OwnedArray<PlayablePad> playablePads;
    InternalMidiRouter::MidiDestination destination;
    InternalMidiRouter* router;
    
    int sphereID;
    
    MixerAudioSource mixer;
        
    SphereData* sphereData;
    
};

#endif /* PlayableSphere_hpp */
