//
//  NeonButton.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 11/07/2016.
//
//

#include "NeonButton.hpp"

NeonButton::NeonButton() : Button("NeonButton")
{
    setColour(Colour(GUIColours::Background));
    font = new Font("Eurostile", 14, Font::plain);
}

NeonButton::~NeonButton()
{
    
}

void NeonButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    
    if (isMouseOverButton)
    {
        g.setColour(hoverColour);
    }
    else
    {
        g.setColour(Colour(GUIColours::Background).withBrightness(Colour(GUIColours::Background).getBrightness()+0.2));
    }
    
    if (isButtonDown) {
        g.setColour(downColour);
    }
   
    g.fillRect(1,1,getWidth()-2, getHeight()-2);
    
    
    g.setColour(neonColour);
    //g.drawRect(1, 1, getWidth()-2, getHeight()-2, 1);
    //g.drawRoundedRectangle(getBounds().toFloat(), 5, 3);
    
    g.setColour(Colours::white);
    g.setFont(*font);
    
    g.setFont(getHeight()*0.62);

    if (font->getStringWidthFloat(getButtonText()) >= getWidth())
    {
        g.setFont(getWidth()*0.62);
    }
  
    g.drawText(getButtonText(), 0, 3, getWidth(), getHeight()-3, Justification::centred);
    
    //g.fillAll(Colours::hotpink);
}


void NeonButton::setColour(const Colour newColour)
{
    neonColour = newColour;
    hoverColour = neonColour.brighter().withAlpha(uint8(0x99));
    downColour = neonColour.darker();
    
    repaint();
}
