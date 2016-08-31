//
//  NoteSelectKBComponent.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 25/08/2016.
//
//

#include "NoteSelectKBComponent.hpp"
/** 
    Class to be used to select midi notes in Alphalive 2.
 */

//============================================================================================
//
// KBComponentKey
//
//============================================================================================

KBComponentKey::KBComponentKey(const int noteNumber, const String labelText)
{
    
    isBlackKey = selected = false;
    velPercentage = 100;
    
    int moduloNoteNumber = noteNumber % 12;
    
    for (int i = 0; i < 5; i++)
    {
        if (moduloNoteNumber == blackKeys[i])
        {
            isBlackKey = true;
        }
    }
    velPercentageColour = *new Colour(GUIColours::MainBlue);
    
    labelFont = * new Font("Roboto", "sans-serif", 12.0);
    
    
    if (labelText != NULLSTRING)
    {
        noteLabel.setText(labelText, dontSendNotification);
        noteLabel.setJustificationType(Justification::centred);
        noteLabel.setFont(labelFont);
        addAndMakeVisible(noteLabel);
    }
}

KBComponentKey::~KBComponentKey()
{
    
}

void KBComponentKey::resized()
{
    labelFont.setHeight(getHeight()*0.3);
    noteLabel.setBounds(getLocalBounds().withTrimmedTop(getHeight()*0.75));
}

void KBComponentKey::paint(Graphics& g)
{
    if (selected)
    {
        g.setColour(velPercentageColour);

    }
    else
    {
        if(!isBlackKey)
        {
            g.setColour(Colours::white);
        }
        else
        {
            g.setColour(Colours::black);
        }
    }
   
    g.fillAll();
    
    if (selected)
    {
        g.setColour(velPercentageColour.brighter());

        g.fillRect(getLocalBounds().withTrimmedBottom(getHeight()* (velPercentage/100.0)));
        //DBG(getHeight()* (velPercentage/100.0));
    }
    
    g.setColour(Colours::black);
    g.drawRect(getLocalBounds(),2);
}
const bool KBComponentKey::getIsBlackKey()
{
    return isBlackKey;
}

const int KBComponentKey::getVelPercentage()
{
    return velPercentage;
}

void KBComponentKey::setVelPercentage(const int newPercentage)
{
    if (newPercentage != velPercentage)
    {
        if (newPercentage > -1 && newPercentage < 101)
        {
            velPercentage = newPercentage;
            DBG(velPercentage);
            repaint();
        }
    }
}

const int KBComponentKey::getNoteNumber()
{
    return noteNumber;
}

const bool KBComponentKey::getSelected()
{
    return selected;
}

void KBComponentKey::setSelected(const bool shouldBeSelected)
{
    if (selected != shouldBeSelected)
    {
        selected = shouldBeSelected;
        repaint();
    }
}

//============================================================================================
//
// NOTESELECTKBCOMPONENT
//
//============================================================================================


NoteSelectKBComponent::NoteSelectKBComponent()
{
    for (int i = 0; i < 128; i++)
    {
        if (i%12 == 0)
        {
            keys.add(new KBComponentKey(i, "C" + String((i/12)-2)));
        }
        else
        {
            keys.add(new KBComponentKey(i));

        }
        keys.getLast()->addMouseListener(this, true);
        addAndMakeVisible(keys.getLast());
        //mainViewPort.addChildComponent(keys.getLast());
    }
    
}

NoteSelectKBComponent::~NoteSelectKBComponent()
{
    
}

void NoteSelectKBComponent::refreshData()
{
    
}

void NoteSelectKBComponent::padDataChangeCallback(const int changedData)
{
    padData = AppData::Instance()->getCurrentlyInspectingPadPtr();
    
    if (changedData == PadData::DataIDs::MidiNotes)
    {
        Array<PadData::MidiNote> newMidiNotes = padData->getMidiNotes();
        for (int i = 0; i < keys.size(); i++)
        {
            for (int j = 0; j < newMidiNotes.size(); j++)
            {
                if (newMidiNotes[j].noteNumber == i)
                {
                    keys[i]->setSelected(true);
                    keys[i]->setVelPercentage(newMidiNotes[j].velocityPercentage);
                }
                else
                {
                    keys[i]->setSelected(false);
                }
            }
        }
    }
}

void NoteSelectKBComponent::resized()
{
    Component* lastWhiteKey = keys[0];
    halfHeight = getHeight()*0.55;
    
    
    whiteKeyWidth = getHeight()*0.2;
    blackKeyWidth = whiteKeyWidth*0.6;
    
    
    keys[0]->setBounds(0, 0, whiteKeyWidth, getHeight());
    
    for (int i = 1; i < 128; i++)
    {
        
        if (!keys[i]->getIsBlackKey())
        {
            keys[i]->setBounds(lastWhiteKey->getBounds().translated(whiteKeyWidth, 0));
            //keys[i]->setSize(40, getHeight());
            lastWhiteKey = keys[i];

        }
    }
    
    lastWhiteKey = keys[0];
    for (int i = 1; i < 128; i++)
    {
        //keys[i]->setBounds(keys[0]->getBounds().translated(20*i, 0));
        if (keys[i]->getIsBlackKey())
        {
            keys[i]->setBounds(lastWhiteKey->getBounds().translated(blackKeyWidth*1.15, 0));
            keys[i]->setSize(blackKeyWidth, halfHeight);
            keys[i]->setAlwaysOnTop(true);
        }
        else
        {
            lastWhiteKey = keys[i];
        }
    }

}

void NoteSelectKBComponent::mouseDown(const MouseEvent &event)
{
    if (!event.mods.isAnyModifierKeyDown())
    {
        if (event.eventComponent != this)
        {
            for (int i = 0; i < keys.size(); i++)
            {
                if (event.eventComponent == keys[i])
                {
                    
                    keys[i]->setSelected(!keys[i]->getSelected());
                }
            }
        }
    }
    
}

void NoteSelectKBComponent::mouseDrag (const MouseEvent &event)
{
    if (event.mods.isAltDown())
    {
        for (int i = 0; i < keys.size(); i++)
        {
            if (event.eventComponent == keys[i])
            {
                static int dragDistance;
                dragDistance = event.getDistanceFromDragStartY();
                if (dragDistance < 0)
                {
                    dragDistance = 0;
                }
                else if (dragDistance > 100)
                {
                    dragDistance = 100;
                }
                
                keys[i]->setVelPercentage(100-dragDistance);
            }
        }
    }
}


void NoteSelectKBComponent::paint(Graphics& g)
{
    g.fillAll(Colours::hotpink);
}

const float NoteSelectKBComponent::getWidthNeeded(const int forHeight)
{
    return (forHeight * 0.2) * (128.0 * (7.0/12.0));
}

