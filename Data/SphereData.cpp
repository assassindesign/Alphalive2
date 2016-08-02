//
//  SphereData.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 15/07/2016.
//
//

#include "SphereData.hpp"

SphereData::SphereData(int numPads, const int _sphereID) : sphereID(_sphereID)
{
    for (int i = 0 ; i < numPads; i++)
    {
        padData.add(new PadData(this));
        padData.getLast()->setPadID(i);
    }
}

ValueTree* SphereData::toValueTree()
{
    ValueTree* outputTree = new ValueTree("SphereData");
    outputTree->setProperty("SphereID", sphereID, 0);
    outputTree->setProperty("SphereType", int(sphereType), 0);
    
    for (int i = 0 ; i < padData.size(); i++)
    {
        outputTree->addChild(*padData[i]->toValueTree(), -1, 0);
    }
    
    return outputTree;
}

const int SphereData::getSphereID()
{
    return sphereID;
}

PadData* SphereData::getPadData(const int forPad)
{
    if (forPad > -1 && forPad < padData.size())
    {
        return padData.getUnchecked(forPad);
    }
    else
    {
        return nullptr;
    }
}

const int SphereData::getNumPadDataObjects()
{
    return padData.size();
}

TempoData* SphereData::getTempoData()
{
    return &tempoData;
}
