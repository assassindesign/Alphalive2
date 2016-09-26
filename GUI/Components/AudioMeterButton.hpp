//
//  AudioMeterButton.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 23/09/2016.
//
//

#ifndef AudioMeterButton_hpp
#define AudioMeterButton_hpp

#include "AppData.hpp"
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
