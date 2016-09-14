//
//  Toggle Switch.hpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 23/08/2016.
//
//

#ifndef ToggleSwitch_hpp
#define ToggleSwitch_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

class ToggleSwitch : public Component
{
public:
    
    class Listener
    {
    public:
        Listener(){}
        virtual ~Listener(){}
        
        virtual void toggleSwitchChanged(const ToggleSwitch* toggle) = 0;
    };
    
    ToggleSwitch(String _labelText, String _option1, String _option2);
    ~ToggleSwitch();
    
    void resized() override;
    void paint(Graphics &g) override;
    
    void mouseDown (const MouseEvent& event) override;
    void addListener (ToggleSwitch::Listener* listenerToAdd);
    void removeListener (ToggleSwitch::Listener* listenerToRemove);
    bool getToggleState();
    void setToggleState(const bool newState, const bool notifyListeners = true);
    
private:
    String labelText, option1, option2;
    Rectangle<int> titleBox, option1Box, option2Box, switchBox, switchThrowBox;
    Font titleFont, optionFont;
    float x, y;
    bool toggleState = 0;
    ListenerList<Listener> toggleListeners;

};

#endif /* ToggleSwitch_hpp */
