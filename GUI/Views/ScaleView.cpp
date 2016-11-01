//
//  ScaleView.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 11/07/2016.
//
//

#include "ScaleView.hpp"
const char* keyStrings[] {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B", NULL};


ScaleView::ScaleView()
{
    upButton.setButtonText("UP");
    upButton.addListener(this);
    addAndMakeVisible(upButton);
    
    downButton.setButtonText("DN");
    downButton.addListener(this);
    addAndMakeVisible(downButton);
    
    octPlusButton.setButtonText("O+");
    octPlusButton.addListener(this);
    addAndMakeVisible(octPlusButton);
    
    octMinusButton.setButtonText("O-");
    octMinusButton.addListener(this);
    addAndMakeVisible(octMinusButton);
    
    keyLabel.setText("C", dontSendNotification);
    keyLabel.setColour(Label::ColourIds::textColourId, Colours::white);
    keyLabel.setJustificationType(Justification::centred);
    keyLabel.setFont(GUIFonts::getMainFont().withHeight(50));
    addAndMakeVisible(keyLabel);
    
    labelStrings = new StringArray(keyStrings);
    
    Alphalive2Engine* engine = AppData::Instance()->getEnginePointer();
    
    scaleData = engine->getHIDLinkedSpherePointer()->getSphereDataObject()->getScaleData();
    
    currentKey = scaleData->getKey();
    currentOctave = scaleData->getOctave();
    scaleData->addListener(this);
    
    //AppData::Instance()->getGlobalScaleData()->addListener(this);
    //addToDesktop(0);
}

ScaleView::~ScaleView()
{
    
}


void ScaleView::paint(Graphics& g)
{
    g.fillAll(GUIColours::Background);
    
}

void ScaleView::resized()
{
    upButton.setBounds(getWidth()/2.0, 0, getWidth()/4.0, getHeight()/2.0);
    downButton.setBounds(upButton.getBounds().translated(0, getHeight()/2.0));
    
    octPlusButton.setBounds(upButton.getBounds().translated(getWidth()/4.0, 0));
    octMinusButton.setBounds(octPlusButton.getBounds().translated(0, getHeight()/2.0));

    
    keyLabel.setBounds(0, 0, getWidth()/2.0, getHeight());
}

void ScaleView::scaleDataChangeCallback(const int changedData)
{

    currentKey = scaleData->getKey();
    currentOctave = scaleData->getOctave();
    currentScale =  scaleData->getScale();

    keyLabel.setText(labelStrings->getReference(currentKey) + String(currentOctave), dontSendNotification);
}


void ScaleView::buttonClicked (Button* button)
{
    PlayableSphere* sphere = AppData::Instance()->getEnginePointer()->getSpherePointer(0);
    if (button == &upButton)
    {
        currentKey++;
        if (currentKey < 0 || currentKey >= labelStrings->size())
        {
            currentKey = 0;
            currentOctave++;
        }
        
        //keyLabel.setText(labelStrings->getReference(currentKey) + String(currentOctave), dontSendNotification);
        sphere->transposeMidiByNote(1);       

        //repaint();
    }
    else if (button == &downButton)
    {
        currentKey--;
        if (currentKey < 0 || currentKey >= labelStrings->size())
        {
            currentKey = labelStrings->size()-1;
            currentOctave--;
        }
        
        //keyLabel.setText(labelStrings->getReference(currentKey) + String(currentOctave), dontSendNotification);
        sphere->transposeMidiByNote(-1);
        //AppData::Instance()->getGlobalScaleData()->setKey(currentKey);

    }
    else if (button == &octPlusButton)
    {
        if (currentOctave > -3 && currentOctave < 9)
        {
            currentOctave++;
            sphere->transposeMidiByOctave(1);
            
        }

    }
    else if (button == &octMinusButton)
    {
        if (currentOctave > -3 && currentOctave < 9)
        {
            currentOctave--;
            sphere->transposeMidiByOctave(-1);
        }
    }
    
    
    
}
