//
//  ButtonGrid.cpp
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

#include "ButtonGrid.hpp"

//==========================================================================
//==========================================================================

ButtonGrid::ButtonGridButton::ButtonGridButton(int buttonNumber, Colour _buttonToggledColour)
{
    buttonNumberString = String(buttonNumber);
    buttonToggledColour = _buttonToggledColour;
    buttonOffColour = Colour(0xff4d4d4d);
    
}

void ButtonGrid::ButtonGridButton::paint(juce::Graphics &g)
{
    if (toggleState == true)
    {
        g.fillAll(buttonToggledColour);
        g.setColour(Colours::black);

    }
    else if (toggleState == false)
    {
        g.fillAll(buttonOffColour);
        g.setColour(Colours::white);
    }
    
    g.drawFittedText(buttonNumberString, getLocalBounds(), Justification::centred, 1);

}

void ButtonGrid::ButtonGridButton::resized()
{
    
}

void ButtonGrid::ButtonGridButton::setToggleState(bool newToggleState)
{
    toggleState = newToggleState;
    repaint();
}

bool ButtonGrid::ButtonGridButton::getToggleState()
{
    return toggleState;
}

//==========================================================================
//==========================================================================


ButtonGrid::ButtonGrid(int numButtons, String _labelText, bool showTopButton, String topButtonText)
{
    labelText = _labelText.toUpperCase();
    titleFont = GUIFonts::getMainFont();
    //titleFont.setHeight(12.0);
    
    isExclusive = true;
    
    for (int i = 0; i < numButtons; i++)
    {
        gridButtons.add(new ButtonGridButton(i + 1, GUIColours::AlphaGreen));
        gridButtons.getLast()->addMouseListener(this, true);
        addAndMakeVisible(gridButtons.getLast());
    }
    
    topRightButton = new ToggleButton(topButtonText);
    if (showTopButton)
    {
        addAndMakeVisible(topRightButton);
    }

}

ButtonGrid::~ButtonGrid()
{
    
}

void ButtonGrid::resized()
{
    x = getWidth();
    y = getHeight();
    
    static float segments, segmentWidth, vertPadding, horPadding;
    segments = gridButtons.size()+2;
    segmentWidth = getWidth()/segments;
    
    titleBox.setBounds(4, 4, x/2, y/4);
    
    vertPadding = titleBox.getBottom() + (getHeight()-titleBox.getBottom() - segmentWidth) * 0.5;
    horPadding = segmentWidth;
    gridButtons[0]->setBounds(segmentWidth*0.5, vertPadding, segmentWidth, segmentWidth);
    for (int i = 1; i < gridButtons.size(); i++)
    {
        //gridButtons[i]->setBounds(segmentWidth + (segmentWidth*1.125*i) + (segmentWidth/segments), vertPadding, segmentWidth, segmentWidth);
        gridButtons[i]->setBounds(gridButtons[i-1]->getBounds().translated(segmentWidth * (1.0+(1.3/gridButtons.size())), 0));
    }
    
    topRightButton->setBounds(x - 70, 0, 70, 20);
    
}

void ButtonGrid::paint(Graphics &g)
{
    g.fillAll(GUIColours::PanelBackground);
    
    g.setColour(GUIColours::AlphaGreen);
    g.drawRect(1, 1, getWidth()-2, getHeight()-2);
    g.setColour(Colours::whitesmoke);
    g.setFont(titleFont);
    g.drawFittedText(labelText, titleBox, Justification::topLeft, 1);
}

void ButtonGrid::mouseDown(const MouseEvent &event)
{
    if (event.eventComponent != this)
    {
        for (int i = 0; i < gridButtons.size(); i++)
        {
            if (event.eventComponent == gridButtons[i])
            {
                listeners.call(&ButtonGrid::Listener::buttonGridCallback, this, i);
                break;
            }
            else if (event.eventComponent == topRightButton)
            {
                listeners.call(&ButtonGrid::Listener::buttonGridCallback, this, -1);
            }
        }
    }
}

void ButtonGrid::addListener(ButtonGrid::Listener* newListener)
{
    listeners.add(newListener);
}

void ButtonGrid::removeListener(ButtonGrid::Listener* listenerToRemove)
{
    listeners.remove(listenerToRemove);
}


bool ButtonGrid::setButtonSelected(const int buttonIndex, const bool isSelected)
{
    if (buttonIndex > -1 && buttonIndex < gridButtons.size())
    {
        
        if (isSelected && isExclusive)
        {
            for (int i = 0; i < gridButtons.size(); i++)
            {
                if (i != buttonIndex)
                {
                    gridButtons[i]->setToggleState(false);
                }
            }
        }
        
        gridButtons[buttonIndex]->setToggleState(isSelected);

        
        return true;
    }
    else
    {
        return false;
    }
    
}

bool ButtonGrid::getIsButtonSelected(const int buttonIndex)
{
    if (buttonIndex > -1 && buttonIndex < gridButtons.size())
    {
        return gridButtons[buttonIndex]->getToggleState();
    }
    else
    {
        return false;
    }

}

void ButtonGrid::setIsExclusive(const bool shouldBeExclusive)
{
    isExclusive = shouldBeExclusive;
}

bool ButtonGrid::getIsExclusive()
{
    return isExclusive;
}

void ButtonGrid::setToggleButtonState(const bool newState)
{
    toggleButtonState = newState;
}

bool ButtonGrid::getToggleButtonState()
{
    return toggleButtonState;
}

