    //
//  AppDataTypes.h
//  AlphaLive Midi
//
//  Created by Luke Saxton on 16/06/2016.
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

#ifndef AppDataTypes_h
#define AppDataTypes_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppDataListeners.h"

/*
    Catch-all file for AppDataFormat objects not big enough to warrant their own files yet
*/

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
    
    enum DataIDs{
        Key = 0,
        Octave,
        Scale
    };
    
    //==GETS=======================================================
    
    const int getKey();
    const int getOctave();
    const int getScale();
    
    //==SETS=======================================================
    
    bool setKey(const int newKey);
    bool setOctave(const int newOctave);
    bool setScale(const int newScale);
    
private:
    
    int key = BaseKey::C;
    int octave = 1;
    int scale = 1;;
    CriticalSection dataLock;
};


class TempoData : public AppDataFormat
{
public:
    TempoData(){}
    ~TempoData(){};
    
    void getTempo();
    void getBeatsPerBar();
    
    bool setTempo(const float newTempo);
    bool setBeatsPerBar(const int newBPB);
    
    
private:
    
    float tempo = 120;
    int beatsPerBar = 4;
    CriticalSection dataLock;
};


#endif /* AppDataTypes_h */
