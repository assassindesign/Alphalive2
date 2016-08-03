//
//  AppData.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 13/06/2016.
//
//

#ifndef AppData_hpp
#define AppData_hpp

#include "../JuceLibraryCode/JuceHeader.h"
//#include "MidiSequencerEngine.hpp"
#include "AppDataTypes.h"
#include "SphereData.hpp"
#include "PadData.hpp"
#include "Constants.h"
/*
    THIS CLASS IS A SINGLETON. 
 
 */
class Alphalive2Engine;


class AppData : public DeletedAtShutdown,
                public AppDataFormat
{
public:
    
    enum DataIDs{
        AdvancedEnabled = 0,
        InspectingPad,
        LAST_ID
    };
    
    struct PadReference{
        int sphereID = 0;
        int padID = 0;
    };
    
    ~AppData();
    
    static AppData* Instance();
    
    
    
    
    //Adds a new SphereData object to the array and returns the array index of
    //the new object.
    const int createNewSphereDataObject(const int withNumPads);
    
    //============= GETS ===========================================
    //Objects
    SphereData* getSphereData (const int forSphere);
    Alphalive2Engine* getEnginePointer();

    //Variables
    bool getAdvancedFeaturesEnabled();
    PadReference getcurrentlyInspectingPad();
    PadData* getCurrentlyInspectingPadPtr();
    //============= SETS ===========================================
    //Objects
    bool setEnginePointer(Alphalive2Engine* newEngine);
    bool setCurrentlyInspectingPad(const int sphereID, const int padID);
    
    //Variables
    void setAdvancedFeaturesEnabled(const bool enabled);
protected:
    AppData();

private:
    //Objects
    static AppData* pInstance;
    Alphalive2Engine* engine;
    OwnedArray<SphereData> sphereDataArray;
    
    //Variables
    bool advancedFeaturesEnabled = false;
    PadReference currentlyInspectingPad;
    CriticalSection dataLock;
};

#endif /* AppData_hpp */
