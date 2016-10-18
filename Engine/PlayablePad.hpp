//
//  PlayablePad.hpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
//
//

#ifndef PlayablePad_hpp
#define PlayablePad_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "InternalMidiRouter.hpp"
/**
   Holds information on what each pad of an alphasphere is set to do. In future, this class will return audio as well as midi information. ALearn doesn't require this.
 */

class PlayablePad
{
    
public:
    PlayablePad(PadData* dataForPad);
    ~PlayablePad();
    
    void hitPad(const int velocity, const bool killingPad = false);
    void pressPad(const float pressure, const bool killingPad = false);
    
    bool setMidiNote(const int newMidiNote);
    const int getMidiNote();
    
    void setMidiChannel(const int newMidiChannel);
    const int getMidiChannel();
    
    void killPad();
    
private:    
    PadData* padData;
    InternalMidiRouter* router;
    
    
};

#endif /* PlayablePad_hpp */
