//
//  Toggle Switch.cpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 23/08/2016.
//
//

#include "ToggleSwitch.hpp"

ToggleSwitch::ToggleSwitch(String _labelText, String _option1, String _option2)
{
    labelText = _labelText.toUpperCase();
    option1 = _option1.toUpperCase();
    option2 = _option2.toUpperCase();
    
    titleFont = optionFont = GUIFonts::Roboto;
    
    //addMouseListener(this, false);
}

ToggleSwitch::~ToggleSwitch()
{
    
}

void ToggleSwitch::resized()
{
    x = getWidth();
    y = getHeight();
    
    titleBox.setBounds(5, 5, x/2, y/4);
    titleFont.setHeight(y/4);
    optionFont.setHeight(y/3.5);
    
    option1Box.setBounds(0, y/2.4, x/3, y/2);
    switchBox.setBounds(option1Box.getRight(), y/2.4, x/3, y/2);
    option2Box.setBounds(switchBox.getRight(), y/2.4, x/3, y/2);
    
    switchThrowBox.setBounds(switchBox.getX()+2, switchBox.getY()+2, (switchBox.getWidth()/2)-4, switchBox.getHeight()-4);
    
}

void ToggleSwitch::paint(Graphics &g)
{
    g.fillAll(GUIColours::PanelBackground);
    g.setColour(Colours::white);
    g.setFont(titleFont);
    g.drawFittedText(labelText, titleBox, Justification::topLeft, 1);
    
    g.setFont(optionFont);
    g.drawFittedText(option1, option1Box, Justification::centredRight, 1);
    g.drawFittedText(option2, option2Box, Justification::centredLeft, 1);
    g.setColour(Colours::darkgrey);
    g.fillRect(switchBox);
    
    g.setColour(GUIColours::AlphaGreen);
    g.drawRect(1, 1, getWidth()-2, getHeight()-2, 1);
    //g.setColour(Colours::slategrey);
    //g.fillRect(switchThrowBox.withPosition(switchThrowBox.getX()+2, switchThrowBox.getY()+2));

    g.setColour(Colours::black);
    if (toggleState)
    {
        g.fillRect(switchThrowBox.translated(switchThrowBox.getWidth()+4, 0));
    }
    else
    {
        g.fillRect(switchThrowBox);
    }
    

}

void ToggleSwitch::mouseDown (const MouseEvent& event)
{
    int mouseDownX = event.getMouseDownPosition().getX();
    int mouseDownY = event.getMouseDownPosition().getY();

    
    if (mouseDownX < switchBox.getX() && mouseDownX >= 0)
    {
        //to select option 1
        setToggleState(false);
    }
    else if (mouseDownX >= switchBox.getX() && mouseDownX <= switchBox.getRight())
    {
        if (mouseDownY >= switchBox.getY() && mouseDownY <= switchBox.getBottom())
       {
            //toggle between options
            setToggleState(!toggleState);
        }
    }
    else if (mouseDownX > switchBox.getRight() && mouseDownX <= getWidth())
    {
        //to select option 2
        setToggleState(true);
    }
}

void ToggleSwitch::addListener(ToggleSwitch::Listener* listenerToAdd)
{
    toggleListeners.add(listenerToAdd);
}

void ToggleSwitch::removeListener(ToggleSwitch::Listener* listenerToRemove)
{
    toggleListeners.remove(listenerToRemove);
}

bool ToggleSwitch::getToggleState()
{
    return toggleState;
}

void ToggleSwitch::setToggleState(const bool newState, const bool notifyListeners)
{
    if (toggleState != newState)
    {
        toggleState = newState;
        repaint();
        if (notifyListeners)
        {
            toggleListeners.call(&ToggleSwitch::Listener::toggleSwitchChanged, this);
        }
    }
    
}
