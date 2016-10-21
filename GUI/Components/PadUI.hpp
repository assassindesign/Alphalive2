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
#include "GUIStyle.h"
#include "PadInspector.hpp"

class MainContentComponent;
class Pad : public GUIRepaintListener
{
public:
    Pad(PadData* _padData);
    ~Pad();
    void paint(Graphics& g) override;
    void resized()override;
    void setColour(const Colour newColour);
    void setSelected(const bool padIsSelected);
    bool getSelected();
    void setAsCurrentlyInspectedPad();
    
    
    bool isPointInsideCircle(const Point<int> point);
    //Mouse Listener============================
    void mouseEnter (const MouseEvent &event) override;
    void mouseExit (const MouseEvent &event) override;
    void mouseDown (const MouseEvent &event) override;
    void mouseDrag (const MouseEvent &event) override;
    void mouseUp (const MouseEvent &event) override;
    void mouseDoubleClick (const MouseEvent &event) override;
    
private:
    WeakReference<PadData> padData;
    Colour padColour, lightColour, darkColour, actualColour;
    bool emulatingPadPress;
    bool isSelected;
    Point<int> centerPoint;
    int halfWidth;
    GlowEffect ringGlowEffect;
    
    Array<Rectangle<float>> selectDots;
    Array<Line<float>> selectDotsALines;
};

#endif /* PadUI_hpp */
