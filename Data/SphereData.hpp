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
    
    OwnedArray<PadData> padData;
    int sphereID = -1;
    SphereType sphereType = Empty;
    
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
};

class TempoData : public AppDataFormat
{
public:
    TempoData(){}
    ~TempoData(){};
    
    float tempo = 120;
    int beatsPerBar = 4;
    
};


class ScaleData : public AppDataFormat
{
public:
    ScaleData(){}
    ~ScaleData(){};
    
    enum BaseKey{
        C = 0,
        CSharp,
        D,
        DSharp,
        E,
        F,
        FSharp,
        G,
        GSharp,
        A,
        ASharp,
        B
    };
    
    int key = BaseKey::C;
    int octave = 1;
    int scale = 1;;
    
};
#endif /* SphereData_hpp */
