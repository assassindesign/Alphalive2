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
#include "AppData.hpp"
#include "GUIColours.h"


class PadInspector : public GUIRepaintListener
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
    void setPadDataToInspect(PadData* newPadData);
    void refreshData();
    
private:
    PadData* padDataToInspect;
    
    Colour backgroundColour;
    
    Rectangle<int> leftBumperBox, titleBox;
    
    OwnedArray<ListEntry> listEntrys;
    
    Slider midiNoteSlider;
    
    
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
