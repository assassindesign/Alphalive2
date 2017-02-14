//
//  MainPadInspectorPanels.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 02/08/2016.
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

#ifndef MainPadInspectorPanels_hpp
#define MainPadInspectorPanels_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "GUIStyle.h"
#include "AppData.hpp"
#include "MidiIPanel.hpp"
#include "AudioIPanel.hpp"
#include "SystemIPanel.hpp"
#include "PadInspectorPanelBase.h"

class InspectorTopPanel : public PadInspectorPanelBase,
                          public Button::Listener,
                          public ButtonGrid::Listener,
                          public SettableTooltipClient
{
public:
    InspectorTopPanel();
    ~InspectorTopPanel();
    void resized() override;
    void paint (Graphics& g) override;
    void appDataChangeCallback(const int changedData) override;
    void padDataChangeCallback(const int changedData) override;
    void buttonClicked (Button* button) override;
    void refreshData() override;
    void setPanelEnabled(bool enabled);
    void buttonGridCallback(ButtonGrid* grid,
                            const int buttonID) override;
private:
    TextButton audioButton, midiButton, systemButton, powerButton;
    AudioIPanel audioPanel;
    MidiIPanel midiPanel;
    SystemIPanel sysPanel;
    bool panelEnabled = false;
    Array<Rectangle<float>> rackAccents;
    
    WeakReference<PadData> padData;
    
    ScopedPointer<ButtonGrid> groupSelector;
};

class InspectorBottomPanel : public PadInspectorPanelBase
{
public:
    InspectorBottomPanel();
    ~InspectorBottomPanel();
    void resized() override;
    void paint (Graphics& g) override;
    void refreshData() override;
private:
    Rectangle<int> groupTitleBox;
    Font groupTitleFont;
};

class MainInspectorPanel : public PadInspectorPanelBase
{
public:
    MainInspectorPanel();
    ~MainInspectorPanel();
    void resized() override;
    void paint (Graphics& g) override;
    void refreshData() override;
    void setTopPanelEnabled(bool enabled);

private:
    InspectorTopPanel top;
    InspectorBottomPanel bottom;
    
};

#endif /* PadInspectorPanels_hpp */

