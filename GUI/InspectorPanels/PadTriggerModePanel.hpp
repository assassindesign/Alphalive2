//
//  PadTriggerModePanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 27/09/2016.
//
//

#ifndef PadTriggerModePanel_hpp
#define PadTriggerModePanel_hpp

#include "AppData.hpp"
#include "PadInspectorPanelBase.h"

class PadTriggerModePanel : public PadInspectorPanelBase,
                            public Button::Listener

{
public:
    PadTriggerModePanel();
    ~PadTriggerModePanel();
    void refreshData() override;
    void paint(Graphics &g) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    void padDataChangeCallback(const int changedData) override;
private:
    
    TextButton stdBtn, tglBtn, ltchBtn, trgrBtn;
    WeakReference<PadData> padData;
    Font font;
};
#endif /* PadTriggerModePanel_hpp */
