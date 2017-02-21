//
//  GUIStyle.h
//  AlphaLive Midi
//
//  Created by Luke Saxton on 16/07/2016.
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

#ifndef GUIStyle_h
#define GUIStyle_h

#include "../JuceLibraryCode/JuceHeader.h"

/* 
    Static colour storage in the style of Juce::Colours.
 */

class GUIColours {
public:
    static const Colour Background, Main, AlphaGreen, PanelBackground, RackScrewAccent, RecButtonRed;
};

struct GUIFonts {
public:
    static Font getMainFont() {return Font("Roboto", "sans-serif", 14.0);}
};

#endif /* GUIColours_h */
