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
#define NUM_PAD_COLOURS 26

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppDataTypes.h"


class SphereData;
class PadData : public AppDataFormat
{
    
public:
    enum PadFunction
    {
        Midi = 0,
        Audio,
        System,
        FINAL_PADFUNCTION
    };
    
    enum PadAudioFunction
    {
        OneShot = 0,
        StandardAF,
        Looping,
        FINAL_PADAUDIOFUNCTION
    };
    
    enum PadMidiFunction{
        SingleNote = 0,
        MultiNote,
        FINAL_PADMIDIFUNCTION
    };
    
    enum PadSystemFunction{
        TransposeScene = 0,
        SwitchScene,
        MidiPgmChange,
        SceneAndPgmCh,
        KillSwitch,
        FINAL_PADSYSFUNCTION
    };
    
    enum MultiNoteMode{
        Chord = 0,
        StepThrough,
        Arpeggiate,
        FINAL_MULTINOTEMODE
    };
    
    enum PressureMode{
        StandardPM = 0,
        LFO,
        FINAL_PRESSUREMODE
    };
    
    enum LFOCurveType{
        Sine = 0,
        Square,
        Triangle,
        Saw,
        Custom,
        FINAL_LFOCURVETYPE
    };
    
    enum PressureDestination
    {
        PolyAT = 0,
        ChannetAT,
        PBup,
        PBDown,
        ControlMessage,
        OSC,
        FINAL_PRESSUREDESTINATION
    };
    
    
    enum CurveType{
        Exponential = 0,
        Linear,
        Logarithmic,
        StaticCurve,
        FINAL_CURVETYPE
        
    };
    
    enum DataIDs{
        RefreshAll = 0,
        PadID,
        MidiNotes,
        PadFunction,
        PadAudioFunction,
        PadMidiFunction,
        PadSystemFunction,
        MultiNoteMode,
        Velocity,
        Pressure,
        MidiChannel,
        PressureMode,
        PressureDestination,
        Sticky,
        Enabled,
        QuantiseEnabled,
        ReversePressure,
        VelocityCurve,
        PressureCurve,
        PadGroup,
        MidiDestination,
        PadColour,
        FINAL_DATAID
    };
    
    constexpr static int64 PadColours[NUM_PAD_COLOURS] = {0xffffffff, 0xffff0000, 0xffff4000, 0xffff8000, 0xffffc000, 0xffffff00, 0xffc0ff00, 0xff80ff00, 0xff80ff00, 0xff40ff00, 0xff00ff00, 0xff00ff40, 0xff00ff80, 0xff00ffc0, 0xff00ffff, 0xff00c0ff, 0xff0080ff, 0xff0040ff, 0xff0000ff, 0xff4000ff, 0xff8000ff, 0xffc000ff, 0xffff00ff, 0xff0000c0, 0xff000080, 0xff000040};
    
    struct MidiNote{
        int noteNumber = 0;
        int velocityPercentage = 100;
    };
    
    PadData(SphereData* _parent);
    ~PadData();
    ValueTree* toValueTree();
    bool fromValueTree(ValueTree* treeToImport);

    //============= SETS ===========================================

    bool setPadID (const int newID);
    bool setMidiNote (const int newNote);
    bool setMidiNote (const int newNote, const int velPercentage);
    bool setPadFunction (const int newFunction);
    bool setPadAudioFunction (const int newFunction);
    bool setPadMidiFunction (const int newFunction);
    bool setPadSystemFunction (const int newFunction);
    bool setMultiNoteMode (const int newMode);
    bool setVelocity (const int newVel);
    bool setPadPressure (const float newPressure);
    bool setMidiChannel (const int newChannel);
    bool setPressureMode(const int newMode);
    bool setPressureDestination(const int newDestination);
    void setSticky(const bool shouldBeSticky);
    void setEnabled(const bool shouldBeEnabled);
    void setQuantiseEnabled(const bool shouldBeQuantised);
    void setReversePressure(const bool shouldReversePressure);
    bool setVelocityCurve(const int newCurve);
    bool setPressureCurve(const int newCurve);
    bool setPadGroup(const int newGroup);
    bool setMidiDestination(const int newDestination);
    bool setPadColour(const int newColour);
    bool setLFOCurveType(const int newType);
    
    bool addMidiNote(const int newNote);
    bool addMidiNote(const int newNote, const int velPercentage);

    bool removeMidiNote(const int noteToRemove);
    void clearAllMidiNotes();
    
    //============= GETS ===========================================
    
    int getPadID();
    int getMidiNote();
    Array<PadData::MidiNote> getMidiNotes();
    int getPadFunction();
    int getPadAudioFunction();
    int getPadMidiFunction();
    int getPadSystemFunction();
    int getMultiNoteMode();
    int getVelocity();
    float getPadPressure();
    int getMidiChannel();
    int getPressureMode();
    int getPressureDestination();
    bool getSticky();
    bool getEnabled();
    bool getQuantiseEnabled();
    bool getReversePressure();
    int getVelocityCurve();
    int getPressureCurve();
    int getPadGroup();
    int getMidiDestination();
    int getPadColour();
    int getLFOCurveType();
    
    SphereData* getParentSphere();

private:
    int padID = -1;
    Array<MidiNote> midiNotes;
    int padFunction = 0;
    int padAudioFunction = 0;
    int padMidiFunction = 0;
    int padSystemFunction = 0;
    int multiNoteMode = 0;
    int velocity = 0;
    float padPressure = 0;
    int midiChannel = 1;
    int pressureMode = 0;
    int pressureDestination = 0;
    bool sticky = false;
    bool enabled = true;
    bool quantiseEnabled = false;
    bool reversePressure = false;
    int velocityCurve = CurveType::Logarithmic;
    int pressureCurve = 0;
    int padGroup = 0;
    int midiDestination = 0;
    int padColour = 0;
    int lFOCurveType = 0;
    
    
    SphereData* parent;
    
    CriticalSection dataLock;
    
};
#endif /* PadData_hpp */
