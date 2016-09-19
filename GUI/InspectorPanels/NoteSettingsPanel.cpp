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
    
    noteModeSwitch = new ToggleSwitch("Note Mode", "Single", "Multi");
    noteModeSwitch->addListener(this);
    addAndMakeVisible(noteModeSwitch);
    
    triggerModeSwitch = new ToggleSwitch("Trigger Mode", "Standard", "Toggle");
    triggerModeSwitch->addListener(this);
    addAndMakeVisible(triggerModeSwitch);
    
    
    noteSelectKeyboard = new NoteSelectKBComponent();
    addAndMakeVisible(noteSelectKeyboard);
    noteSelectKeyboard->setDataObject(getDataObject());

    channelSwitcher = new ButtonGrid(16, "MIDI Channel");
    channelSwitcher->addListener(this);
    addAndMakeVisible(channelSwitcher);
    
    groupSelector = new ButtonGrid(8, "SEND TO GROUP:");
    groupSelector->addListener(this);
    addAndMakeVisible(groupSelector);
}
NoteSettingsPanel::~NoteSettingsPanel()
{

}

void NoteSettingsPanel::resized()
{
    
    static float halfWidth;
    halfWidth = getWidth()/2.0;
    //float thirdHeight = getHeight()/3.0;
    
    
    noteModeSwitch->setBounds(0, 0, halfWidth, 50);
    triggerModeSwitch->setBounds(noteModeSwitch->getBounds().translated(halfWidth, 0));

    channelSwitcher->setBounds(0, noteModeSwitch->getBottom(), getWidth(), 50);
    
    noteSelectKeyboard->setBounds(0, channelSwitcher->getBottom() + 5, getWidth(), 120);
    
    groupSelector->setBounds(0, getHeight() - 60, getWidth(), 60);
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
    padDataChangeCallback(PadData::DataIDs::MidiChannel);
}

void NoteSettingsPanel::padDataChangeCallback(const int changedData)
{
    padData = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
    if (changedData == PadData::DataIDs::PadMidiFunction)
    {
        if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::SingleNote)
        {
            noteModeSwitch->setToggleState(false);
        }
        else if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::MultiNote)
        {
            noteModeSwitch->setToggleState(true);
        }
    }
    else if (changedData == PadData::DataIDs::NoteTriggerMode)
    {
        if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::StandardNoteMode)
        {
            triggerModeSwitch->setToggleState(false);
        }
        else if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::ToggleNoteMode)
        {
            triggerModeSwitch->setToggleState(true);
        }
    }
    else if (changedData == PadData::DataIDs::MidiChannel)
    {
        channelSwitcher->setButtonSelected(padData->getMidiChannel()-1, true);
    }
}



void NoteSettingsPanel::toggleSwitchChanged(const ToggleSwitch* toggle)
{
    padData = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
    if (toggle == noteModeSwitch)
    {
        if (!noteModeSwitch->getToggleState()) // single note mode
        {
            AppData::Instance()->getEnginePointer()->getSpherePointer(padData->getParentSphere()->getSphereID())->killPad(padData->getPadID());
            padData->setPadMidiFunction(PadData::PadMidiFunctions::SingleNote);
        }
        else //multi note mode
        {
            AppData::Instance()->getEnginePointer()->getSpherePointer(padData->getParentSphere()->getSphereID())->killPad(padData->getPadID());
            padData->setPadMidiFunction(PadData::PadMidiFunctions::MultiNote);
        }
        
    }
    else if (toggle == triggerModeSwitch)
    {
        if (!triggerModeSwitch->getToggleState()) //standard mode
        {
            AppData::Instance()->getEnginePointer()->getSpherePointer(padData->getParentSphere()->getSphereID())->killPad(padData->getPadID());
            padData->setNoteTriggerMode(PadData::NoteTriggerModes::StandardNoteMode);
        }
        else //toggle mode
        {
            AppData::Instance()->getEnginePointer()->getSpherePointer(padData->getParentSphere()->getSphereID())->killPad(padData->getPadID());
            padData->setNoteTriggerMode(PadData::NoteTriggerModes::ToggleNoteMode);
        }
        
    }

    
}

void NoteSettingsPanel::buttonGridCallback(ButtonGrid* grid, const int buttonID)
{
    //DBG(buttonID);
    if (grid == channelSwitcher)
    {
        padData->setMidiChannel(buttonID+1);
    }
    else if (grid == groupSelector)
    {
        DBG(buttonID);
    }
    
}
