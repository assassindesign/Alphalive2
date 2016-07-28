//
//  PadData.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 15/07/2016.
//
//

#ifndef PadData_hpp
#define PadData_hpp

#define MAX_GROUPS 16

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppDataTypes.h"

class SphereData;
class PadData : public AppDataFormat
{
    
public:
    enum PadFunction
    {
        Midi = 0,
        Chord,
        Sequencer,
        Controller
    };
    
    enum PressureMode{
        PolyAT = 0,
        CC,
        ChannelAT,
        PitchUp,
        PitchDown,
        Sequence
    };
    
    enum CurveType{
        Exponential = 0,
        Linear,
        Logarithmic,
        StaticCurve
    };
    
    struct MidiNote{
        int noteNumber = 0;
        int velocityPercentage = 100;
    };
    
    PadData(SphereData* _parent);
    ~PadData();
    ValueTree* toValueTree();
    bool fromValueTree(ValueTree* treeToImport);

    //============= SETS ===========================================

    void setPadID (const int newID);
    bool setMidiNote (const int newNote);
    bool setMidiNote (const int newNote, const int velPercentage);
    void setPadFunction (const int newFunction);
    bool setVelocity (const int newVel);
    bool setPadPressure (const float newPressure);
    void setMidiChannel (const int newChannel);
    void setPressureMode(const int newMode);
    void setSticky(const bool shouldBeSticky);
    void setEnabled(const bool shouldBeEnabled);
    void setQuantiseEnabled(const bool shouldBeQuantised);
    void setReversePressure(const bool shouldReversePressure);
    void setVelocityCurve(const int newCurve);
    void setPressureCurve(const int newCurve);
    void setPadGroup(const int newGroup);
    void setMidiDestination(const int newDestination);
    
    bool addMidiNote(const int newNote);
    bool addMidiNote(const int newNote, const int velPercentage);

    void removeMidiNote(const int noteToRemove);
    void clearAllMidiNotes();
    
    //============= GETS ===========================================
    
    int getPadID();
    int getMidiNote();
    Array<PadData::MidiNote> getMidiNotes();
    int getPadFunction();
    int getVelocity();
    float getPadPressure();
    int getMidiChannel();
    int getPressureMode();
    bool getSticky();
    bool getEnabled();
    bool getQuantiseEnabled();
    bool getReversePressure();
    int getVelocityCurve();
    int getPressureCurve();
    int getPadGroup();
    int getMidiDestination();
    
    
    SphereData* getParentSphere();

private:
    int padID = -1;
    Array<MidiNote> midiNotes;
    int padFunction = 0;
    int velocity = 0;
    float padPressure = 0;
    int midiChannel = 1;
    int pressureMode = 0;
    bool sticky = false;
    bool enabled = true;
    bool quantiseEnabled = false;
    bool reversePressure = false;
    int velocityCurve = 0;
    int pressureCurve = 0;
    int padGroup = 0;
    int midiDestination = 0;

    
    SphereData* parent;
    
    CriticalSection dataLock;
    
};
#endif /* PadData_hpp */
