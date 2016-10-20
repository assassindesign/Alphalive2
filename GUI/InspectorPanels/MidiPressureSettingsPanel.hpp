//
//  MidiPressureSettingsPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 20/09/2016.
//
//

#ifndef MidiPressureSettingsPanel_hpp
#define MidiPressureSettingsPanel_hpp

#include "AppData.hpp"
#include "PadInspectorPanelBase.h"
#include "ToggleSwitch.hpp"

class MidiPressureSettingsPanel : public PadInspectorPanelBase,
                                  public Button::Listener
{
public:
    MidiPressureSettingsPanel();
    ~MidiPressureSettingsPanel();
    void refreshData() override;
    void padDataChangeCallback(const int changedData) override;
    void paint(Graphics &g) override;
    void resized() override;
    void buttonClicked(Button* button) override;

private:
    //ToggleSwitch
    
    ToggleButton polyATButton, channelATButton, pbUpButton, pbDownButton, modWheelButton, midiCCButton;
    
    Rectangle<int> destinationsBox, rangeBox, settingsBox;
    Font labelFont;
};

#endif /* MidiPressureSettingsPanel_hpp */
