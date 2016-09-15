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
    
    
    noteSelectKeyboard = new NoteSelectKBComponent();
    addAndMakeVisible(noteSelectKeyboard);
    noteSelectKeyboard->setDataObject(getDataObject());


}
NoteSettingsPanel::~NoteSettingsPanel()
{

}

void NoteSettingsPanel::resized()
{
    
    float halfWidth = getWidth()/2.0;
    float thirdHeight = getHeight()/3.0;
    
    standardTriggerBtn.setBounds(0, 0, halfWidth, 20);
    toggleTriggerBtn.setBounds(standardTriggerBtn.getBounds().translated(halfWidth, 0));
    
    singleNoteBtn.setBounds(standardTriggerBtn.getBounds().translated(0, standardTriggerBtn.getHeight()));
    multiNoteBtn.setBounds(singleNoteBtn.getBounds().translated(singleNoteBtn.getWidth(), 0));

    noteSelectKeyboard->setBounds(0, multiNoteBtn.getBottom() + 5, getWidth(), thirdHeight);
}

void NoteSettingsPanel::paint(Graphics& g)
{
    //g.fillAll(Colours::blue);
}

void NoteSettingsPanel::refreshData()
{
    noteSelectKeyboard->setDataObject(getDataObject());
    
    padDataChangeCallback(PadData::DataIDs::PadMidiFunction);
    padDataChangeCallback(PadData::DataIDs::NoteTriggerMode);
}

void NoteSettingsPanel::padDataChangeCallback(const int changedData)
{
    padData = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
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
    padData = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
    if (button == &singleNoteBtn)
    {
        AppData::Instance()->getEnginePointer()->getSpherePointer(padData->getParentSphere()->getSphereID())->killPad(padData->getPadID());
        padData->setPadMidiFunction(PadData::PadMidiFunctions::SingleNote);
    }
    else if (button == &multiNoteBtn)
    {
        AppData::Instance()->getEnginePointer()->getSpherePointer(padData->getParentSphere()->getSphereID())->killPad(padData->getPadID());
        padData->setPadMidiFunction(PadData::PadMidiFunctions::MultiNote);
    }
    else if (button == &standardTriggerBtn)
    {
        AppData::Instance()->getEnginePointer()->getSpherePointer(padData->getParentSphere()->getSphereID())->killPad(padData->getPadID());
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::StandardNoteMode);
    }
    else if (button == &toggleTriggerBtn)
    {
        AppData::Instance()->getEnginePointer()->getSpherePointer(padData->getParentSphere()->getSphereID())->killPad(padData->getPadID());
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::ToggleNoteMode);
    }
    
}

void NoteSettingsPanel::toggleSwitchChanged(const ToggleSwitch* toggle)
{
    
    
    
}
