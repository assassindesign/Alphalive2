//
//  AlphaLiveLookAndFeel.cpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 10/08/2016.
//
//

#include "AlphaLiveLookAndFeel.hpp"

CustomLookAndFeel::CustomLookAndFeel()
{
    knobShadow.offset = *new Point<int> (3,3);
}

CustomLookAndFeel::~CustomLookAndFeel()
{
    
}

void CustomLookAndFeel::drawRoundThumb (Graphics& g, const float x, const float y,
                     const float diameter, const Colour& colour, float outlineThickness)
{
    const Rectangle<float> a (x, y, diameter, diameter);
    const float halfThickness = outlineThickness * 0.5f;
    
 
    
    Path p;
    p.addEllipse (x + halfThickness, y + halfThickness, diameter - outlineThickness, diameter - outlineThickness);
    
    Path middleFill;
    middleFill.addEllipse (2, 1.5, 5, 5);
    
    Path toggleOff;
    toggleOff.loadPathFromData (CustomVectorPaths::ToggleOff, sizeof (CustomVectorPaths::ToggleOff));
    toggleOff.scaleToFit(x + halfThickness, y + halfThickness, diameter - outlineThickness, diameter - outlineThickness, true);


    
//    const DropShadow ds (Colours::black, 1, Point<int> (0, 0));
//    ds.drawForPath (g, p);
    
    g.setColour (Colours::white);
//    g.fillPath (toggleOff);
    
    //g.setColour(Colour (0xff4d4d4d));
    //g.fillPath(middleFill);
    
    //g.setColour (colour.brighter());
    //g.strokePath (p, PathStrokeType (outlineThickness));
}


void CustomLookAndFeel::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                           bool isMouseOverButton, bool isButtonDown)
{
    Colour baseColour (backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                       .withMultipliedAlpha (button.isEnabled() ? 0.9f : 0.5f));
    
    if (isButtonDown || isMouseOverButton)
        baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f);
    
    const bool flatOnLeft   = button.isConnectedOnLeft();
    const bool flatOnRight  = button.isConnectedOnRight();
    const bool flatOnTop    = button.isConnectedOnTop();
    const bool flatOnBottom = button.isConnectedOnBottom();
    
    const float width  = button.getWidth() - 1.0f;
    const float height = button.getHeight() - 1.0f;
    
    if (width > 0 && height > 0)
    {
        const float cornerSize = jmin (15.0f, jmin (width, height) * 0.45f);
        const float lineThickness = cornerSize * 0.1f;
        const float halfThickness = lineThickness * 0.5f;
        
        Path outline;
        outline.addRoundedRectangle (0.5f + halfThickness, 0.5f + halfThickness, width - lineThickness, height - lineThickness,
                                     cornerSize, cornerSize,
                                     ! (flatOnLeft  || flatOnTop),
                                     ! (flatOnRight || flatOnTop),
                                     ! (flatOnLeft  || flatOnBottom),
                                     ! (flatOnRight || flatOnBottom));
        
        const Colour outlineColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId
                                                       : TextButton::textColourOffId));
        
        g.setColour (baseColour);
        g.fillPath (outline);
        
        if (! button.getToggleState())
        {
            g.setColour (outlineColour);
            g.strokePath (outline, PathStrokeType (lineThickness));
        }
    }
}

void CustomLookAndFeel::drawTickBox (Graphics& g, Component& component,
                  float x, float y, float w, float h,
                  bool ticked,
                  bool isEnabled,
                  bool isMouseOverButton,
                  bool isButtonDown)
{
    
    if (ticked)
    {
        Path tick;
        tick.addEllipse (2, 1.5, 5, 5);
        g.setColour (isEnabled ? findColour (TextButton::buttonOnColourId) : Colours::grey);
        
        Path toggleOn;
        toggleOn.loadPathFromData (CustomVectorPaths::ToggleOn, sizeof (CustomVectorPaths::ToggleOn));
        toggleOn.scaleToFit(x, y, w, h, true);
        g.setColour (isEnabled ? findColour (TextButton::buttonOnColourId) : Colours::grey);

        g.fillPath (toggleOn);
    }
    
    else
    {
        Path toggleOff;
        toggleOff.loadPathFromData (CustomVectorPaths::ToggleOff, sizeof (CustomVectorPaths::ToggleOff));
        toggleOff.scaleToFit(x, y, w, h, true);
        
        g.setColour (Colour (0xff4d4d4d));
        g.fillPath (toggleOff);
        
    }
}

void CustomLookAndFeel::drawToggleButton (Graphics& g, ToggleButton& button, bool isMouseOverButton, bool isButtonDown)
{
        const int tickWidth = jmin (20, button.getHeight() - 4);
    
    drawTickBox (g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
                 (float) tickWidth, (float) tickWidth,
                 button.getToggleState(),
                 button.isEnabled(),
                 isMouseOverButton,
                 isButtonDown);
    
    g.setColour (button.findColour (ToggleButton::textColourId));
    g.setFont (jmin (15.0f, button.getHeight() * 0.6f));
    
    if (! button.isEnabled())
        g.setOpacity (0.5f);
    
    const int textX = tickWidth + 5;
    
    g.drawFittedText (button.getButtonText(),
                      textX, 4,
                      button.getWidth() - textX - 2, button.getHeight() - 8,
                      Justification::centredLeft, 10);
}

void CustomLookAndFeel::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, float minSliderPos, float maxSliderPos,
                                            const Slider::SliderStyle style, Slider& slider)
{
    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
    
    Colour knobColour = Colour(ALPHAGREEN);
    
    
    const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;
    
    if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
    {
        float kx, ky;
        
        if (style == Slider::LinearVertical)
        {
            kx = x + width * 0.5f;
            ky = sliderPos;
        }
        else
        {
            kx = sliderPos;
            ky = y + height * 0.5f;
        }
        
        drawGlassSphere (g,
                         kx - sliderRadius,
                         ky - sliderRadius,
                         sliderRadius * 2.0f,
                         knobColour, outlineThickness);
    }
    else
    {
        if (style == Slider::ThreeValueVertical)
        {
            drawGlassSphere (g, x + width * 0.5f - sliderRadius,
                             sliderPos - sliderRadius,
                             sliderRadius * 2.0f,
                             knobColour, outlineThickness);
        }
        else if (style == Slider::ThreeValueHorizontal)
        {
            drawGlassSphere (g,sliderPos - sliderRadius,
                             y + height * 0.5f - sliderRadius,
                             sliderRadius * 2.0f,
                             knobColour, outlineThickness);
        }
        
        if (style == Slider::TwoValueVertical || style == Slider::ThreeValueVertical)
        {
            const float sr = jmin (sliderRadius, width * 0.4f);
            
            drawPointer (g, jmax (0.0f, x + width * 0.5f - sliderRadius * 2.0f),
                              minSliderPos - sliderRadius,
                              sliderRadius * 2.0f, knobColour, outlineThickness, 1);
            
            drawPointer (g, jmin (x + width - sliderRadius * 2.0f, x + width * 0.5f), maxSliderPos - sr,
                              sliderRadius * 2.0f, knobColour, outlineThickness, 3);
        }
        else if (style == Slider::TwoValueHorizontal || style == Slider::ThreeValueHorizontal)
        {
            const float sr = jmin (sliderRadius, height * 0.4f);
            
            drawPointer (g, minSliderPos - sr,
                              jmax (0.0f, y + height * 0.5f - sliderRadius * 2.0f),
                              sliderRadius * 2.0f, knobColour, outlineThickness, 2);
            
            drawPointer (g, maxSliderPos - sliderRadius,
                              jmin (y + height - sliderRadius * 2.0f, y + height * 0.5f),
                              sliderRadius * 2.0f, knobColour, outlineThickness, 4);
        }
    }
}

void CustomLookAndFeel::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                       float sliderPos, float minSliderPos, float maxSliderPos,
                       const Slider::SliderStyle style, Slider& slider)
{
    g.fillAll (slider.findColour (Slider::backgroundColourId));
    
    if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
    {
        const float fx = (float) x, fy = (float) y, fw = (float) width, fh = (float) height;
        
        Path p;
        
        if (style == Slider::LinearBarVertical)
            p.addRectangle (fx, sliderPos, fw, 1.0f + fh - sliderPos);
        else
            p.addRectangle (fx, fy, sliderPos - fx, fh);
        
        
        Colour baseColour (slider.findColour (Slider::rotarySliderFillColourId)
                           .withMultipliedSaturation (slider.isEnabled() ? 1.0f : 0.5f)
                           .withMultipliedAlpha (0.8f));
        
        g.setColour (baseColour);
        g.fillPath (p);
        
        const float lineThickness = jmin (15.0f, jmin (width, height) * 0.45f) * 0.1f;
        g.drawRect (slider.getLocalBounds().toFloat(), lineThickness);
    }
    else
    {
        drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

void CustomLookAndFeel::drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                 float /*sliderPos*/,
                                 float /*minSliderPos*/,
                                 float /*maxSliderPos*/,
                                 const Slider::SliderStyle style, Slider& slider)
{
    const float sliderRadius = getSliderThumbRadius (slider) - 5.0f;
    Path on, off;
    
    if (style == Slider::SliderStyle::LinearHorizontal)
    {
        const float iy = y + height * 0.5f - sliderRadius * 0.5f;
        Rectangle<float> r (x - sliderRadius * 0.5f, iy, width + sliderRadius, sliderRadius);
        const float onW = r.getWidth() * ((float) slider.valueToProportionOfLength (slider.getValue()));
        on.addRectangle (r.removeFromLeft (onW));
        off.addRectangle (r);
    }
    else if (style == Slider::SliderStyle::TwoValueHorizontal)
    {
        const float iy = y + height * 0.5f - sliderRadius * 0.5f;
        Rectangle<float> r (x - sliderRadius * 0.5f, iy, width + sliderRadius, sliderRadius);
        
        const float onW = r.getWidth() * ((float) slider.valueToProportionOfLength (slider.getMaxValue())) + r.getX();
        const float onX = (r.getWidth() * ((float) slider.valueToProportionOfLength (slider.getMinValue())) + r.getX());

        //on.addRectangle (r.removeFromLeft (onW));
        
        
        off.addRectangle (r);
        on.addRectangle(onX, r.getY(), onW - onX, r.getHeight());
        
    }
    else
    {
        const float ix = x + width * 0.5f - sliderRadius * 0.5f;
        Rectangle<float> r (ix, y - sliderRadius * 0.5f, sliderRadius, height + sliderRadius);
        const float onH = r.getHeight() * ((float) slider.valueToProportionOfLength (slider.getValue()));
        
        on.addRectangle (r.removeFromBottom (onH));
        off.addRectangle (r);
    }
    g.setColour (slider.findColour (Slider::trackColourId));
    g.fillPath (off);
    
    g.setColour (slider.findColour (Slider::thumbColourId));
    g.fillPath (on);
    
}

void CustomLookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                       float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    const float radius = jmin (width / 2, height / 2) - 2.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
    
    if (slider.isEnabled())
        g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
    else
        g.setColour (Colour (0x80808080));
    
    Rectangle<float> r (rx, ry, rw, rw);
//AffineTransform t (AffineTransform::rotation (angle, r.getCentreX(), r.getCentreY()));
    
    {
//        Path knobOutline;
//        knobOutline.addEllipse(rx - 0.7, ry - 0.7, rw + 1.4, rw + 1.4);
//        g.setColour(juce::Colours::white);
//        g.fillPath(knobOutline);
        
        Path filledArc;
        filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, 0.0);
        g.setColour(Colour(ALPHAGREEN).withAlpha((float (0.99))));
        g.fillPath (filledArc);
        
        Path knob;
        knob.addEllipse(rx, ry, rw, rw);
        //knobShadow.drawForPath(g, knob);
        g.setColour(Colour(ALPHAGREEN).withAlpha((float (0.99))));
        //g.fillPath(knob);
        
        //======================================================= //make this work good and look good too you chump
        Rectangle<float> r3 (r * 0.1f);

        Path knobInside;
        knobInside.addEllipse(rx + 2.0, ry + 2.0, rw - 4.0, rw - 4.0);
        g.setColour(slider.findColour(Slider::rotarySliderFillColourId));
        g.fillPath(knobInside);
        //========================================================

        
        Path needle;
        Rectangle<float> r2 (r * 0.1f);
        needle.addRectangle(r2.withPosition((Point<float>(r.getCentreX() - (r2.getWidth() / 2.0f), r.getY()))));
        g.setColour (juce::Colours::white);
        g.fillPath (needle, AffineTransform::rotation(angle, r.getCentreX(), r.getCentreY()));
    }
    
    {
      //  const float lineThickness = jmin (15.0f, jmin (width, height) * 0.45f) * 0.1f;
//        Path outlineArc;
//        outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.0);
//        g.strokePath (outlineArc, PathStrokeType (lineThickness));

    }
}

void CustomLookAndFeel::drawComboBox (Graphics& g, int width, int height, const bool /*isButtonDown*/,
                                   int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box)
{
    Path comboBoxArrowPath;
    comboBoxArrowPath.loadPathFromData (CustomVectorPaths::DotsArrow, sizeof (CustomVectorPaths::DotsArrow));
    comboBoxArrowPath.scaleToFit(buttonX + 8, buttonY + 10, 20, 20, true);
    
    g.fillAll (box.findColour (ComboBox::backgroundColourId));
    const Colour buttonColour (box.findColour (ComboBox::buttonColourId));
    
    if (box.isPopupActive())
    {
        comboBoxArrowPath.applyTransform(juce::AffineTransform::rotation(M_PI, comboBoxArrowPath.getBounds().getCentre().x, comboBoxArrowPath.getBounds().getCentre().y));
    }
    
    if (box.isMouseOverOrDragging() && box.hasKeyboardFocus (false))    //revert to box.isEnabled()
    {
        g.setColour (buttonColour);
        g.drawRect (0, 0, width, height, 2);
    }
    else
    {
        g.setColour (box.findColour (ComboBox::outlineColourId));
        g.drawRect (0, 0, width, height);
    }
    
//    const float arrowX = 0.3f;
//    const float arrowH = 0.2f;
//    
//    Path p;
//    
//    
//    p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.45f - arrowH),
//                   buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.45f,
//                   buttonX + buttonW * arrowX,          buttonY + buttonH * 0.45f);
//    
//    p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.55f + arrowH),
//                   buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.55f,
//                   buttonX + buttonW * arrowX,          buttonY + buttonH * 0.55f);
    
    g.setColour (box.findColour (ComboBox::arrowColourId).withMultipliedAlpha (box.isEnabled() ? 1.0f : 0.3f));
    g.fillPath (comboBoxArrowPath);
}


//Font LookAndFeel_V2::getComboBoxFont (ComboBox& box)
//{
//    return Font (jmin (15.0f, box.getHeight() * 0.85f));
//}

void CustomLookAndFeel::positionComboBoxText (ComboBox& box, Label& label)
{
    //OG ONE
    label.setBounds (1, 1,
                     box.getWidth() + 3 - box.getHeight(),
                     box.getHeight() - 2);
    
    label.setFont (getComboBoxFont (box));
}

void CustomLookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
    g.setColour (findColour (PopupMenu::backgroundColourId));
    //g.setColour (Colour (juce::Colours::darkgrey));
    g.fillRect (0, 0, width, height);
    
    
}

//void CustomLookAndFeel::drawLabel (Graphics& g, Label& label)
//{
//
//}


void CustomLookAndFeel::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                        const bool isSeparator, const bool isActive,
                                        const bool isHighlighted, const bool isTicked,
                                        const bool hasSubMenu, const String& text,
                                        const String& shortcutKeyText,
                                        const Drawable* icon, const Colour* const textColourToUse)
{
    if (isSeparator)
    {
        Rectangle<int> r (area.reduced (5, 0));
        r.removeFromTop (r.getHeight() / 2 - 1);
        
        g.setColour (Colour (0x33000000));
        g.fillRect (r.removeFromTop (1));
        
        g.setColour (Colour (0x66ffffff));
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        Colour textColour (findColour (PopupMenu::textColourId));
        
        if (textColourToUse != nullptr)
            textColour = *textColourToUse;
        
        Rectangle<int> r (area.reduced (1));
        
        if (isHighlighted)
        {
            g.setColour (Colour(ALPHAGREEN)); //(findColour (PopupMenu::highlightedBackgroundColourId));
            
            g.fillRect (r);
            
            g.setColour (findColour (PopupMenu::highlightedTextColourId));
            //g.setColour (Colour (juce::Colours::white));

        }
        else
        {
            g.setColour (Colour (juce::Colours::white));
        }
        
        if (!isActive)
        {
            g.setOpacity (0.3f);
        }
        
        Font font (getPopupMenuFont());
        
        const float maxFontHeight = area.getHeight() / 1.3f;
        
        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);
        
        g.setFont (font);
        
        Rectangle<float> textIndent (r.removeFromLeft ((r.getHeight() * 5) / 32).reduced (3).toFloat());
        
        if (hasSubMenu)
        {
            const float arrowH = 0.6f * getPopupMenuFont().getAscent();
            
            const float x = (float) r.removeFromRight ((int) arrowH).getX();
            const float halfH = (float) r.getCentreY();
            
            Path p;
            p.addTriangle (x, halfH - arrowH * 0.5f,
                           x, halfH + arrowH * 0.5f,
                           x + arrowH * 0.6f, halfH);
            
            g.fillPath (p);
        }
        
        r.removeFromRight (3);
        g.drawFittedText (text, r, Justification::centredLeft, 1);
        
        if (shortcutKeyText.isNotEmpty())
        {
            Font f2 (font);
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);
            
            g.drawText (shortcutKeyText, r, Justification::centredRight, true);
        }
    }
}

//=======================================================================

void CustomLookAndFeel::drawPointer (Graphics& g,
                                       const float x, const float y, const float diameter,
                                       const Colour& colour, const float outlineThickness,
                                       const int direction)
{
    if (diameter <= outlineThickness)
        return;
    
    Path p;
    p.addTriangle(x + diameter * 0.5f, y, x + diameter, y + diameter, x, y + diameter);
    
    p.applyTransform (AffineTransform::rotation (direction * (float_Pi * 0.5f), x + diameter * 0.5f, y + diameter * 0.5f));
    
    g.setColour(findColour(Slider::ColourIds::thumbColourId));
    g.fillPath(p);
    
    g.setColour (Colours::black.withAlpha (0.5f * colour.getFloatAlpha()));
    g.strokePath (p, PathStrokeType (outlineThickness));
}
