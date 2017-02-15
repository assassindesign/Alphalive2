//
//  NoteSelectKBComponent.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 25/08/2016.
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

#ifndef NoteSelectKBComponent_hpp
#define NoteSelectKBComponent_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"
#include "GUIStyle.h"

/*
    Class for selecting midi notes in Alphalive 2.
 
    Displays all the keys of a midi keyboard inside a viewport and scrolls to the relevant octave
    when a new key is selected. Scrolling is fairly processor intensive, so the NoteSelectKBComponent
    uses an OpenGLContext to render itself using OpenGL. shift + click allows selection of multiple 
    keys, alt + drag enables tweaking of velocity percentages.
 
    Inherits from PadInspectorPanelBase so it gets a notification when the currently displayed 
    PadData object is changed.
 
 */


class KBComponentKey : public Component
{
public:
    KBComponentKey(const int noteNumber, const String labelText = NULLSTRING);
    
    ~KBComponentKey();
    void resized() override;
    void paint(Graphics& g) override;
    const bool getIsBlackKey();
    const bool getIsCNote();
    const int getVelPercentage();
    void setVelPercentage(const int newPercentage);
    
    const int getNoteNumber();
    
    const bool getSelected();
    
    void setSelected(const bool shouldBeSelected);
private:
    const int blackKeys[5] = {1,3,6,8,10};
    
    bool isBlackKey, isCNote;
    int noteNumber;
    int velPercentage;
    bool selected;
    Colour velPercentageColour;
    Label noteLabel;
    Font labelFont;
};

class NoteSelectPositionerComponent : public Component
{
public:
    NoteSelectPositionerComponent();
    ~NoteSelectPositionerComponent();
    void resized() override;
    void paint(Graphics& g) override;
private:
    
};

class NoteSelectKBComponent : public PadInspectorPanelBase,
                              public Timer
{
public:
    NoteSelectKBComponent();
    ~NoteSelectKBComponent();
    void refreshData() override;
    void padDataChangeCallback(const int changedData) override;
    
    void resized() override;
    void paint(Graphics& g) override;
    void mouseDown(const MouseEvent &event) override;
    void mouseDrag (const MouseEvent &event) override;
    
    void timerCallback() override;

private:
    OwnedArray<KBComponentKey> keys;
    float halfHeight;
    float whiteKeyWidth, blackKeyWidth;
    WeakReference<PadData> padData;
    ScopedPointer<NoteSelectPositionerComponent> keyPositioner;
    Viewport mainViewport;
    
    int lowestKeyToDisplayX, lowestKeytoDisplay;
    int totalXDistanceToMove;
    bool scrollingKeys;
    
    OpenGLContext glContext;
};

#endif /* NoteSelectKBComponent_hpp */
