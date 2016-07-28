//
//  AppDataListeners.h
//  AlphaLive Midi
//
//  Created by Luke Saxton on 16/06/2016.
//
//

#ifndef AppDataListeners_h
#define AppDataListeners_h
#include "../JuceLibraryCode/JuceHeader.h"


class GUIRepaintListener : public Component
{
public:
    GUIRepaintListener(){};
    virtual ~GUIRepaintListener(){};
    void refreshUI() {repaint();};
    virtual void paint(Graphics&) = 0;
    virtual void resized() = 0;
};

#endif /* AppDataListeners_h */
