//
//  MidiPressureSettingsPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 20/09/2016.
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

#ifndef MidiPressureSettingsPanel_hpp
#define MidiPressureSettingsPanel_hpp

#include "AppData.hpp"
#include "PadInspectorPanelBase.h"
#include "ToggleSwitch.hpp"

/*
 Panel for configuring pad pressure settings
 */


class MidiPressureSettingsPanel : public PadInspectorPanelBase,
                                  public Button::Listener,
                                  public Slider::Listener
{
public:
    MidiPressureSettingsPanel();
    ~MidiPressureSettingsPanel();
    void refreshData() override;
    void padDataChangeCallback(const int changedData) override;
    void paint(Graphics &g) override;
    void resized() override;
    void buttonClicked(Button* button) override;

    void sliderValueChanged (Slider* slider) override;

private:
    //ToggleSwitch
    
    TextButton polyATButton, channelATButton, pbUpButton, pbDownButton, modWheelButton, midiCCButton;
    
    ToggleButton reverseButton, stickyButton;
    
    Rectangle<int> destinationsBox, rangeBox, settingsBox;
    Font labelFont;
    
    WeakReference<PadData> padData;
    
    ScopedPointer<Slider> rangeSlider;
};

#endif /* MidiPressureSettingsPanel_hpp */

//testing
