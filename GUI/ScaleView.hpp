//
//  ScaleView.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 11/07/2016.
//
//

#ifndef ScaleView_hpp
#define ScaleView_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "AlphaLive2Engine.hpp"
#include "NeonButton.hpp"

class ScaleView : public Button::Listener,
                  public AppDataListener,
                  public Component
                  
{
public:
    
    
    ScaleView();
    ~ScaleView();
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    void appDataChangeCallback(const int changedData) override;
    
    void buttonClicked (Button*) override;
private:
    NeonButton upButton, downButton, octPlusButton, octMinusButton;
    Label keyLabel;
    
    ScopedPointer<StringArray> labelStrings;
    
    ScaleData* scaleData;
    //const char*
    int currentKey, currentOctave, currentScale;
    
    ComboBox scaleSelectCB, rowConfigCB;


};

#endif /* ScaleView_hpp */
