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
#include "MasterClock.hpp"

/**
   Holds information on what each pad of an alphasphere is set to do. In future, this class will return audio as well as midi information. ALearn doesn't require this.
 */

class PlayablePad : public MasterClock::Listener
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
    void setPadEnabled(const bool enabled);
    
    
    
    
private:
    // Master Clock ======================================================
    void barClockCallback() override;
    void stepClockCallback(const int currentPositionInLoop) override;
    void masterClockStopped() override;
    void masterTempoChanged(const int beatsInLoop, const float newTempo) override;
    void rawClockCallback(const int clock) override;
    
    
    PadData* padData = 0;
    InternalMidiRouter* router = 0;
    MasterClock* masterClock = 0;
    
};

#endif /* PlayablePad_hpp */
