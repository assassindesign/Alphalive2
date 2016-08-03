//
//  MidiIPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
//
//

#ifndef MidiIPanel_hpp
#define MidiIPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"

class MidiIPanel : public AppDataListener
{
public:
    MidiIPanel()
    {
        
    }
    ~MidiIPanel(){}
    void resized(){}
    void paint(Graphics& g)
    {
        g.fillAll(Colours::blue);
    }
private:
};

#endif /* MidiIPanel_hpp */
