//
//  AudioIPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
//
//

#ifndef AudioIPanel_hpp
#define AudioIPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"

class AudioIPanel : public PadInspectorPanelBase
{
public:
    AudioIPanel()
    {
        
    }
    ~AudioIPanel() {}
    void resized() override{}
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::red);
    }
    
    void refreshData() override
    {
        
    }
private:
    PadInspectorPanelBase* panel;
};

#endif /* AudioIPanel_hpp */
