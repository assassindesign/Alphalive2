//
//  NeonButton.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 11/07/2016.
//
//

#ifndef NeonButton_hpp
#define NeonButton_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "GUIColours.h"

class NeonButton : public Button
{
public:
    NeonButton();
    ~NeonButton();
    
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    
    //void setButtonText(const String buttonText);
    
    void setColour(const Colour newColour);
    private:
    Colour neonColour, hoverColour, downColour;
    ScopedPointer<Font> font;
};

#endif /* NeonButton_hpp */
