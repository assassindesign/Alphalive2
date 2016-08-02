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
    
    SphereData(int numPads = 48, const int _sphereID = -1);
    ~SphereData();
    
    ValueTree* toValueTree();
    
    const int getSphereID();
    
    PadData* getPadData(const int forPad);
    
    const int getNumPadDataObjects();
    
    TempoData* getTempoData();
    ScaleData* getScaleData();
    
private:
    OwnedArray<PadData> padData;
    int sphereID = -1;
    SphereType sphereType = Empty;
    
    TempoData tempoData;
    ScaleData scaleData;
};





#endif /* SphereData_hpp */
