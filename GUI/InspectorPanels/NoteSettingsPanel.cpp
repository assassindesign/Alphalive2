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
    
    multiNotePanel = new MultiNotePanel();
    multiNotePanel->setDataObject(getDataObject());
    addAndMakeVisible(multiNotePanel);
    
    noteSelectKeyboard = new NoteSelectKBComponent();
    //addAndMakeVisible(noteSelectKeyboard);
    
    noteSelectViewPort = new Viewport();
    noteSelectViewPort->setViewedComponent(noteSelectKeyboard);
    noteSelectViewPort->setScrollBarsShown(false, true);
    addAndMakeVisible(noteSelectViewPort);

}
NoteSettingsPanel::~NoteSettingsPanel()
{
    //noteSelectViewPort->setViewedComponent(nullptr);
}

void NoteSettingsPanel::resized()
{
    
    float halfWidth = getWidth()/2.0;
    float thirdHeight = getHeight()/3.0;
    
    standardTriggerBtn.setBounds(0, 0, halfWidth, 20);
    toggleTriggerBtn.setBounds(standardTriggerBtn.getBounds().translated(halfWidth, 0));
    
    singleNoteBtn.setBounds(standardTriggerBtn.getBounds().translated(0, standardTriggerBtn.getHeight()));
    multiNoteBtn.setBounds(singleNoteBtn.getBounds().translated(singleNoteBtn.getWidth(), 0));
    
    //multiNotePanel->setBounds(0,singleNoteBtn.getBottom()+5, getWidth(), 20);
    
    
    noteSelectViewPort->setBounds(0, multiNoteBtn.getBottom() + 5, getWidth(), thirdHeight+noteSelectViewPort->getScrollBarThickness());

    noteSelectKeyboard->setBounds(0, multiNoteBtn.getBottom() + 5, noteSelectKeyboard->getWidthNeeded(thirdHeight), thirdHeight);
}

void NoteSettingsPanel::paint(Graphics& g)
{
    //g.fillAll(Colours::blue);
}

void NoteSettingsPanel::refreshData()
{
    noteSelectPanel->setDataObject(getDataObject());
    noteSelectKeyboard->setDataObject(getDataObject());
    
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