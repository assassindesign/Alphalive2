//
//  ButtonGrid.cpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 30/08/2016.
//
//

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


ButtonGrid::ButtonGrid(int numRows, String _labelText)
{
    labelText = _labelText.toUpperCase();
    titleFont = GUIFonts::Roboto;
    titleFont.setHeight(11.0);
    
    for (int i = 0; i < numRows * BUTTONS_PER_ROW; i++)
    {
        gridButtons.add(new ButtonGridButton(i + 1, Colour(0xff1d837a)));
        gridButtons.getLast()->addMouseListener(this, true);
        addAndMakeVisible(gridButtons.getLast());
    }
}

ButtonGrid::~ButtonGrid()
{
    
}

void ButtonGrid::resized()
{
    x = getWidth();
    y = getHeight();
    
    titleBox.setBounds(2, 2, x/2, y/4);
    titleFont.setHeight(y/5);
    
    gridButtons[0]->setBounds(titleBox.getX(), titleBox.getBottom()*1.05, getWidth()*0.1, getWidth()*0.1);
    for (int i = 1; i < gridButtons.size(); i++)
    {
        gridButtons[i]->setBounds(gridButtons[i-1]->getBounds().translated(getWidth()*0.125, 0));
    }
}

void ButtonGrid::paint(Graphics &g)
{
    //g.fillAll(Colours::hotpink);
    
    g.setColour(Colours::black);
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
                gridButtons[i]->setToggleState(!gridButtons[i]->getToggleState());
        }
    }
}
