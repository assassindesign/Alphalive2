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
    backgroundColour = Colour(GUIColours::Background).withBrightness(Colour(GUIColours::Background).getBrightness()+0.05);
//    for (int i = 0; i < 10; i++)
//    {
//        listEntrys.add(new ListEntry());
//        addAndMakeVisible(listEntrys.getLast());
//    }
    
    midiNoteSlider.setRange(0, 127, 1);
    addAndMakeVisible(midiNoteSlider);
    
    addAndMakeVisible(mainInspectorPanel);
    mainViewport.setViewedComponent(&mainInspectorPanel);
    
    mainViewport.setScrollBarsShown(false, false, true);
    addAndMakeVisible(mainViewport);
    
    AppData::Instance()->addListener(this);
    
}

PadInspector::~PadInspector()
{
    
}

void PadInspector::paint(Graphics& g)
{
    g.fillAll(backgroundColour);
    
    g.setColour(backgroundColour.withBrightness(0.25));
    g.fillRect(titleBox);
    
    g.setColour(backgroundColour.withBrightness(0.1));
    g.fillRect(leftBumperBox);

   
    
    g.setColour(Colours::white);
    g.drawText("Pad Inspector", titleBox, Justification::centred);
    
    if (padDataToInspect != nullptr)
    {
        g.drawSingleLineText(String(padDataToInspect->getPadID()), 0, getHeight());

    }
}

void PadInspector::resized()
{
    titleBox = leftBumperBox = getLocalBounds();
    leftBumperBox.removeFromRight(getWidth()-10);
    titleBox.removeFromBottom(getHeight()-20);
    

    
    mainViewportBox.setPosition(leftBumperBox.getRight(), titleBox.getBottom());
    mainViewportBox.setSize(getWidth()-leftBumperBox.getWidth(), getHeight()-titleBox.getHeight());
    
    
    mainViewport.setBounds(mainViewportBox);
    mainInspectorPanel.setSize(mainViewportBox.getWidth(), mainViewportBox.getHeight());

}

void PadInspector::setPadDataToInspect(PadData* newPadData)
{
    if (newPadData != padDataToInspect)
    {
        if (padDataToInspect != nullptr)
        {
            padDataToInspect->removeListener(this);
        }
        
        if (newPadData != nullptr)
        {
            padDataToInspect = newPadData;
            padDataToInspect->addListener(this);
            
        }
        repaint();
    }
}

void PadInspector::appDataChangeCallback(const int changedData)
{
    if (changedData == AppData::DataIDs::InspectingPad)
    {
        padDataToInspect = AppData::Instance()->getCurrentlyInspectingPadPtr();
        
        mainInspectorPanel.setDataObject(padDataToInspect);
        
    }
}
