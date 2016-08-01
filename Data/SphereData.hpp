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
        Empty
    };
    
    SphereData(int numPads = 48, const int _sphereID = -1) : sphereID(_sphereID)
    {
        for (int i = 0 ; i < numPads; i++)
        {
            padData.add(new PadData(this));
            padData.getLast()->setPadID(i);
        }
    }
    ~SphereData(){}
    
    ValueTree* toValueTree()
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
    
    const int getSphereID()
    {
        return sphereID;
    }
    
    PadData* getPadData(const int forPad)
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
    
    const int getNumPadDataObjects()
    {
        return padData.size();
    }
    
private:
    OwnedArray<PadData> padData;
    int sphereID = -1;
    SphereType sphereType = Empty;
};

class TempoData : public AppDataFormat
{
public:
    TempoData(){}
    ~TempoData(){};
    
    float tempo = 120;
    int beatsPerBar = 4;
    
};



#endif /* SphereData_hpp */
