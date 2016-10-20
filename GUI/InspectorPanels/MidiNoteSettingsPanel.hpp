//
//  MidiNoteSettingsPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 11/08/2016.
//
//

#ifndef MidiNoteSettingsPanel_hpp
#define MidiNoteSettingsPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"
#include "GUIStyle.h"
#include "Alphalive2Engine.hpp"
#include "NoteSelectKBComponent.hpp"
#include "ToggleSwitch.hpp"
#include "ButtonGrid.hpp"
#include "PadTriggerModePanel.hpp"

class MidiNoteSettingsPanel : public PadInspectorPanelBase,
                              public ToggleSwitch::Listener,
                              public ButtonGrid::Listener
{
public:

    MidiNoteSettingsPanel();
    ~MidiNoteSettingsPanel();
    void resized() override;
    void paint(Graphics& g) override;
    void refreshData() override;
    void padDataChangeCallback(const int changedData) override;
    void setCurrentlyViewingPanel(const int childPanelID);
    void toggleSwitchChanged(const ToggleSwitch* toggle) override;
    
    void buttonGridCallback(ButtonGrid* grid, const int buttonID) override;

private:
    PadData* padData;
    
    ScopedPointer<ToggleSwitch> noteModeSwitch, triggerModeSwitch;
    
    ScopedPointer<NoteSelectKBComponent> noteSelectKeyboard;
    
    ScopedPointer<ButtonGrid> channelSwitcher;
    
    ScopedPointer<PadTriggerModePanel> triggerPanel;
};



#endif /* MidiNoteSettingsPanel_hpp */
