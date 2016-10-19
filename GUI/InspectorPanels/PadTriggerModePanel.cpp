//
//  PadTriggerModePanel.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 27/09/2016.
//
//

#include "PadTriggerModePanel.hpp"
#include "GUIStyle.h"
#include "Alphalive2Engine.hpp"

PadTriggerModePanel::PadTriggerModePanel()
{
    stdBtn.setButtonText("STD");
    stdBtn.addListener(this);
    stdBtn.setConnectedEdges(3);
    addAndMakeVisible(stdBtn);
    
    tglBtn.setButtonText("TGL");
    tglBtn.addListener(this);
    tglBtn.setConnectedEdges(3);
    addAndMakeVisible(tglBtn);
    
    ltchBtn.setButtonText("LTCH");
    ltchBtn.addListener(this);
    ltchBtn.setConnectedEdges(3);
    addAndMakeVisible(ltchBtn);
    
    trgrBtn.setButtonText("TRGR");
    trgrBtn.addListener(this);
    trgrBtn.setConnectedEdges(3);
    addAndMakeVisible(trgrBtn);
    
    font = GUIFonts::Roboto.withHeight(8);
    
}

PadTriggerModePanel::~PadTriggerModePanel()
{
    
}

void PadTriggerModePanel::refreshData()
{
    padData = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
    
    padDataChangeCallback(PadData::DataIDs::NoteTriggerMode);
}

void PadTriggerModePanel::paint(Graphics &g)
{
    g.fillAll(GUIColours::PanelBackground);
    
    g.setColour(GUIColours::AlphaGreen);
    g.drawRect(1, 1, getWidth()-2, getHeight()-2);
    
    g.setColour(Colours::white);
    g.drawFittedText("TRIGGER MODE", 4, 4, getWidth(), getHeight(), Justification::topLeft, 1);
    
}

void PadTriggerModePanel::resized()
{
    stdBtn.setBounds(1, 20, getWidth()*0.25, 30);
    tglBtn.setBounds(stdBtn.getBounds().translated(getWidth()*0.25, 0));
    ltchBtn.setBounds(tglBtn.getBounds().translated(getWidth()*0.25, 0));
    trgrBtn.setBounds(ltchBtn.getBounds().translated(getWidth()*0.25, 0));

}

void PadTriggerModePanel::buttonClicked(Button* button)
{
    if (button == &stdBtn)
    {
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::StandardNoteMode);
    }
    else if (button == &tglBtn)
    {
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::ToggleNoteMode);

    }
    else if (button == &ltchBtn)
    {
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::LatchNoteMode);

    }
    else if (button == &trgrBtn)
    {
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::TriggerNoteMode);
    }
    else
    {
        return;
    }
    
    AppData::Instance()->getEnginePointer()->getSpherePointer(padData->getParentSphereID())->killPad(padData->getPadID());
}

void PadTriggerModePanel::padDataChangeCallback(const int changedData)
{
    if (changedData == PadData::DataIDs::NoteTriggerMode)
    {
        switch (padData->getNoteTriggerMode()) {
            case PadData::NoteTriggerModes::StandardNoteMode:
                stdBtn.setToggleState(true, dontSendNotification);
                tglBtn.setToggleState(false, dontSendNotification);
                ltchBtn.setToggleState(false, dontSendNotification);
                trgrBtn.setToggleState(false, dontSendNotification);
                break;
            case PadData::NoteTriggerModes::ToggleNoteMode:
                stdBtn.setToggleState(false, dontSendNotification);
                tglBtn.setToggleState(true, dontSendNotification);
                ltchBtn.setToggleState(false, dontSendNotification);
                trgrBtn.setToggleState(false, dontSendNotification);
                break;
            case PadData::NoteTriggerModes::LatchNoteMode:
                stdBtn.setToggleState(false, dontSendNotification);
                tglBtn.setToggleState(false, dontSendNotification);
                ltchBtn.setToggleState(true, dontSendNotification);
                trgrBtn.setToggleState(false, dontSendNotification);
                break;
            case PadData::NoteTriggerModes::TriggerNoteMode:
                stdBtn.setToggleState(false, dontSendNotification);
                tglBtn.setToggleState(false, dontSendNotification);
                ltchBtn.setToggleState(false, dontSendNotification);
                trgrBtn.setToggleState(true, dontSendNotification);
                break;
            default:
                break;
        }
    }
}
