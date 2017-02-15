//
//  MidiIPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
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

#ifndef MidiIPanel_hpp
#define MidiIPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"
#include "MidiNoteSettingsPanel.hpp"
#include "MidiPressureSettingsPanel.hpp"
#include "TabButton.hpp"

/* 
    Config panel for Pad Midi Settings
 */

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
    WeakReference<PadData> padData;
    bool panelEnabled;
    
    
};

#endif /* MidiIPanel_hpp */
