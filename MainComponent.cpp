/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_CPP_INCLUDED
#define MAINCOMPONENT_CPP_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/


MainContentComponent::MainContentComponent()
{
    
    appData = AppData::Instance();
    appData->setEnginePointer(new Alphalive2Engine());
    alphalive2Engine = appData->getEnginePointer();
    
    alphalive2Engine->initialise();
    
    padInspector = new PadInspector();
    addAndMakeVisible(padInspector);
    
    sphereView = new SphereView(0, *this);
    addAndMakeVisible(sphereView);

    tempoView = new TempoView();
    addAndMakeVisible(tempoView);
    
    scaleView = new ScaleView();
    addAndMakeVisible(scaleView);
    
    killButton = new TextButton();
    killButton->setButtonText("Kill");
    killButton->addListener(this);
    addAndMakeVisible(killButton);
    
    setSize (1200, 700);
    
}

MainContentComponent::~MainContentComponent()
{
        
}


//==============================================================================
void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (GUIColours::Background);

}

void MainContentComponent::resized()
{
    static int segmentWidth;
    segmentWidth = (getWidth() - 400)*0.125;
    
    sphereView->setBounds(segmentWidth*2,0,segmentWidth*6, getHeight());
    
    
    //tempoView->setBounds(0, 0, getWidth()/8.0, getHeight()/10.0);
    //scaleView->setBounds(tempoView->getBounds().translated(getWidth()/8.0, 0));
    padInspector->setBounds(getWidth()-385, 0, 385, getHeight());
    
    killButton->setBounds(sphereView->getRight()-40, sphereView->getBottom()-20, 40, 20);
}

PadInspector* MainContentComponent::getPadInspector()
{
    return padInspector;
}

void MainContentComponent::buttonClicked (Button* button)
{
    if (button == killButton)
    {
        alphalive2Engine->killAllPads();
    }
}

    //==============================================================================

    // Your private member variables go here...



#endif  // MAINCOMPONENT_CPP_INCLUDED
