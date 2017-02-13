//
//  PadInspector.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 12/07/2016.
//
//

#ifndef PadInspector_hpp
#define PadInspector_hpp

#include "../JuceLibraryCode/JuceHeader.h"
//#include "AppData.hpp"
#include "GUIStyle.h"
#include "MainPadInspectorPanels.hpp"

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
    /*Midi note(s)
      Velocity
      Midi Channel
      Pressure Mode
      Sticky
      Enabled
      Quantised
      Reverse Pressure
      Vel Curve
      Press Curve
      Group
    */
};

#endif /* PadInspector_hpp */
