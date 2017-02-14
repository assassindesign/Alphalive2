//
//  SphereView.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 14/06/2016.
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

#ifndef SphereView_hpp
#define SphereView_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadUI.hpp"

class MainContentComponent;

class SphereView : public Component,
                   public Slider::Listener,
                   public ChangeListener,
                   public AppDataListener
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
    void appDataChangeCallback(const int changedData) override;

    
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
