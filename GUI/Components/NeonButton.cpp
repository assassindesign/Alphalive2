//
//  NeonButton.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 11/07/2016.
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

#include "NeonButton.hpp"

NeonButton::NeonButton() : Button("NeonButton")
{
    setColour(GUIColours::Background);
    font = GUIFonts::getMainFont();
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
    g.setFont(font);
    
    g.setFont(getHeight()*0.62);

    if (font.getStringWidthFloat(getButtonText()) >= getWidth())
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
