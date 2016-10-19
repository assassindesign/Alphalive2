//
//  MidiPanel.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
//
//

#include "MidiIPanel.hpp"

MidiIPanel::MidiIPanel()
{
    noteButton = new TabButton("Note");
    noteButton->addListener(this);
    noteButton->setTabToggleState(true);
    addAndMakeVisible(noteButton);

//    noteButton.addListener(this);
//    noteButton.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::AlphaGreen));
    
    pressureButton = new TabButton("Pressure");
    pressureButton->addListener(this);
    addAndMakeVisible(pressureButton);
    
    midiNoteSettingsPanel = new MidiNoteSettingsPanel();
    addAndMakeVisible(midiNoteSettingsPanel);
    
    midiPressureSettingsPanel = new MidiPressureSettingsPanel();
    addChildComponent(midiPressureSettingsPanel);
}
MidiIPanel::~MidiIPanel()
{
    
}

void MidiIPanel::resized()
{
    static float thirdWidth, twelthHeight;
    thirdWidth = getWidth()/3.0;
    twelthHeight = getHeight()/12.0;
    
    //static int borderPadding = 10;
    
    noteButton->setBounds(thirdWidth*0.5, 0, thirdWidth, 25);
    pressureButton->setBounds(noteButton->getBounds().translated(thirdWidth, 0));
    
    midiNoteSettingsPanel->setBounds(0, noteButton->getBottom(), getWidth(), getHeight() - noteButton->getHeight());
    midiPressureSettingsPanel->setBounds(midiNoteSettingsPanel->getBounds());
    
}
void MidiIPanel::paint(Graphics& g)
{
    //g.fillAll(Colours::slategrey);
}

void MidiIPanel::refreshData()
{
    midiNoteSettingsPanel->setDataObject(getDataObject());
    midiPressureSettingsPanel->setDataObject(getDataObject());

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
        midiNoteSettingsPanel->setVisible(true);
        midiPressureSettingsPanel->setVisible(false);
        noteButton->setTabToggleState(true);
        pressureButton->setTabToggleState(false);

    }
    else if (button == pressureButton)
    {
        midiNoteSettingsPanel->setVisible(false);
        midiPressureSettingsPanel->setVisible(true);
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
                getSpherePointer(padData->getParentSphereID())->hitPad(padData->getPadID(), 0);
        }
        
        padData->setNoteEnabled(!padData->getNoteEnabled());

    }
    else if (button == pressureButton)
    {
        
        if (padData->getPressureEnabled() == true && padData->getPadPressure() > 0)
        {
            AppData::Instance()->getEnginePointer()->
                getSpherePointer(padData->getParentSphereID())->pressPad(padData->getPadID(), 0);
            padData->setPressureEnabled(!padData->getPressureEnabled());

        }
        else
        {
            padData->setPressureEnabled(!padData->getPressureEnabled());

        }
        
    }
}


