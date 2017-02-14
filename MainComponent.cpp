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
    
    setLookAndFeel(appData->getAlphaliveLookAndFeel());
    
    padInspector = new PadInspector();
    addAndMakeVisible(padInspector);
    
    sphereView = new SphereView(0, *this);
    addAndMakeVisible(sphereView);

    //tempoView = new TempoView();
    //addAndMakeVisible(tempoView);
    
    //scaleView = new ScaleView();
    //addAndMakeVisible(scaleView);
    
    killButton = new TextButton();
    killButton->setButtonText("Kill");
    killButton->addListener(this);
    addAndMakeVisible(killButton);
    
    audioMeter = new AudioMeterButton();
    //addAndMakeVisible(audioMeter);
    
    connectionStatus = new AlphaSphereConnectedButton();
    addAndMakeVisible(connectionStatus);
    
    tooltip = new TooltipWindow(this, 700);
    tooltip->setAlwaysOnTop(true);
    
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
    
    sphereView->setBounds(segmentWidth*2,30,segmentWidth*6, getHeight()-40);
    
    connectionStatus->setBounds(segmentWidth*5 - 40, 0, 80, 15);
    //tempoView->setBounds(0, getHeight()/10.0, getWidth()/8.0, getHeight()/10.0);
    //scaleView->setBounds(0, 0, getWidth()/8.0, getHeight()/10.0);
    
    audioMeter->setBounds(sphereView->getRight() - 100, 5, 100, 30);
    
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


#endif  // MAINCOMPONENT_CPP_INCLUDED
