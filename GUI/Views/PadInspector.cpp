//
//  PadInspector.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 12/07/2016.
//
//

#include "PadInspector.hpp"

PadInspector::PadInspector()
{
    padDataToInspect = nullptr;
    backgroundColour = GUIColours::Background.withBrightness(GUIColours::Background.getBrightness()+0.05);

    
    midiNoteSlider.setRange(0, 127, 1);
    addAndMakeVisible(midiNoteSlider);
    
    addAndMakeVisible(mainInspectorPanel);
    mainViewport.setViewedComponent(&mainInspectorPanel);
    
    mainViewport.setScrollBarsShown(false, false, true);
    addAndMakeVisible(mainViewport);
    
    AppData::Instance()->addListener(this);
    
    startTimer(250);
    
    setInterceptsMouseClicks(false, true);
}

PadInspector::~PadInspector()
{
    
}

void PadInspector::paint(Graphics& g)
{
    //g.fillAll(Colours::hotpink);
    
    g.setColour(backgroundColour.withBrightness(0.25));
    g.fillRect(titleBox);
    
    g.setColour(backgroundColour.withBrightness(0.1));
    g.fillRect(leftBumperBox);

   
    
    g.setColour(Colours::white);
    g.setFont(GUIFonts::getMainFont());
    g.drawText("Pad Inspector", titleBox, Justification::centred);
    
}

void PadInspector::resized()
{
    titleBox = leftBumperBox = getLocalBounds();
    leftBumperBox.removeFromRight(getWidth()-10);
    titleBox.removeFromBottom(getHeight()-20);
    

    mainViewportBox.setPosition(leftBumperBox.getRight(), titleBox.getBottom());
    mainViewportBox.setSize(getWidth()-leftBumperBox.getWidth(), getHeight()-titleBox.getHeight());
    
    
    mainViewport.setBounds(mainViewportBox);
    mainInspectorPanel.setSize(mainViewportBox.getWidth(), mainViewportBox.getHeight() * 1.5);

}


void PadInspector::appDataChangeCallback(const int changedData)
{
    if (changedData == AppData::DataIDs::InspectingPad)
    {
      
        inspectingPadChanged = true;
            
        
    }
}


void PadInspector::timerCallback()
{
    if (inspectingPadChanged)
    {
        padDataToInspect = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
        
        if (padDataToInspect == nullptr)
        {
            mainInspectorPanel.setTopPanelEnabled(false);
        }
        else
        {
            mainInspectorPanel.setTopPanelEnabled(true);
            mainInspectorPanel.setDataObject(padDataToInspect);
        }
        
        inspectingPadChanged = false;
    }
    
}

void PadInspector::mouseDown(const MouseEvent& event)
{
//    if (event.eventComponent == ) {
//        <#statements#>
//    }
}


