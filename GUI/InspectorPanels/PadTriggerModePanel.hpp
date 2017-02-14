//
//  PadTriggerModePanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 27/09/2016.
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

#ifndef PadTriggerModePanel_hpp
#define PadTriggerModePanel_hpp

#include "AppData.hpp"
#include "PadInspectorPanelBase.h"
#include "CustomIconButton.hpp"

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
    
    ScopedPointer<CustomIconButton> stdBtn, tglBtn, ltchBtn, trgrBtn;
    WeakReference<PadData> padData;
};
#endif /* PadTriggerModePanel_hpp */
