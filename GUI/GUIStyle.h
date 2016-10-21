//
//  GUIStyle.h
//  AlphaLive Midi
//
//  Created by Luke Saxton on 16/07/2016.
//
//

#ifndef GUIStyle_h
#define GUIStyle_h

#include "../JuceLibraryCode/JuceHeader.h"

class GUIColours {
public:
    static const Colour Background, Main, AlphaGreen, PanelBackground, RackScrewAccent, RecButtonRed;
};

struct GUIFonts {
public:
    static Font getMainFont() {return Font("Roboto", "sans-serif", 14.0);}
};

#endif /* GUIColours_h */
