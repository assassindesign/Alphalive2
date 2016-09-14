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
    noteButton = new TabButton("Note");
    noteButton->addListener(this);
    noteButton->setTabToggleState(true);
    addAndMakeVisible(noteButton);

//    noteButton.addListener(this);
//    noteButton.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));
    
    pressureButton = new TabButton("Pressure");
    pressureButton->addListener(this);
    addAndMakeVisible(pressureButton);
    
    noteSettingsPanel = new NoteSettingsPanel();
    addAndMakeVisible(noteSettingsPanel);
}
MidiIPanel::~MidiIPanel()
{
    
}

void MidiIPanel::resized()
{
    static float thirdWidth;
    thirdWidth = getWidth()/3.0;
    
    static int borderPadding = 10;
    
    noteButton->setBounds(thirdWidth*0.5, 0, thirdWidth, 20);
    pressureButton->setBounds(noteButton->getBounds().translated(thirdWidth, 0));
    
    noteSettingsPanel->setBounds(borderPadding, noteButton->getBottom(), getWidth() - borderPadding*2, getHeight() - noteButton->getHeight());
    
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
    padData = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
    
    padDataChangeCallback(PadData::DataIDs::NoteEnabled);
    padDataChangeCallback(PadData::DataIDs::PressureEnabled);
    
    
}


void MidiIPanel::padDataChangeCallback(const int changedData)
{
    if (changedData == PadData::DataIDs::NoteEnabled)
    {
        noteButton->setLightToggleState(padData->getNoteEnabled());
    }
    else if (changedData == PadData::DataIDs::PressureEnabled)
    {
        pressureButton->setLightToggleState(padData->getPressureEnabled());
    }
}

void MidiIPanel::tabButtonClicked(const TabButton* button)
{
    if (button == noteButton)
    {
        noteSettingsPanel->setVisible(true);
        noteButton->setTabToggleState(true);
        pressureButton->setTabToggleState(false);

    }
    else if (button == pressureButton)
    {
        noteSettingsPanel->setVisible(false);
        noteButton->setTabToggleState(false);
        pressureButton->setTabToggleState(true);
    }
}

void MidiIPanel::tabLightClicked(const TabButton* button)
{
    if (button == noteButton)
    {
        
        if (padData->getNoteEnabled() == true && padData->getVelocity() > 0)
        {
            AppData::Instance()->getEnginePointer()->
                getSpherePointer(padData->getParentSphere()->getSphereID())->hitPad(padData->getPadID(), 0);
        }
        
        padData->setNoteEnabled(!padData->getNoteEnabled());

    }
    else if (button == pressureButton)
    {
        
        if (padData->getPressureEnabled() == true && padData->getPadPressure() > 0)
        {
            AppData::Instance()->getEnginePointer()->
                getSpherePointer(padData->getParentSphere()->getSphereID())->pressPad(padData->getPadID(), 0);
            padData->setPressureEnabled(!padData->getPressureEnabled());

        }
        else
        {
            padData->setPressureEnabled(!padData->getPressureEnabled());

        }
        
    }
}


