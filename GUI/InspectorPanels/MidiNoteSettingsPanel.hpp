//
//  MidiNoteSettingsPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 11/08/2016.
//
//
//  Alphalive 2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  Alphalive 2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
    WeakReference<PadData> padData;
    
    ScopedPointer<ToggleSwitch> noteModeSwitch, triggerModeSwitch;
    
    ScopedPointer<NoteSelectKBComponent> noteSelectKeyboard;
    
    ScopedPointer<ButtonGrid> channelSwitcher;
    
    ScopedPointer<PadTriggerModePanel> triggerPanel;
};



#endif /* MidiNoteSettingsPanel_hpp */
