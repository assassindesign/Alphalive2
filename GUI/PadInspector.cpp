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
    
    
//    for (int i = 0; i < listEntrys.size(); i++)
//    {
//        if (i == 0) {
//            listEntrys[0]->setBounds(leftBumperBox.getRight(), titleBox.getBottom() + 5, getWidth()-leftBumperBox.getWidth(), 50);
//        }
//        else
//        {
//            listEntrys[i]->setBounds(listEntrys[i-1]->getBounds().translated(0, 52));
//        }
//    }
    
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

void PadInspector::appDataChangeCallback()
{
    
}
