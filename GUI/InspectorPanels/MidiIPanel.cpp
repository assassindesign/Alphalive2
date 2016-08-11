//
//  MidiPanel.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
//
//

#include "MidiIPanel.hpp"
#include "GUIColours.h"

MidiIPanel::MidiIPanel()
{
    noteButton.setButtonText("NOTE");
    noteButton.addListener(this);
    noteButton.setToggleState(true, dontSendNotification);
    noteButton.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));
    addAndMakeVisible(noteButton);
    
    pressureButton.setButtonText("PRESSURE");
    pressureButton.addListener(this);
    pressureButton.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));

    addAndMakeVisible(pressureButton);
    
    noteSettingsPanel = new NoteSettingsPanel();
    addAndMakeVisible(noteSettingsPanel);
}
MidiIPanel::~MidiIPanel()
{
    
}

void MidiIPanel::resized()
{
    noteButton.setBounds(0, 0, getWidth()/2.0, 20);
    pressureButton.setBounds(noteButton.getBounds().translated(getWidth()/2.0, 0));
    
    noteSettingsPanel->setBounds(0, noteButton.getBottom(), getWidth(), getHeight() - noteButton.getHeight());
    
}
void MidiIPanel::paint(Graphics& g)
{
    //g.fillAll(Colours::blue);
}

void MidiIPanel::refreshData()
{
    noteSettingsPanel->setDataObject(getDataObject());
    noteSettingsPanel->refreshData();
    //noteSettingsPane->setDataObject(getDataObject());

    
}


void MidiIPanel::padDataChangeCallback(const int changedData)
{
    
}

void MidiIPanel::buttonClicked (Button* button)
{
    if (button == &noteButton)
    {
        noteSettingsPanel->setVisible(true);
        noteButton.setToggleState(true, dontSendNotification);
        pressureButton.setToggleState(false, dontSendNotification);

    }
    else if (button == &pressureButton)
    {
        noteSettingsPanel->setVisible(false);
        noteButton.setToggleState(false, dontSendNotification);
        pressureButton.setToggleState(true, dontSendNotification);
    }
}

