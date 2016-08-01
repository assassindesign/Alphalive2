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
/*
    THIS CLASS IS A SINGLETON. 
 
 */
class Alphalive2Engine;


class AppData : public DeletedAtShutdown,
                public ActionBroadcaster,
                public ActionListener
{
public:
    ~AppData();
    
    static AppData* Instance();
    
    bool setEnginePointer(Alphalive2Engine* newEngine);
    
    Alphalive2Engine* getEnginePointer();
    
    
    //Adds a new SphereData object to the array and returns the array index of
    //the new object.
    const int createNewSphereDataObject(const int withNumPads);
    
    SphereData* getSphereData (const int forSphere);
    
    ScaleData* getGlobalScaleData();
    
    void actionListenerCallback (const String& message) override;
    
protected:
    AppData();

private:
    
    static AppData* pInstance;
    Alphalive2Engine* engine;
    
    OwnedArray<SphereData> sphereDataArray;
    ScopedPointer<ScaleData> globalScaleData;
    
};

#endif /* AppData_hpp */
