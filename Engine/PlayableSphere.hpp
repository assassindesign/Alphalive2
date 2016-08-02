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
class PlayableSphere
{
public:
    
    
    enum MappedScale
    {
        Major = 1,
        Natural,
        Harmonic,
        Hungarian,
        Chromatic,
        FINAL_SCALE
    };
    
    enum RowConfig
    {
        OneRow = 1,
        TwoRow,
        CenterRow,
        FINAL_ROW
    };
    
    PlayableSphere(const int numPads = 48, const int _sphereID = 0);
    
    ~PlayableSphere();
    
    void hitPad(const int padID, const int vel);
    
    void pressPad(const int padID, const float pressure);
    
    void midiThruToDestination (const int note, const int vel);
    
    void setDestination (const InternalMidiRouter::MidiDestination);
    /** 
        Function that resets the sphere
     */
    
    void setRootNote(const int newRootNote);
    
    const int getRootNote();
    
    const MappedScale getCurrentScale();
    
    void setScale (MappedScale newScale);
    
    void mapSphere(const int rootNote, const MappedScale scale, const RowConfig config);
    
    void mapSphere(const int key, const int octave, const MappedScale scale, const RowConfig config);

    void setMidiThruEnabled (bool shouldBeEnabled);
    
    void setSphereMidiEnabled(bool shouldBeEnabled);
    
    void setRowConfig(const RowConfig newConfig);
    
    void setSphereID(const int newID);
    
    const int getSphereID();
    
    void transposeMidiByOctave(const int octavesToTranspose);
    
    void transposeMidiByNote(const int semiTonesToTranspose);
    
    SphereData* getSphereDataObject();
    
private:
    OwnedArray<PlayablePad> playablePads;
    InternalMidiRouter::MidiDestination destination;
    InternalMidiRouter* router;
    
    int currentRootNote, currentKey, currentOctave;
    MappedScale currentScale;
    RowConfig currentRowConfig;
    bool midiThruEnabled, sphereMidiEnabled;
    
    SphereData* sphereData;
    
    int sphereID;
};

#endif /* PlayableSphere_hpp */
