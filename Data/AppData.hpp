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
#include "AlphaLiveLookAndFeel.hpp"

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
        PadPressSwitch,
        HIDSphereConnected,
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
    
    void refreshHIDDeviceConnected();
    
    //============= GETS ===========================================
    //Objects
    SphereData* getSphereData (const int forSphere);
    Alphalive2Engine* getEnginePointer();
    CustomLookAndFeel* getAlphaliveLookAndFeel();
    
    //Variables
    bool getAdvancedFeaturesEnabled();
    bool getPadPressSwitchesInspectedPad();
    PadReference getcurrentlyInspectingPad();
    WeakReference<PadData> getCurrentlyInspectingPadDataPtr();
    //============= SETS ===========================================
    //Objects
    bool setEnginePointer(Alphalive2Engine* newEngine);
    bool setCurrentlyInspectingPad(const int sphereID, const int padID);
    
    //Variables
    void setAdvancedFeaturesEnabled(const bool enabled);
    void setPadPressSwitchesInspectedPad(const bool enabled);

protected:
    AppData();

private:
    //Objects
    static AppData* pInstance;
    Alphalive2Engine* engine;
    OwnedArray<SphereData> sphereDataArray;
    LookAndFeelManager lookAndFeelManager;
    
    //Variables
    bool advancedFeaturesEnabled = true;
    PadReference currentlyInspectingPad;
    bool padPressSwitchesInspectedPad = false;
    CriticalSection dataLock;
};

#endif /* AppData_hpp */
