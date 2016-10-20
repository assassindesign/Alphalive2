//
//  SystemIPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 04/08/2016.
//
//

#ifndef SystemIPanel_hpp
#define SystemIPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"

class SystemIPanel : public PadInspectorPanelBase
{
public:
    SystemIPanel()
    {
        
    }
    ~SystemIPanel() {}
    void resized() override{}
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::green);
    }
    
    void refreshData() override
    {
        
    }

    
    
private:
};

#endif /* SystemIPanel_hpp */
