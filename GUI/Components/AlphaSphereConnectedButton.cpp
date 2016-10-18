//
//  AlphaSphereConnectedButton.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 28/09/2016.
//
//

#include "AlphasphereConnectedButton.hpp"
#include "GUIStyle.h"
#include "Alphalive2Engine.hpp"

AlphaSphereConnectedButton::AlphaSphereConnectedButton()
{
    AppData::Instance()->addListener(this);
}

AlphaSphereConnectedButton::~AlphaSphereConnectedButton()
{
    
}


void AlphaSphereConnectedButton::paint(Graphics& g)
{
    g.fillAll(GUIColours::PanelBackground);
    g.setColour(GUIColours::MainBlue);
    g.drawRect(1, 1, getWidth()-2, getHeight()-2);
    
    if (alphaSphereConnected)
    {
        g.setColour(Colours::darkblue);
    }
    else
    {
        g.setColour(Colours::black);
    }
    
    g.fillRect(3, 0, getWidth()-6, getHeight()-3);
}

void AlphaSphereConnectedButton::resized ()
{
    
}

void AlphaSphereConnectedButton::mouseDown(const MouseEvent& event)
{
    if (alphaSphereConnected)
    {
        String instructionString = translate("AlphaLive has detected an AlphaSphere with no issues connected to your computer.");
        AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::NoIcon,
                                          translate("AlphaSphere Connected."),
                                          translate(instructionString));
    }
    else
    {
        String instructionString = translate("AlphaLive cannot detect an AlphaSphere connected to your computer.");
        AlertWindow::showMessageBoxAsync (AlertWindow::NoIcon,
                                          translate("No AlphaShere Connected"),
                                          translate(instructionString));
    }
}

void AlphaSphereConnectedButton::appDataChangeCallback(const int changedData)
{
    if (changedData == AppData::DataIDs::HIDSphereConnected)
    {
        alphaSphereConnected = AppData::Instance()->getEnginePointer()->getHIDSphereConnected();
        repaint();
    }
}
