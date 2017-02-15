//
//  AudioMeterButton.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 23/09/2016.
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

#ifndef AudioMeterButton_hpp
#define AudioMeterButton_hpp

#include "AppData.hpp"

/* 
    Half-developed class to display audio output levels. Uses a timer to pull 
    averaged values that are calculated in the engine.
 */

class AudioMeterButton : public Timer,
                         public Component
{
public:
    AudioMeterButton();
    ~AudioMeterButton();
    
    void resized() override;
    void paint(Graphics& g) override;
    
    void timerCallback() override;
    
    void setToggleState(const bool newState);
    bool getToggleState();
    
    
    
private:
    double audioAverageL, audioAverageR;
    Rectangle<int> leftChannelBox, rightChannelBox;
    
    Alphalive2Engine* engine;
    
    ListenerList<Button::Listener> listeners;
    
    bool toggleState;
};

#endif /* AudioMeterButton_hpp */
