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
    enum PadFunctions
    {
        Midi = 0,
        Audio,
        System,
        FINAL_PADFUNCTION
    };
    
    enum PadAudioFunctions
    {
        OneShot = 0,
        StandardAF,
        Looping,
        FINAL_PADAUDIOFUNCTION
    };
    
    enum PadMidiFunctions{
        SingleNote = 0,
        MultiNote,
        FINAL_PADMIDIFUNCTION
    };
    
    enum PadSystemFunctions{
        TransposeScene = 0,
        SwitchScene,
        MidiPgmChange,
        SceneAndPgmCh,
        KillSwitch,
        FINAL_PADSYSFUNCTION
    };
    
    enum NoteTriggerModes{
        StandardNoteMode = 0,
        ToggleNoteMode,
        LatchNoteMode,
        TriggerNoteMode,
        FINAL_SINGLENOTEMODE
    };
    
    enum MultiNoteModes{
        Chord = 0,
        StepThrough,
        Arpeggiate,
        FINAL_MULTINOTEMODE
    };
    
    enum PressureModes{
        StandardPM = 0,
        LFO,
        FINAL_PRESSUREMODE
    };
    
    enum LFOCurveTypes{
        Sine = 0,
        Square,
        Triangle,
        Saw,
        Custom,
        FINAL_LFOCURVETYPE
    };
    
    enum PressureDestinations
    {
        PolyAT = 0,
        ChannetAT,
        PBup,
        PBDown,
        ModWheel,
        MidiCC,
        OSC,
        FINAL_PRESSUREDESTINATION
    };
    
    
    enum CurveTypes{
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
        NoteTriggerMode,
        MultiNoteMode,
        Velocity,
        Pressure,
        MidiChannel,
        PressureMode,
        PressureDestination,
        Sticky,
        NoteEnabled,
        PressureEnabled,
        QuantiseEnabled,
        ReversePressure,
        VelocityCurve,
        PressureCurve,
        PadGroup,
        MidiDestination,
        PadColour,
        DynamicMidiChannel,
        PadEnabled,
        MidiCCType,
        PressureRange,
        NoteRepeat,
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

    void testFunction() {}
    //============= SETS ===========================================
    
    bool setPadID (const int newID);
    bool setMidiNote (const int newNote);
    bool setMidiNote (const int newNote, const int velPercentage);
    bool setMidiNote (const int arrayIndex, const int newNote, const int velPercentage);
    bool setVelPercentForNote(const int noteToFind, const int newVelPercentage);
    bool setPadFunction (const int newFunction);
    bool setPadAudioFunction (const int newFunction);
    bool setPadMidiFunction (const int newFunction);
    bool setPadSystemFunction (const int newFunction);
    bool setNoteTriggerMode (const int newMode);
    bool setMultiNoteMode (const int newMode);
    bool setVelocity (const int newVel);
    bool setPadPressure (const float newPressure);
    bool setMidiChannel (const int newChannel);
    bool addMidChannel(const int newChannel);
    bool removeMidiChannel(const int channelToRemove);
    void clearAllMidiChannels();
    bool setPressureMode(const int newMode);
    bool setPressureDestination(const int newDestination);
    bool setPressureRange(const float newMin, const float newMax);
    void setSticky(const bool shouldBeSticky);
    void setNoteEnabled(const bool shouldBeEnabled);
    void setPressureEnabled(const bool shouldBeEnabled);
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
    bool setAllMidiNotes(Array<PadData::MidiNote> newNotes);
    
    bool removeMidiNote(const int noteToRemove);
    void clearAllMidiNotes();
    void setDynamicMidiChannel(const bool enabled);
    void setPadEnabled (const bool enabled);
    bool setMidiCC(const int newCC);
    
    //============= GETS ===========================================
    
    int getPadID();
    int getMidiNote();
    Array<PadData::MidiNote> getMidiNotes();
    int getNumMidiNotes();
    int getPadFunction();
    int getPadAudioFunction();
    int getPadMidiFunction();
    int getPadSystemFunction();
    int getNoteTriggerMode();
    int getMultiNoteMode();
    int getVelocity();
    float getPadPressure();
    int getMidiChannel();
    Array<int> getMidiChannels();
    int getNumMidiChannels();
    int getPressureMode();
    int getPressureDestination();
    float getPressureMin();
    float getPressureRange();
    bool getSticky();
    bool getNoteEnabled();
    bool getPressureEnabled();
    bool getQuantiseEnabled();
    bool getReversePressure();
    int getVelocityCurve();
    int getPressureCurve();
    int getPadGroup();
    int getMidiDestination();
    int getPadColour();
    int getLFOCurveType();
    bool getDynamicMidiChannel();
    bool getPadEnabled();
    int getMidiCC();
    
    //SphereData* getParentSphere();
    int getParentSphereID();

    WeakReference<PadData>::Master masterReference;

private:
    
    Atomic<int> padID {-1};
    Array<MidiNote> midiNotes;
    Atomic<int> padFunction {0};
    Atomic<int> padAudioFunction {0};
    Atomic<int> padMidiFunction  {PadMidiFunctions::MultiNote};
    Atomic<int> padSystemFunction {0};
    Atomic<int> noteTriggerMode {0};
    Atomic<int> multiNoteMode {0};
    Atomic<int> velocity {0};
    Atomic<float> padPressure {0};
    Array<int> midiChannels;
    Atomic<int> pressureMode {0};
    Atomic<int> pressureDestination {0};
    Atomic<float> pressureMin {0.0};
    Atomic<float> pressureRange {1.0};
    Atomic<int> sticky {false};
    Atomic<int> noteEnabled {true};
    Atomic<int> pressureEnabled {true};
    Atomic<int> quantiseEnabled {false};
    Atomic<int> reversePressure {false};
    Atomic<int> velocityCurve {CurveTypes::Linear};
    Atomic<int> pressureCurve {0};
    Atomic<int> padGroup {0};
    Atomic<int> midiDestination {0};
    Atomic<int> padColour {0};
    Atomic<int> lFOCurveType {0};
    Atomic<int> dynamicMidiChannel {false};
    Atomic<int> padEnabled {true};
    Atomic<int> midiCC {0};
    
    SphereData* parent = nullptr;
    
    CriticalSection dataLock;
    
};
#endif /* PadData_hpp */
