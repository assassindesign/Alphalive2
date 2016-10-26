//
//  AlphaLiveLookAndFeel.hpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 10/08/2016.
//
//

#ifndef AlphaLiveLookAndFeel_hpp
#define AlphaLiveLookAndFeel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomVectorPaths.h"
#include "GUIStyle.h"

#define ALPHAGREEN 0xff1d837a


class CustomLookAndFeel    : public LookAndFeel_V3
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel();
    
    void drawRoundThumb (Graphics& g, const float x, const float y,
                         const float diameter, const Colour& colour, float outlineThickness);
 
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;

    
    void drawTickBox (Graphics& g, Component& component,
                      float x, float y, float w, float h,
                      bool ticked,
                      bool isEnabled,
                      bool isMouseOverButton,
                      bool isButtonDown) override;
    
    
    void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                float sliderPos, float minSliderPos, float maxSliderPos,
                                const Slider::SliderStyle style, Slider& slider) override;
    
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle style, Slider& slider) override;
    
    void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                     float /*sliderPos*/,
                                     float /*minSliderPos*/,
                                     float /*maxSliderPos*/,
                                     const Slider::SliderStyle /*style*/, Slider& slider) override;
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
    
    void drawComboBox (Graphics& g, int width, int height, const bool /*isButtonDown*/,
                                       int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override;
    
    void positionComboBoxText (ComboBox& box, Label& label) override;
    
    void drawPopupMenuBackground (Graphics& g, int width, int height) override;
    
    //void drawLabel (Graphics& g, Label& label) override;
    
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                               const bool isSeparator, const bool isActive,
                                               const bool isHighlighted, const bool isTicked,
                                               const bool hasSubMenu, const String& text,
                                               const String& shortcutKeyText,
                                               const Drawable* icon, const Colour* const textColourToUse) override;
private:
    DropShadow knobShadow;
};

class LookAndFeelManager
{
public:
    LookAndFeelManager()
    {
        myLookAndFeel = new CustomLookAndFeel();
        setupCustomLookAndFeelColours(*myLookAndFeel);
        myLookAndFeel->setUsingNativeAlertWindows(true);
        myLookAndFeel->setDefaultSansSerifTypefaceName("Times New Roman");
    }
    ~LookAndFeelManager() {}
    
    void setupCustomLookAndFeelColours (LookAndFeel& laf)
    {
        // Slider
        laf.setColour (Slider::thumbColourId, Colour (ALPHAGREEN));
        laf.setColour (Slider::textBoxOutlineColourId, Colours::transparentWhite);
        laf.setColour (Slider::rotarySliderFillColourId, Colour (0xff4d4d4d));
        laf.setColour (Slider::rotarySliderOutlineColourId, Colours::white);
        // Text Button
        laf.setColour (TextButton::buttonColourId, Colours::white);
        laf.setColour (TextButton::textColourOffId, Colour (ALPHAGREEN));
        
        laf.setColour (TextButton::buttonOnColourId, laf.findColour (TextButton::textColourOffId));
        laf.setColour (TextButton::textColourOnId, laf.findColour (TextButton::buttonColourId));
        
        // Combobox
        laf.setColour (ComboBox::buttonColourId, Colour (ALPHAGREEN));
        laf.setColour (ComboBox::backgroundColourId, Colour (0xff4d4d4d));
        laf.setColour (ComboBox::arrowColourId, Colour (ALPHAGREEN));
        laf.setColour (ComboBox::textColourId, Colours::white);
        
        // Popup Menu
        laf.setColour (PopupMenu::backgroundColourId, Colour(0xff4d4d4d));
        
        // Tooltip
        laf.setColour(TooltipWindow::ColourIds::backgroundColourId, GUIColours::Background);
        laf.setColour(TooltipWindow::ColourIds::outlineColourId, GUIColours::AlphaGreen);
        laf.setColour(TooltipWindow::ColourIds::textColourId, Colours::white);

        
        // Toggle Button
        laf.setColour(ToggleButton::ColourIds::textColourId, Colours::white);
        
    }
    
    CustomLookAndFeel* getMyLookAndFeel()
    {
        return myLookAndFeel;
    }
    
private:
    ScopedPointer<CustomLookAndFeel> myLookAndFeel;
    
};

#endif /* AlphaLiveLookAndFeel_hpp */
