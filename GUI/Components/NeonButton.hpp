//
//  NeonButton.hpp
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

#ifndef NeonButton_hpp
#define NeonButton_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "GUIStyle.h"

/* 
    Awful custom button class, should be removed as soon as possible.
 */

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
    Font font;
};

#endif /* NeonButton_hpp */
