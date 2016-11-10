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
    void mouseMove (const MouseEvent&) override;
    
    int isMouseInCircle(const MouseEvent &event);
    
    void clearSelectedPads();
    
    void positionPad(Pad* padToPosition, const int segment, const int row);
    
    
    float gridDivisions;
    
    OwnedArray<Line<float>> segmentLines;
    
    ScopedPointer<Line<float>> paintHelperLine;
    OwnedArray<Pad> pads;
    
    Point<int> centerPoint;
    
    int numRows;
    int mouseCurrentlyOverCircle;
    
    float padSpacing, circleRadius;
    
    Array<float> rowRadii, padSizeModifiers, rowRadiusModifiers;
    Array<Rectangle<float>> backgroundCircleBoxes;
    Array<Path> backgroundCirclePaths;
    
    OwnedArray<Slider> ratioSliders;
    
    SphereData* sphereData;
    
    int sphereID;
    
    ColourSelector colourSelector;
    
    MainContentComponent* mainComponent;
    
    Array<Pad*> selectedPads, prevSelectedPads;
    
    bool allPadsSelected;
    Array<bool> padRowSelected;
    
    OpenGLContext openGLContext;
    
};

#endif /* SphereView_hpp */
