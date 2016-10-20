//
//  MidiPressureSettingsPanel.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 20/09/2016.
//
//

#include "MidiPressureSettingsPanel.hpp"


MidiPressureSettingsPanel::MidiPressureSettingsPanel()
{
    
}

MidiPressureSettingsPanel::~MidiPressureSettingsPanel()
{
    
}

void MidiPressureSettingsPanel::refreshData()
{
    
}

void MidiPressureSettingsPanel::paint(Graphics &g)
{
    g.fillAll(GUIColours::Background);
    g.setColour(GUIColours::PanelBackground);
    g.fillRect(destinationsBox);
    g.fillRect(rangeBox);
    g.fillRect(settingsBox);
    
    g.setColour(GUIColours::AlphaGreen);
    g.drawRect(destinationsBox, 1);
    g.drawRect(rangeBox, 1);
    g.drawRect(settingsBox, 1);
    
    g.setColour(Colours::white);
    g.setFont(GUIFonts::Roboto);
    g.drawSingleLineText("DESTINATION", destinationsBox.getX()+5, destinationsBox.getY()+15);
    g.drawSingleLineText("RANGE", rangeBox.getX()+5, rangeBox.getY()+15);
    g.drawSingleLineText("SETTINGS", settingsBox.getX()+5, settingsBox.getY()+15);


    
    
}

void MidiPressureSettingsPanel::resized()
{
    destinationsBox.setBounds(0, 0, getWidth(), 50);
    rangeBox = settingsBox = destinationsBox;
    rangeBox.translate(0, 51);
    settingsBox.translate(0, 102);
}

void MidiPressureSettingsPanel::padDataChangeCallback(const int changedData)
{
    
}
