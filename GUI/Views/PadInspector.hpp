//
//  PadInspector.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 12/07/2016.
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

#ifndef PadInspector_hpp
#define PadInspector_hpp

#include "../JuceLibraryCode/JuceHeader.h"
//#include "AppData.hpp"
#include "GUIStyle.h"
#include "MainPadInspectorPanels.hpp"

/* 
    Highest level PadInspector component, contains all PadData config classes.
    Some beautification elements and the main viewport that enables inspector 
    scrolling.
 
    As a change in padDataToInspect is fairly intensive (updates every child 
    component in some way), A timer system has been implemented to set the 
    minimum time between changes to 250ms.
 */

class PadInspector : public AppDataListener,
                     public Component,
                     public Timer
{
public:
    
    class ListEntry : public Component{
    public:
        ListEntry(){}
        ~ListEntry(){};
        void paint(Graphics& g) override
        { g.fillAll(Colour(GUIColours::Background));}

        void resized() override {}
        
    };
    
    PadInspector();
    ~PadInspector();
    void paint(Graphics& g) override;
    void resized() override;
    void appDataChangeCallback(const int changedData) override;
    void timerCallback() override;
    
    void mouseDown(const MouseEvent& event) override;
    
private:
    WeakReference<PadData> padDataToInspect;
    
    Colour backgroundColour;
    
    Rectangle<int> leftBumperBox, titleBox, mainViewportBox;
        
    Slider midiNoteSlider;
    
    Viewport mainViewport;
    
    MainInspectorPanel mainInspectorPanel;
    
    bool inspectingPadChanged = false;

};

#endif /* PadInspector_hpp */
