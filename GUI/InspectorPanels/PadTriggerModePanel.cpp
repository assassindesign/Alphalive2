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
    stdBtn = new CustomIconButton(CustomIconButton::IconPath::StandardIcon);
    stdBtn->getButtonObject()->addListener(this);
    addAndMakeVisible(stdBtn);
    
    tglBtn = new CustomIconButton(CustomIconButton::IconPath::ToggleIcon);
    tglBtn->getButtonObject()->addListener(this);
    addAndMakeVisible(tglBtn);
    
    ltchBtn = new CustomIconButton(CustomIconButton::IconPath::LatchIcon);
    ltchBtn->getButtonObject()->addListener(this);
    addAndMakeVisible(ltchBtn);
    
    trgrBtn = new CustomIconButton(CustomIconButton::IconPath::TriggerIcon);
    trgrBtn->getButtonObject()->addListener(this);
    addAndMakeVisible(trgrBtn);
    
    
    
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
    g.setFont(GUIFonts::getMainFont());
    g.drawFittedText("TRIGGER MODE", 4, 4, getWidth(), getHeight(), Justification::topLeft, 1);
    
}

void PadTriggerModePanel::resized()
{
    stdBtn->setBounds(1, 20, getWidth()*0.25, 30);
    tglBtn->setBounds(stdBtn->getBounds().translated(getWidth()*0.25, 0));
    ltchBtn->setBounds(tglBtn->getBounds().translated(getWidth()*0.25, 0));
    trgrBtn->setBounds(ltchBtn->getBounds().translated(getWidth()*0.25, 0));

}

void PadTriggerModePanel::buttonClicked(Button* button)
{
    if (button == stdBtn->getButtonObject())
    {
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::StandardNoteMode);
    }
    else if (button == tglBtn->getButtonObject())
    {
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::ToggleNoteMode);

    }
    else if (button == ltchBtn->getButtonObject())
    {
        padData->setNoteTriggerMode(PadData::NoteTriggerModes::LatchNoteMode);

    }
    else if (button == trgrBtn->getButtonObject())
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
                stdBtn->getButtonObject()->setToggleState(true, dontSendNotification);
                tglBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                ltchBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                trgrBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                break;
            case PadData::NoteTriggerModes::ToggleNoteMode:
                stdBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                tglBtn->getButtonObject()->setToggleState(true, dontSendNotification);
                ltchBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                trgrBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                break;
            case PadData::NoteTriggerModes::LatchNoteMode:
                stdBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                tglBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                ltchBtn->getButtonObject()->setToggleState(true, dontSendNotification);
                trgrBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                break;
            case PadData::NoteTriggerModes::TriggerNoteMode:
                stdBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                tglBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                ltchBtn->getButtonObject()->setToggleState(false, dontSendNotification);
                trgrBtn->getButtonObject()->setToggleState(true, dontSendNotification);
                break;
            default:
                break;
        }
    }
}
