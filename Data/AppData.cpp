//
//  AppData.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 13/06/2016.
//
//

#include "AppData.hpp"
AppData* AppData::pInstance = 0;


AppData::AppData() : engine(nullptr)
{
    sphereDataArray.clear();
    createNewSphereDataObject(48);
    padPressSwitchesInspectedPad = false;
    //sphereDataArray.add(new SphereData(48));
}

AppData::~AppData()
{
    
}

AppData* AppData::Instance()
{
    if (pInstance == 0) //is this is the first call ?
    {
        //create sole instance
        pInstance = new AppData();
    }
    //address of sole instance
    return pInstance;
}


const int AppData::createNewSphereDataObject(const int withNumPads)
{
    sphereDataArray.add(new SphereData(withNumPads, sphereDataArray.size()));
    return sphereDataArray.size()-1;
}


//============= GETS ===========================================
SphereData* AppData::getSphereData (const int forSphere)
{
    if (forSphere < sphereDataArray.size() && !sphereDataArray.isEmpty())
    {
        return sphereDataArray[forSphere];
    }
    else
    {
        jassert(false);
        return nullptr;
    }
}

Alphalive2Engine* AppData::getEnginePointer()
{
    return engine;
}

CustomLookAndFeel* AppData::getAlphaliveLookAndFeel()
{
    return lookAndFeelManager.getMyLookAndFeel();
}


bool AppData::getAdvancedFeaturesEnabled()
{
    return advancedFeaturesEnabled;
}

bool AppData::getPadPressSwitchesInspectedPad()
{
    return padPressSwitchesInspectedPad;
}


AppData::PadReference AppData::getcurrentlyInspectingPad()
{
    return currentlyInspectingPad;
}

PadData* AppData::getCurrentlyInspectingPadDataPtr()
{
    if (currentlyInspectingPad.sphereID == -1 || currentlyInspectingPad.padID == -1)
    {
        return nullptr;
    }
    else
    {
        return sphereDataArray.getUnchecked(currentlyInspectingPad.sphereID)->getPadData(currentlyInspectingPad.padID);
    }
}



//============= SETS ===========================================
bool AppData::setEnginePointer(Alphalive2Engine* newEngine = nullptr)
{
    if (newEngine != nullptr)
    {
        engine = newEngine;
        return true;
    }
    else
    {
        return false;
    }
}

void AppData::setAdvancedFeaturesEnabled(const bool enabled)
{
    dataLock.enter();
    advancedFeaturesEnabled = enabled;
    dataLock.exit();
    callListeners(DataIDs::AdvancedEnabled, AppDataFormat::AppDataType);
}

bool AppData::setCurrentlyInspectingPad(const int sphereID, const int padID)
{
    bool success = false;
    if (sphereID > -1 && sphereID < sphereDataArray.size()) //if valid sphere ID
    {
        if (padID > -1 && padID < sphereDataArray.getUnchecked(sphereID)->getNumPadDataObjects()) //if valid pad ID
        {
            dataLock.enter();
            currentlyInspectingPad.sphereID = sphereID;
            currentlyInspectingPad.padID = padID;
            dataLock.exit();
            
            callListeners(DataIDs::InspectingPad, AppDataFormat::AppDataType);
            success = true;
        }
    }
    
    if (!success)
    {
        dataLock.enter();
        currentlyInspectingPad.sphereID = -1;
        currentlyInspectingPad.padID = -1;
        dataLock.exit();
        
        callListeners(DataIDs::InspectingPad, AppDataFormat::AppDataType);
    }
    
    
    return success;
}

void AppData::setPadPressSwitchesInspectedPad(const bool enabled)
{
    dataLock.enter();
    padPressSwitchesInspectedPad = enabled;
    dataLock.exit();
    callListeners(DataIDs::PadPressSwitch, AppDataFormat::AppDataType);
}


