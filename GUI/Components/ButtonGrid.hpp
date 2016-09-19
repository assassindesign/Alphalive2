//
//  ButtonGrid.hpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 30/08/2016.
//
//

#ifndef ButtonGrid_hpp
#define ButtonGrid_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "GUIStyle.h"

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
    
    ButtonGrid(int numRows, String _labelText);
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
    
private:
    String labelText;
    Rectangle<int> titleBox;
    Font titleFont;
    float x, y;
    OwnedArray<ButtonGridButton> gridButtons;
    bool isExclusive;
    ListenerList<ButtonGrid::Listener> listeners;
};

#endif /* ButtonGrid_hpp */
