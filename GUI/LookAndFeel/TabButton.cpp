//
//  TabButtons.cpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 30/08/2016.
//
//

#include "TabButton.hpp"

TabButton::TabButton(String _labelText)
{
    labelText = _labelText.toUpperCase();
    titleFont = GUIFonts::Roboto;
}

TabButton::~TabButton()
{
    
}

void TabButton::paint(Graphics &g)
{
    
   // float toggleDiameter = getHeight() * 0.25;

    
    if (tabToggleState == true) //toggled
    {
        g.fillAll(Colours::grey);
        g.setColour(Colours::black);
    }
    else if (tabToggleState == false) //not toggled
    {
        g.fillAll(Colours::darkgrey);
        g.setColour(Colours::white);

    }
    
    g.setFont(titleFont);

    g.drawFittedText(labelText, 0, 0, getWidth(), getHeight(), Justification::centred, 1);
    
    //(labelText, toggleLightBox.getRight() * 1.25, 0, getWidth(), getHeight(), Justification::centredLeft, 1);

    
    if (lightToggleState == true) //toggled
    {
        g.setColour(Colour(0xff1d837a));
        g.fillEllipse(toggleLightBox);
    }
    else if (lightToggleState == false) //toggled off
    {
        g.setColour(Colours::red);
        g.fillEllipse(toggleLightBox);

    }
}

void TabButton::resized()
{
    float toggleDiameter = getHeight() * 0.25;

    x = getWidth();
    y = getHeight();
    
    toggleLightBox.setBounds(4.0, (toggleDiameter) + (0.5 * toggleDiameter), toggleDiameter, toggleDiameter);
    toggleHitBox.setBounds(0, 0, getWidth()*0.175, getHeight());
    
    toggleLightAlignBox.setBounds(0, 0, y, y);
}

void TabButton::mouseDown(const MouseEvent &event)
{
    static bool wasOnLightToggle;
    wasOnLightToggle = false;
    
    if (event.getMouseDownX() >= 0 && event.getMouseDownX() <= toggleHitBox.getRight())
    {
        if (event.getMouseDownY() >= 0 && event.getMouseDownY() <= getHeight())
        {
            //setLightToggleState(!getLightToggleState());
            listeners.call(&TabButton::Listener::tabLightClicked, this);
            wasOnLightToggle = true;
        }
    }
    
    if (!wasOnLightToggle)
    {
        listeners.call(&TabButton::Listener::tabButtonClicked, this);
        //setTabToggleState(!getTabToggleState());
    }
    
}

bool TabButton::getTabToggleState()
{
    return tabToggleState;
}

bool TabButton::getLightToggleState()
{
    return lightToggleState;
}

void TabButton::setTabToggleState(bool newTabToggleState)
{
    tabToggleState = newTabToggleState;
    repaint();
}

void TabButton::setLightToggleState(bool newLightToggleState)
{
    lightToggleState = newLightToggleState;
    repaint();
}

void TabButton::drawYCentredEllipse (Graphics &g, float x, float y, float radius, float lineThickness)
{
    g.drawEllipse (x, y - radius, 2.0f * radius, 2.0f * radius, lineThickness);
}



void TabButton::addListener(TabButton::Listener* newListener)
{
    listeners.add(newListener);
}

void TabButton::removeListener(TabButton::Listener* listenerToRemove)
{
    listeners.remove(listenerToRemove);
}

