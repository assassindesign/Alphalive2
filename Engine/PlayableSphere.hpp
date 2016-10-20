//
//  PlayableSphere.hpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
//
//

#ifndef PlayableSphere_hpp
#define PlayableSphere_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayablePad.hpp"
#include "InternalMidiRouter.hpp"
#include "AppData.hpp"
//#include "MidiSequencerEngine.cpp"
/**
    Class that converts pad hits to useful data. the sphere contains a collection of pad objects that hold information on what each pad on the alphasphere is currently set to do and return midi, control, system or audio data accordingly. In Alphalearn, that data is only midi note and aftertouch data, but the classes can be expanded in future versions of the engine.
 
    @see PlayablePad
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
