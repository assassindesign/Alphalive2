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
    polyATButton.setButtonText("PolyAT");
    polyATButton.addListener(this);
    polyATButton.setConnectedEdges(3);
    addAndMakeVisible(polyATButton);
    
    channelATButton.setButtonText("ChnlAT");
    channelATButton.addListener(this);
    channelATButton.setConnectedEdges(3);
    addAndMakeVisible(channelATButton);
    
    pbUpButton.setButtonText("PB Up");
    pbUpButton.addListener(this);
    pbUpButton.setConnectedEdges(3);
    addAndMakeVisible(pbUpButton);
    
    pbDownButton.setButtonText("PB Dn");
    pbDownButton.addListener(this);
    pbDownButton.setConnectedEdges(3);
    addAndMakeVisible(pbDownButton);
    
    polyATButton.setButtonText("PolyAT");
    polyATButton.addListener(this);
    polyATButton.setConnectedEdges(3);
    addAndMakeVisible(polyATButton);
    
    polyATButton.setButtonText("PolyAT");
    polyATButton.addListener(this);
    polyATButton.setConnectedEdges(3);
    addAndMakeVisible(polyATButton);
}

MidiPressureSettingsPanel::~MidiPressureSettingsPanel()
{
    
}

void MidiPressureSettingsPanel::refreshData()
{
    
}

void MidiPressureSettingsPanel::padDataChangeCallback(const int changedData)
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

void MidiPressureSettingsPanel::buttonClicked(Button* button)
{
    
}

