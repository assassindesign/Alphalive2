//
//  AppData.hpp
//  AlphaLive 2
//
//  Created by Luke Saxton on 13/06/2016.
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

#ifndef AppData_hpp
#define AppData_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppDataTypes.h"
#include "SphereData.hpp"
#include "PadData.hpp"
#include "Constants.h"
#include "AlphaLiveLookAndFeel.hpp"

/*
    THIS CLASS IS A SINGLETON. 
 
    This is the main class that holds all the runtime and state data for the app.
    This class can be accessed from anywhere in the program by using AppData::Instance();
    Access to member variables is threadsafe (Locking), this is one of the first classes 
    created so it's a little behind on best coding practices, I have begun moving
    sub-classes to use lock-free (atomic) data storage, This class should evenutally
    follow suit.
 
    So that the GUI can issue commands to the engine without pointer-passing gore,
    this class contains a pointer to the Alphalive2Engine, which is initialised and passed
    in by MainContentComponent. Note the forward definition of the Alphalive2Engine class,
    avoiding self-referencing issues.
 
    Inherits from AppDataFormat (see AppDataListeners.h for more info) to allow a broadcaster
    -listener relationship with the UI.
 
    Stores data relevant to the whole app.
 
    As it is accessible from anywhere, this class also contains pointers to the MasterClock
    and the InternalMidiRouter, so MIDI clock Messages can be passed directly to and from 
    the relevant objects
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
    bool padPressSwitchesInspectedPad = true;
    CriticalSection dataLock;
};

#endif /* AppData_hpp */
