//
//  PadUI.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 12/07/2016.
//
//

#ifndef PadUI_hpp
#define PadUI_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "AlphaLive2Engine.hpp"
#include "GUIColours.h"
#include "PadInspector.hpp"

class MainContentComponent;
class Pad : public GUIRepaintListener
{
public:
    Pad(PadData* _padData, MainContentComponent &ref);
    ~Pad();
    void paint(Graphics& g) override;
    void resized()override;
    void setColour(const Colour newColour);
    void setSelected(const bool padIsSelected);
    bool getSelected();
    
    bool isPointInsideCircle(const Point<int> point);
    //Mouse Listener============================
    void mouseEnter (const MouseEvent &event) override;
    void mouseExit (const MouseEvent &event) override;
    void mouseDown (const MouseEvent &event) override;
    void mouseDrag (const MouseEvent &event) override;
    void mouseUp (const MouseEvent &event) override;
    void mouseDoubleClick (const MouseEvent &event) override;
    
private:
    PadData* padData;
    Colour padColour, lightColour, darkColour, actualColour;
    bool emulatingPadPress;
    PadInspector* padInspector;
    bool isSelected;
    Point<int> centerPoint;
    int halfWidth;
};

#endif /* PadUI_hpp */
