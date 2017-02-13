//
//  SphereData.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 15/07/2016.
//
//

#ifndef SphereData_hpp
#define SphereData_hpp
#include "AppDataTypes.h"
#include "PadData.hpp"

class SphereData : public AppDataFormat
{
public:
    enum SphereType
    {
        Instrument =  0,
        Controller,
        Empty,
        FINAL_SPHERETYPE
    };
    
    enum MappedScale
    {
        Major = 0,
        Natural,
        Harmonic,
        Hungarian,
        Chromatic,
        FINAL_SCALE
    };
    
    enum RowConfig
    {
        OneRow = 0,
        TwoRow,
        CenterRow,
        FINAL_ROWCONF
    };
    
    enum DataIDS
    {
        SphereID = 0,
        SphereTypeData,
        RootNote,
        Octave,
        Scale,
        RowConfigData,
        MidiThru,
        MidiEnabled,
        FINAL_ID
    };
    
    SphereData(int numPads = 48, const int _sphereID = -1);
    ~SphereData();
    
    ValueTree* toValueTree();
    
    //============= GETS ===========================================
    //Objects
    PadData* getPadData(const int forPad);
    const int getNumPadDataObjects();
    TempoData* getTempoData();
    ScaleData* getScaleData();
    
    //Variables
    const int getSphereID();
    const enum SphereType getSphereType();
    const int getRootNote();
    const int getOctave();
    const enum MappedScale getScale();
    const enum RowConfig getRowConfig();
    const bool getMidiThruEnabled();
    const bool getSphereMidiEnabled();
    //============= SETS ===========================================
    //Variables
    bool setSphereID(const int newID);
    bool setSphereType(const int newType);
    bool setRootNote(const int newRootNote);
    bool setOctave(const int newOctave);
    bool setScale(const int newScale);
    bool setRowConfig(const int newConfig);
    void setMidiThruEnabled(const bool isEnabled);
    void setSphereMidiEnabled(const bool isEnabled);
    
    bool editPadGroup(const int groupID, const PadData::DataIDs dataID, const var changedData);
    
private:
    
    //Objects
    OwnedArray<PadData> padData;
    TempoData tempoData;
    ScaleData scaleData;
    
    CriticalSection dataLock;
    
    //Variables
    int sphereID = -1;
    SphereType sphereType = Empty;
    int rootNote = 36;
    int octave = 0;
    MappedScale scale;
    RowConfig rowConfig;
    bool midiThruEnabled = false;
    bool sphereMidiEnabled = true;
    

};





#endif /* SphereData_hpp */
