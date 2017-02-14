//
//  MainPadInspectorPanels.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 15/09/2016.
//
//
//  Alphalive 2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  Alphalive 2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "MainPadInspectorPanels.hpp"


//===============================================================================================================
//
//  TOP PANEL
//
//===============================================================================================================

InspectorTopPanel::InspectorTopPanel()
{
    audioButton.setButtonText("AUDIO");
    audioButton.addListener(this);
    audioButton.setConnectedEdges(3);
    addAndMakeVisible(&audioButton);
    
    midiButton.setButtonText("MIDI");
    midiButton.addListener(this);
    midiButton.setConnectedEdges(3);
    
    addAndMakeVisible(&midiButton);
    
    systemButton.setButtonText("SYSTEM");
    systemButton.addListener(this);
    systemButton.setConnectedEdges(3);
    addChildComponent(&systemButton);
    
    powerButton.setButtonText("PWR");
    powerButton.addListener(this);
    powerButton.setConnectedEdges(3);
    powerButton.setAlwaysOnTop(true);
    powerButton.setEnabled(false);
    addAndMakeVisible(&powerButton);
    
    audioButton.setEnabled(false);
    midiButton.setEnabled(false);
    systemButton.setEnabled(false);
    
    addChildComponent(audioPanel);
    addChildComponent(midiPanel);
    addChildComponent(sysPanel);
    
    groupSelector = new ButtonGrid(10, "SEND TO GROUP");
    groupSelector->addListener(this);
    addChildComponent(groupSelector);
    
    if (AppData::Instance()->getAdvancedFeaturesEnabled())
    {
        systemButton.setVisible(true);
    }
    
    //padDataChangeCallback(PadData::DataIDs::PadFunction); //pull currenly inspected pad function from appdata
    
    AppData::Instance()->addListener(this);
    //AppData::Instance()->setAdvancedFeaturesEnabled(true);
    
    for (int i = 0; i < 24; i++)
    {
        rackAccents.add(*new Rectangle<float>(2,0,6,6));
    }
    
    setTooltip("Pad Inspector");
}
InspectorTopPanel::~InspectorTopPanel()
{

}

void InspectorTopPanel::resized()
{
    static float buttonWidth;
    
    if (AppData::Instance()->getAdvancedFeaturesEnabled())
        buttonWidth = getWidth()/3.0;
    else
        buttonWidth = getWidth()/2.0;
    
    audioButton.setBounds(0, 0, buttonWidth, 25);
    midiButton.setBounds(audioButton.getBounds().translated(buttonWidth, 0));
    
    if (AppData::Instance()->getAdvancedFeaturesEnabled())
        systemButton.setBounds(midiButton.getBounds().translated(buttonWidth, 0));
    
    powerButton.setBounds(10, audioButton.getBottom(), (getWidth() - 20)/6.0, audioButton.getHeight());
    
    static float twelthHeight;
    twelthHeight = (getHeight() - audioButton.getHeight()) /12.0;
    
    for (int i = 0; i < 12; i++)
    {
        rackAccents.getReference(i).setPosition(2, (twelthHeight*i)+ twelthHeight*0.4 + audioButton.getHeight());
        
        //rackAccents[i].setY(twelthHeight*i);
        rackAccents.getReference(i+12).setPosition(getWidth()-8, (twelthHeight*i)+ twelthHeight*0.4 + audioButton.getHeight());
        
        //DBG(String(rackAccents[i].getX()) + ":" + String(rackAccents[i].getY()) + " : " + String(twelthHeight*i));
    }
    
    groupSelector->setBounds(10, getHeight()-60, getWidth() -20, 60);
    
    audioPanel.setBounds(10, audioButton.getBottom(), getWidth()-20, getHeight() - audioButton.getBottom());
    midiPanel.setBounds(audioPanel.getBounds());
    sysPanel.setBounds(audioPanel.getBounds());
    
    
    
}

void InspectorTopPanel::paint (Graphics& g)
{
    g.fillAll(GUIColours::Background);
    
    g.setColour(GUIColours::Background.brighter());
    g.fillRect(0, audioButton.getBottom(), 10, getHeight());
    g.fillRect(getWidth()-10, audioButton.getBottom(), 10, getHeight());
    
    g.setColour(GUIColours::RackScrewAccent);
    for (int i = 0; i < rackAccents.size(); i++)
    {
        g.fillEllipse(rackAccents[i]);
    }
    //g.fillAll(Colours::azure);
}

void InspectorTopPanel::appDataChangeCallback(const int changedData)
{
    if (changedData == AppData::DataIDs::AdvancedEnabled)
    {
        systemButton.setVisible(AppData::Instance()->getAdvancedFeaturesEnabled());
    }
    
}

void InspectorTopPanel::padDataChangeCallback(const int changedData)
{
    padData = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
    
    if (changedData == PadData::DataIDs::PadFunction)
    {
        switch (AppData::Instance()->getCurrentlyInspectingPadDataPtr()->getPadFunction()) {
            case PadData::PadFunctions::Audio:
                audioPanel.setVisible(true);
                midiPanel.setVisible(false);
                sysPanel.setVisible(false);
                audioButton.setToggleState(true, dontSendNotification);
                midiButton.setToggleState(false, dontSendNotification);
                systemButton.setToggleState(false, dontSendNotification);
                break;
            case PadData::PadFunctions::Midi:
                audioPanel.setVisible(false);
                midiPanel.setVisible(true);
                sysPanel.setVisible(false);
                audioButton.setToggleState(false, dontSendNotification);
                midiButton.setToggleState(true, dontSendNotification);
                systemButton.setToggleState(false, dontSendNotification);
                break;
            case PadData::PadFunctions::System:
                audioPanel.setVisible(false);
                midiPanel.setVisible(false);
                sysPanel.setVisible(true);
                audioButton.setToggleState(false, dontSendNotification);
                midiButton.setToggleState(false, dontSendNotification);
                systemButton.setToggleState(true, dontSendNotification);
                break;
            default:
                break;
        }
    }
    else if (changedData == PadData::DataIDs::PadGroup)
    {
        groupSelector->setButtonSelected(padData->getPadGroup(), true);
    }
    else if (changedData == PadData::DataIDs::PadEnabled);
    {
        audioPanel.setVisible(padData->getPadEnabled() && audioButton.getToggleState());
        midiPanel.setVisible(padData->getPadEnabled() && midiButton.getToggleState());
        sysPanel.setVisible(padData->getPadEnabled() && systemButton.getToggleState());
        groupSelector->setVisible(padData->getPadEnabled());
        
        
        powerButton.setToggleState(padData->getPadEnabled(), dontSendNotification);
    }
}


void InspectorTopPanel::buttonClicked (Button* button)
{
    if (button == &audioButton)
    {
        //set pad mode to audio
        AppData::Instance()->getCurrentlyInspectingPadDataPtr()->setPadFunction(PadData::PadFunctions::Audio);
    }
    else if (button == &midiButton)
    {
        //set pad mode to midi
        AppData::Instance()->getCurrentlyInspectingPadDataPtr()->setPadFunction(PadData::PadFunctions::Midi);
    }
    else if (button == &systemButton)
    {
        //set pad mode to system
        AppData::Instance()->getCurrentlyInspectingPadDataPtr()->setPadFunction(PadData::PadFunctions::System);
    }
    else if (button == &powerButton)
    {
        //set pad enabled
        powerButton.setToggleState(!powerButton.getToggleState(), dontSendNotification);
        AppData::Instance()->getEnginePointer()->getPlayablePadPtr(padData->getParentSphereID(), padData->getPadID())->setPadEnabled(powerButton.getToggleState());
        
        //AppData::Instance()->getCurrentlyInspectingPadDataPtr()->setPadEnabled(powerButton.getToggleState());
    }
}

void InspectorTopPanel::refreshData()
{
    
    audioPanel.setDataObject(getDataObject());
    midiPanel.setDataObject(getDataObject());
    sysPanel.setDataObject(getDataObject());
    padDataChangeCallback(PadData::DataIDs::PadFunction);
    padDataChangeCallback(PadData::DataIDs::PadGroup);
    padDataChangeCallback(PadData::DataIDs::PadEnabled);
}

void InspectorTopPanel::setPanelEnabled(bool enabled)
{
    if (enabled)
    {
        
    }
    else
    {
        audioPanel.setVisible(false);
        midiPanel.setVisible(false);
        sysPanel.setVisible(false);
        groupSelector->setVisible(false);
    }
    
    audioButton.setEnabled(enabled);
    midiButton.setEnabled(enabled);
    systemButton.setEnabled(enabled);
    powerButton.setEnabled(enabled);
    
    audioButton.setToggleState(false, dontSendNotification);
    midiButton.setToggleState(false, dontSendNotification);
    systemButton.setToggleState(false, dontSendNotification);
}

void InspectorTopPanel::buttonGridCallback(ButtonGrid* grid, const int buttonID)
{
    if (grid == groupSelector)
    {
        padData->setPadGroup(buttonID);
        DBG(padData->getPadGroup());
    }
}

//===============================================================================================================
//
//  BOTTOM PANEL
//
//===============================================================================================================

InspectorBottomPanel::InspectorBottomPanel()
{
    groupTitleFont = GUIFonts::getMainFont().withHeight(30);
}

InspectorBottomPanel::~InspectorBottomPanel()
{

}

void InspectorBottomPanel::resized()
{
    groupTitleBox.setBounds(0, 0, getWidth(), 40);
}

void InspectorBottomPanel::paint (Graphics& g)
{
    g.fillAll(GUIColours::Background.brighter());
    
    g.setColour(Colours::whitesmoke);
    g.setFont(groupTitleFont);
    g.drawFittedText("GROUPS", groupTitleBox, Justification::centred, 1);
    
    
}

void InspectorBottomPanel::refreshData()
{
    
}


//===============================================================================================================
//
//  MAIN PANEL
//
//===============================================================================================================

MainInspectorPanel::MainInspectorPanel()
{
    addAndMakeVisible(&top);
    addAndMakeVisible(&bottom);
}

MainInspectorPanel::~MainInspectorPanel()
{

}

void MainInspectorPanel::resized()
{
    top.setBounds(0, 0, getWidth(), getHeight()*0.618);
    bottom.setBounds(0, top.getBottom(), getWidth(), getHeight()-top.getHeight());
}

void MainInspectorPanel::paint (Graphics& g)
{
    g.fillAll(Colour(GUIColours::Background));
}

void MainInspectorPanel::refreshData()
{
    top.setDataObject(getDataObject());
    bottom.setDataObject(getDataObject());
}

void MainInspectorPanel::setTopPanelEnabled(bool enabled)
{
    top.setPanelEnabled(enabled);
    if (enabled)
    {
        top.AppDataListener::padDataChangeCallback(PadData::DataIDs::PadFunction);
    }
}
