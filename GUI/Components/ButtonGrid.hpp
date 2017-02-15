//
//  ButtonGrid.hpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 30/08/2016.
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

#ifndef ButtonGrid_hpp
#define ButtonGrid_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "GUIStyle.h"

/* 
    Fairly generic GUI Class that contains a title and a number of buttons that can 
    be toggled.
 */

class ButtonGrid : public Component
{
public:

//==========================================================================
//==========================================================================

    class ButtonGridButton : public Component
    {
    public:
        ButtonGridButton(int buttonNumber, Colour buttonToggledColour);
        ~ButtonGridButton(){}
        
        void paint(Graphics &g) override;
        void resized() override;
        bool getToggleState();
        void setToggleState(bool newToggleState);
        
    private:
        String buttonNumberString;
        Colour buttonToggledColour, buttonOffColour;
        bool toggleState = 0;

    };
    
//==========================================================================
//==========================================================================

    class Listener{
    public:
        virtual ~Listener(){};
        virtual void buttonGridCallback(ButtonGrid* grid,
                                        const int buttonID) = 0;
    };
    
    ButtonGrid(int numButtons, String _labelText, bool showTopButton = false, String topButtonText = "");
    ~ButtonGrid();
    
    void resized() override;
    void paint(Graphics &g) override;
    void mouseDown (const MouseEvent &event) override;
    
    void addListener(ButtonGrid::Listener* newListener);
    void removeListener(ButtonGrid::Listener* listenerToRemove);
    
    bool setButtonSelected(const int buttonIndex, const bool isSelected);
    bool getIsButtonSelected(const int buttonIndex);
    
    void setIsExclusive(const bool shouldBeExclusive);
    bool getIsExclusive();
    
    void setToggleButtonState(const bool newState);
    bool getToggleButtonState();
    
private:
    String labelText;
    Rectangle<int> titleBox;
    Font titleFont;
    float x, y;
    OwnedArray<ButtonGridButton> gridButtons;
    ScopedPointer<ToggleButton> topRightButton;
    
    bool isExclusive, toggleButtonState;
    ListenerList<ButtonGrid::Listener> listeners;
};

#endif /* ButtonGrid_hpp */
