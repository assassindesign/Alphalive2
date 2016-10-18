//
//  NoteSelectKBComponent.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 25/08/2016.
//
//

#include "NoteSelectKBComponent.hpp"
#include "Alphalive2Engine.hpp"
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
    
    isCNote = isBlackKey = selected = false;
    velPercentage = 100;
    
    int moduloNoteNumber = noteNumber % 12;
    
    for (int i = 0; i < 5; i++)
    {
        if (moduloNoteNumber == blackKeys[i])
        {
            isBlackKey = true;
        }
        else if (moduloNoteNumber == 0)
        {
            isCNote = true;
        }
    }
    velPercentageColour = GUIColours::MainBlue;
    
    labelFont = GUIFonts::Roboto;
    labelFont.setHeight(12.0);
    
    if (labelText != NULLSTRING)
    {
        noteLabel.setText(labelText, dontSendNotification);
        noteLabel.setJustificationType(Justification::centred);
        noteLabel.setFont(labelFont);
        noteLabel.setEditable(false);
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

const bool KBComponentKey::getIsCNote()
{
    return isCNote;
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
            //DBG(velPercentage);
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
// NoteSelectPositionerComponent
//
//============================================================================================

NoteSelectPositionerComponent::NoteSelectPositionerComponent()
{
    
}

NoteSelectPositionerComponent::~NoteSelectPositionerComponent()
{
    
}

void NoteSelectPositionerComponent::resized()
{
    
}

void NoteSelectPositionerComponent::paint(Graphics& g)
{
    g.fillAll(Colours::limegreen);
}


//============================================================================================
//
// NOTESELECTKBCOMPONENT
//
//============================================================================================


NoteSelectKBComponent::NoteSelectKBComponent()
{
    keyPositioner = new NoteSelectPositionerComponent();
    //addAndMakeVisible(keyPositioner);
    addChildComponent(keyPositioner);
    mainViewport.setViewedComponent(keyPositioner, false);
    addAndMakeVisible(mainViewport);
    
    lowestKeyToDisplayX = 0;
    
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
        keyPositioner->addAndMakeVisible(keys.getLast());
    }
    
    scrollingKeys = false;
    
    setInterceptsMouseClicks(true, true);
    lowestKeytoDisplay = 0;
}

NoteSelectKBComponent::~NoteSelectKBComponent()
{
    
}

void NoteSelectKBComponent::refreshData()
{
    padDataChangeCallback(PadData::DataIDs::MidiNotes);
}

void NoteSelectKBComponent::padDataChangeCallback(const int changedData)
{
    padData = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
    
    if (changedData == PadData::DataIDs::MidiNotes || changedData == PadData::DataIDs::PadMidiFunction)
    {
        Array<PadData::MidiNote> newMidiNotes = padData->getMidiNotes();
        if (newMidiNotes.size() > 0)
        {
            if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::SingleNote)
            {
                for (int i = 0; i < keys.size(); i++)
                {
                    if (newMidiNotes[0].noteNumber == i)
                    {
                        keys[i]->setSelected(true);
                        keys[i]->setVelPercentage(newMidiNotes[0].velocityPercentage);
                    }
                    else
                    {
                        keys[i]->setSelected(false);
                    }
                }
            }
            else if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::MultiNote)
            {
                for (int i = 0; i < keys.size(); i++)
                {
                    keys[i]->setSelected(false);

                    for (int j = 0; j < newMidiNotes.size(); j++)
                    {
                        if (newMidiNotes[j].noteNumber == i)
                        {
                            keys[i]->setSelected(true);
                            keys[i]->setVelPercentage(newMidiNotes[j].velocityPercentage);
                        }
                    }
                }
            }

            
            
            int lowestNote = 128;
            for (int i = 0; i < newMidiNotes.size(); i++)
            {
                if (newMidiNotes[i].noteNumber < lowestNote)
                    lowestNote = newMidiNotes[i].noteNumber;
            }
            
            if (lowestNote < 128)
            {
                while (!keys[lowestNote]->getIsCNote())
                {
                    lowestNote--;
                }
                
                if (lowestNote>0)
                {
                    lowestNote = lowestNote -1;
                }
                
//                if (lowestNote > 111)
//                {
//                    lowestKeytoDisplay = 111;
//                    lowestKeyToDisplayX = keyPositioner->getRight() - keyPositioner->getWidth();
//                    scrollingKeys = true;
//                    startTimer(10);
//                }
//                else
//                {
                    lowestKeytoDisplay = lowestNote;
                    lowestKeyToDisplayX = keys[lowestNote]->getBounds().getX();
                    scrollingKeys = true;
                    startTimer(10);
//                }
                
            }
            
        }
        
    }
}

void NoteSelectKBComponent::resized()
{
    static int x, y;
    x = getWidth();
    y = getHeight() - mainViewport.getScrollBarThickness();
    
    Component* lastWhiteKey = keys[0];
    halfHeight = y*0.55;

    whiteKeyWidth = getHeight()*0.2;
    blackKeyWidth = whiteKeyWidth*0.6;
    
    mainViewport.setBounds(getLocalBounds());
    
    keys[0]->setBounds(0, 0, whiteKeyWidth, y);
    
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
    
    keyPositioner->setBounds(0,0, keys.getLast()->getRight(), y);

    mainViewport.setViewPosition(keys[lowestKeytoDisplay]->getBounds().getX() , 0);
    
}

void NoteSelectKBComponent::mouseDown(const MouseEvent &event)
{
    if (!scrollingKeys)
    {
        if (!event.mods.isAnyModifierKeyDown())
        {
            if (event.eventComponent != this) // if one of the keys has been clicked
            {
                //padData = AppData::Instance()->getCurrentlyInspectingPadPtr();
                static Array<PadData::MidiNote> midiNotes;
                midiNotes = padData->getMidiNotes(); //get midi notes from paddata

                
//                if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::SingleNote)
//                {
                    for (int i = 0; i < keys.size(); i++) //loop through the keys - SHOULD BE OPTIMISED AT SOME POINT
                    {
                        if (event.eventComponent == keys[i] || keys[i]->isParentOf(event.eventComponent)) //when we find the key that has been pressed
                        {
                            AppData::Instance()->getEnginePointer()->getPlayablePadPtr(padData->getParentSphere()->getSphereID(), padData->getPadID())->killPad();
                            
                            int diff = i - padData->getMidiNote();
                            Array<PadData::MidiNote> currentNotes = padData->getMidiNotes();
                            padData->clearAllMidiNotes();
                            for (int j = 0; j < currentNotes.size(); j++)
                            {
                                padData->addMidiNote(currentNotes[j].noteNumber + diff, currentNotes[j].velocityPercentage);
                            }
                            
                            //padData->setMidiNote(i, keys[i]->getVelPercentage());
                        }
                    }
//                }
        
            }
        }
        else if (event.mods.isShiftDown())
        {
//            if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::MultiNote)
//            {
                for (int i = 0; i < keys.size(); i++) //loop through the keys - SHOULD BE OPTIMISED AT SOME POINT
                {
                    if (event.eventComponent == keys[i] || keys[i]->isParentOf(event.eventComponent)) //when we find the key that has been pressed
                    {
                        if (!keys[i]->getSelected())
                        {
                            padData->addMidiNote(i);
                        }
                        else
                        {
                            if (padData->getNumMidiNotes() > 1)
                            {
                                padData->removeMidiNote(i);
                            }
                        }
                        
                    }
                }
//            }
        }
    }
}

void NoteSelectKBComponent::mouseDrag (const MouseEvent &event)
{
    if (!scrollingKeys)
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
                    
                    padData->setVelPercentForNote(i, 100-dragDistance);
                }
            }
        }
    }
    
}


void NoteSelectKBComponent::paint(Graphics& g)
{
    //g.fillAll(Colours::hotpink);
}


void NoteSelectKBComponent::timerCallback()
{
    static int targetXposition;
    static int increment, currentXPosition;
    
    increment = 16;
    
    currentXPosition = mainViewport.getViewPosition().x;
    targetXposition = lowestKeyToDisplayX;
    
    if (currentXPosition > targetXposition)
    {
        mainViewport.setViewPosition(currentXPosition-increment, 0);
        
        if (currentXPosition - targetXposition < increment)
        {
            mainViewport.setViewPosition(targetXposition, 0);
            stopTimer();
            scrollingKeys = false;
        }
    }
    else if (currentXPosition < targetXposition)
    {
        mainViewport.setViewPosition(currentXPosition+increment, 0);

        if (targetXposition - currentXPosition < increment)
        {
            mainViewport.setViewPosition(targetXposition, 0);
            stopTimer();
            scrollingKeys = false;
        }
        
    }
    else
    {
        stopTimer();
        scrollingKeys = false;
    }
    
}

