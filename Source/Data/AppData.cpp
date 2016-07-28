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
    
    globalScaleData = new ScaleData();

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

bool AppData::setEnginePointer(MidiSequencerEngine* newEngine = nullptr)
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

MidiSequencerEngine* AppData::getEnginePointer()
{
    return engine;
}

const int AppData::createNewSphereDataObject(const int withNumPads)
{
    sphereDataArray.add(new SphereData(withNumPads, 48));
    return sphereDataArray.size()-1;
}

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

ScaleData* AppData::getGlobalScaleData()
{
    return globalScaleData;
}


void AppData::actionListenerCallback (const String& message)
{
    
}
