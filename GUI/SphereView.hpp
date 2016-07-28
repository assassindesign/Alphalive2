//
//  SphereView.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 14/06/2016.
//
//

#ifndef SphereView_hpp
#define SphereView_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadUI.hpp"

class MainContentComponent;
static int64 PadColours[25] = {0xffff0000, 0xffff4000, 0xffff8000, 0xffffc000, 0xffffff00, 0xffc0ff00, 0xff80ff00, 0xff80ff00, 0xff40ff00, 0xff00ff00, 0xff00ff40, 0xff00ff80, 0xff00ffc0, 0xff00ffff, 0xff00c0ff, 0xff0080ff, 0xff0040ff, 0xff0000ff, 0xff4000ff, 0xff8000ff, 0xffc000ff, 0xffff00ff, 0xff0000c0, 0xff000080, 0xff000040};


class SphereView : public Component,
                   public Slider::Listener,
                   public ChangeListener
{
public:
    SphereView(const int _sphereID, MainContentComponent &ref);
    ~SphereView();
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override;
    
    void setAllPadColours(const Colour newColour);
    
    //temporary change listener for colour selector
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    
    
private:
    
    //Mouse Listener============================
    void mouseEnter (const MouseEvent &event) override;
    void mouseExit (const MouseEvent &event) override;
    void mouseDown (const MouseEvent &event) override;
    void mouseDrag (const MouseEvent &event) override;
    void mouseUp (const MouseEvent &event) override;
    void mouseDoubleClick (const MouseEvent &event) override;
    
    void clearSelectedPads();
    
    void positionPad(Pad* padToPosition, const int segment, const int row);
    
    float gridDivisions;
    
    OwnedArray<Line<float>> segmentLines;
    
    ScopedPointer<Line<float>> paintHelperLine;
    OwnedArray<Pad> pads;
    
    Point<int> centerPoint;
    
    int numRows;
    
    float padSpacing, circleRadius;
    
    Array<float> rowRadii, padSizeModifiers, rowRadiusModifiers;
    
    OwnedArray<Slider> ratioSliders;
    
    SphereData* sphereData;
    
    int sphereID;
    
    ColourSelector colourSelector;
    
    MainContentComponent* mainComponent;
    
    Array<Pad*> selectedPads;
};

#endif /* SphereView_hpp */
