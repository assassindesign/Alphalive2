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
#include "PadInspectorPanelBase.h"

class MidiIPanel : public PadInspectorPanelBase
{
public:
    MidiIPanel()
    {
        
    }
    ~MidiIPanel(){}
    void resized() override{}
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::blue);
    }
    
    void refreshData() override
    {
        
    }
    
    void padDataChangeCallback(const int changedData) override
    {
        
    }
private:
};

#endif /* MidiIPanel_hpp */
