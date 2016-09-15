//
//  MainPadInspectorPanels.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 15/09/2016.
//
//

#include "MainPadInspectorPanels.hpp"


//===============================================================================================================
//
//  TOP PANEL
//
//===============================================================================================================

InspectorTopPanel::InspectorTopPanel()
{
    audioButton.setButtonText("Audio");
    audioButton.addListener(this);
    audioButton.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));
    audioButton.setConnectedEdges(3);
    addAndMakeVisible(&audioButton);
    
    midiButton.setButtonText("MIDI");
    midiButton.addListener(this);
    midiButton.setConnectedEdges(3);
    midiButton.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));
    
    addAndMakeVisible(&midiButton);
    
    systemButton.setButtonText("System");
    systemButton.addListener(this);
    systemButton.setConnectedEdges(3);
    systemButton.setColour(TextButton::ColourIds::buttonOnColourId, Colour(GUIColours::MainBlue));
    addChildComponent(&systemButton);
    
    audioButton.setEnabled(false);
    midiButton.setEnabled(false);
    systemButton.setEnabled(false);
    
    addChildComponent(audioPanel);
    addChildComponent(midiPanel);
    addChildComponent(sysPanel);
    
    
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
    
    
    
    static float twelthHeight;
    twelthHeight = (getHeight() - audioButton.getHeight()) /12.0;
    
    for (int i = 0; i < 12; i++)
    {
        rackAccents.getReference(i).setPosition(2, (twelthHeight*i)+ twelthHeight*0.5 + audioButton.getHeight());
        
        //rackAccents[i].setY(twelthHeight*i);
        rackAccents.getReference(i+12).setPosition(getWidth()-8, (twelthHeight*i)+ twelthHeight*0.5 + audioButton.getHeight());
        
        //DBG(String(rackAccents[i].getX()) + ":" + String(rackAccents[i].getY()) + " : " + String(twelthHeight*i));
    }
    
    audioPanel.setBounds(0, audioButton.getBottom(), getWidth(), getHeight() - audioButton.getBottom());
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
    if (button == &systemButton)
    {
        //set pad mode to system
        AppData::Instance()->getCurrentlyInspectingPadDataPtr()->setPadFunction(PadData::PadFunctions::System);
    }
}

void InspectorTopPanel::refreshData()
{
    audioPanel.setDataObject(getDataObject());
    midiPanel.setDataObject(getDataObject());
    padDataChangeCallback(PadData::DataIDs::PadFunction);
}

void InspectorTopPanel::setPanelEnabled(bool enabled)
{
    audioPanel.setVisible(enabled);
    midiPanel.setVisible(enabled);
    sysPanel.setVisible(enabled);
    
    audioButton.setEnabled(enabled);
    midiButton.setEnabled(enabled);
    systemButton.setEnabled(enabled);
    
    audioButton.setToggleState(false, dontSendNotification);
    midiButton.setToggleState(false, dontSendNotification);
    systemButton.setToggleState(false, dontSendNotification);
}


//===============================================================================================================
//
//  BOTTOM PANEL
//
//===============================================================================================================

InspectorBottomPanel::InspectorBottomPanel()
{

}

InspectorBottomPanel::~InspectorBottomPanel()
{

}

void InspectorBottomPanel::resized()
{
    
}

void InspectorBottomPanel::paint (Graphics& g)
{
    g.fillAll(Colour(GUIColours::Background).brighter());
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
