//
//  ButtonGrid.hpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 30/08/2016.
//
//

#ifndef ButtonGrid_hpp
#define ButtonGrid_hpp
#define BUTTONS_PER_ROW 8

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

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

    ButtonGrid(int numRows, String _labelText);
    ~ButtonGrid();
    
    void resized() override;
    void paint(Graphics &g) override;
    void mouseDown (const MouseEvent &event) override;
    
private:
    String labelText;
    Rectangle<int> titleBox;
    Font titleFont;
    float x, y;
    OwnedArray<ButtonGridButton> gridButtons;
};

#endif /* ButtonGrid_hpp */
