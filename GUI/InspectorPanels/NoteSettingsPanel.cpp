//
//  NoteSettingsPanel.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 11/08/2016.
//
//

#include "NoteSettingsPanel.hpp"


NoteSettingsPanel::NoteSettingsPanel()
{
    singleNoteBtn.setButtonText("Single");
    singleNoteBtn.addListener(this);
    singleNoteBtn.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));
    addAndMakeVisible(singleNoteBtn);
    
    multiNoteBtn.setButtonText("Multi");
    multiNoteBtn.addListener(this);
    multiNoteBtn.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));
    addAndMakeVisible(multiNoteBtn);
    
    standardTriggerBtn.setButtonText("Standard");
    standardTriggerBtn.addListener(this);
    standardTriggerBtn.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));
    addAndMakeVisible(standardTriggerBtn);
    
    toggleTriggerBtn.setButtonText("Toggle");
    toggleTriggerBtn.addListener(this);
    toggleTriggerBtn.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));
    addAndMakeVisible(toggleTriggerBtn);
    
    noteSelectPanel = new NoteSelectPanel();
    noteSelectPanel->setDataObject(getDataObject());
    addAndMakeVisible(noteSelectPanel);
    

}
NoteSettingsPanel::~NoteSettingsPanel()
{

}

void NoteSettingsPanel::resized()
{
    
    float halfWidth = getWidth()/2.0;
    
    singleNoteBtn.setBounds(0, 0, halfWidth, 20);
    multiNoteBtn.setBounds(singleNoteBtn.getBounds().translated(halfWidth, 0));

    standardTriggerBtn.setBounds(singleNoteBtn.getBounds().translated(0, singleNoteBtn.getHeight()));
    toggleTriggerBtn.setBounds(multiNoteBtn.getBounds().translated(0, multiNoteBtn.getHeight()));
    
    noteSelectPanel->setBounds(0,standardTriggerBtn.getBottom(), getWidth(), 40);
}

void NoteSettingsPanel::paint(Graphics& g)
{
    //g.fillAll(Colours::blue);
}

void NoteSettingsPanel::refreshData()
{
    noteSelectPanel->setDataObject(getDataObject());
    
    padDataChangeCallback(PadData::DataIDs::PadMidiFunction);
    padDataChangeCallback(PadData::DataIDs::NoteTriggerMode);
}

void NoteSettingsPanel::padDataChangeCallback(const int changedData)
{
    padData = AppData::Instance()->getCurrentlyInspectingPadPtr();
    if (changedData == PadData::DataIDs::PadMidiFunction)
    {
        if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::SingleNote)
        {
            singleNoteBtn.setToggleState(true, dontSendNotification);
            multiNoteBtn.setToggleState(false, dontSendNotification);
        }
        else if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::MultiNote)
        {
            singleNoteBtn.setToggleState(false, dontSendNotification);
            multiNoteBtn.setToggleState(true, dontSendNotification);
        }
    }
    else if (changedData == PadData::DataIDs::NoteTriggerMode)
    {
        if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::StandardNoteMode)
        {
            standardTriggerBtn.setToggleState(true, dontSendNotification);
            toggleTriggerBtn.setToggleState(false, dontSendNotification);
        }
        else if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::ToggleNoteMode)
        {
            standardTriggerBtn.setToggleState(false, dontSendNotification);
            toggleTriggerBtn.setToggleState(true, dontSendNotification);
        }
    }
}


void NoteSettingsPanel::buttonClicked (Button* button)
{
    padData = AppData::Instance()->getCurrentlyInspectingPadPtr();
    if (button == &singleNoteBtn)
    {
        padData->setPadMidiFunction(PadData::PadMidiFunctions::SingleNote);
    }
    else if (button == &multiNoteBtn)
    {
        padData->setPadMidiFunction(PadData::PadMidiFunctions::MultiNote);
    }
    else if (button == &standardTriggerBtn)
    {
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::StandardNoteMode);
    }
    else if (button == &toggleTriggerBtn)
    {
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::ToggleNoteMode);
    }
    
   
}