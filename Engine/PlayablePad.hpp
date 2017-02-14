//
//  PlayablePad.hpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
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

#ifndef PlayablePad_hpp
#define PlayablePad_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "InternalMidiRouter.hpp"
#include "MasterClock.hpp"

/*
    Main functionality class. Acts as a virtul representation of a pad on and Alphasphere.
    
    Relies on a PadData object to store variables, actually turns pad presses into audio/midi
    data. Currently only performing masic midi functions, but most of the variables for more 
    complex functionality should already be present in the PadData class so it should be really
    straightforward to make cool things happen.
 
    See rawClockCallback() for beginnings of MasterClock integration.
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
