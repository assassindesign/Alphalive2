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
#include "MidiNoteSettingsPanel.hpp"
#include "MidiPressureSettingsPanel.hpp"
#include "TabButton.hpp"



class MidiIPanel : public PadInspectorPanelBase,
                   public TabButton::Listener
{
public:
    MidiIPanel();
    ~MidiIPanel();
    void resized() override;
    void paint(Graphics& g) override;
    void refreshData() override;
    void padDataChangeCallback(const int changedData) override;
    
    void tabButtonClicked(const TabButton* button) override;
    void tabLightClicked(const TabButton* button) override;
    
private:
    ScopedPointer<TabButton> noteButton, pressureButton;

    
    ScopedPointer<MidiNoteSettingsPanel> midiNoteSettingsPanel;
    ScopedPointer<MidiPressureSettingsPanel> midiPressureSettingsPanel;
    // scoped pointer to pressure settings panel
    PadData* padData;
    bool panelEnabled;
    
    
};

#endif /* MidiIPanel_hpp */
