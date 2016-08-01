//
//  AppDataTypes.h
//  AlphaLive Midi
//
//  Created by Luke Saxton on 16/06/2016.
//
//

#ifndef AppDataTypes_h
#define AppDataTypes_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppDataListeners.h"

/*
    Catch-all file for AppDataTypes not big enough to warrant their own files
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
    
    const int getKey()
    {
        return key;
    }
    
    const int getOctave()
    {
        return octave;
    }
    
    const int getScale()
    {
        return scale;
    }
    
    //==SETS=======================================================
    
    bool setKey(const int newKey)
    {
        bool success = true;
        dataLock.enter();
        if (newKey != key)
        {
            if (newKey > -1 && newKey < BaseKey::B)
                key = newKey;
            else
                success = false;
            
        }
        else{
            success = false;
        }
        dataLock.exit();
        
        if(success)
            callListeners(DataIDs::Key);
        
        return success;
    }
    
    bool setOctave(const int newOctave)
    {
        bool success = true;
        dataLock.enter();
        if (newOctave != octave)
        {
            if (newOctave > -3 && newOctave < 6)
                octave = newOctave;
            else
                success = false;
            
        }
        else
        {
            success = false;
        }
        
        dataLock.exit();
        
        if (success)
            callListeners(DataIDs::Key);
        
        return success;
    }
    
    bool setScale(const int newScale)
    {
        bool success = true;
        dataLock.enter();
        if (newScale != scale)
        {
            scale = newScale;
        }
        else
        {
            jassertfalse; //value out of range
            success = false;
        }
        dataLock.exit();
        
        if (success)
            callListeners(DataIDs::Key);
        
        return success;
    }
    
private:
    
    int key = BaseKey::C;
    int octave = 1;
    int scale = 1;;
    CriticalSection dataLock;
};


#endif /* AppDataTypes_h */
