//
//  NoteSelectKBComponent.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 25/08/2016.
//
//

#ifndef NoteSelectKBComponent_hpp
#define NoteSelectKBComponent_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"
#include "GUIColours.h"

class KBComponentKey : public Component
{
public:
    KBComponentKey(const int noteNumber, const String labelText = NULLSTRING);
    
    ~KBComponentKey();
    void resized() override;
    void paint(Graphics& g) override;
    const bool getIsBlackKey();
    const int getVelPercentage();
    void setVelPercentage(const int newPercentage);
    
    const int getNoteNumber();
    
    const bool getSelected();
    
    void setSelected(const bool shouldBeSelected);
private:
    const int blackKeys[5] = {1,3,6,8,10};
    
    bool isBlackKey;
    int noteNumber;
    int velPercentage;
    bool selected;
    Colour velPercentageColour;
    Label noteLabel;
    Font labelFont;
};

class NoteSelectKBComponent : public PadInspectorPanelBase
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

    const float getWidthNeeded(const int forHeight);
private:
    OwnedArray<KBComponentKey> keys;
    float halfHeight;
    float whiteKeyWidth, blackKeyWidth;
    PadData* padData;
};

#endif /* NoteSelectKBComponent_hpp */
