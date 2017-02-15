//
//  AudioIPanel.hpp
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

#ifndef AudioIPanel_hpp
#define AudioIPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"

/* 
    Placeholder class for audio settings inspector panel
 */

class AudioIPanel : public PadInspectorPanelBase
{
public:
    AudioIPanel()
    {
        
    }
    ~AudioIPanel() {}
    void resized() override{}
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::red);
    }
    
    void refreshData() override
    {
        
    }
    
private:
    PadInspectorPanelBase* panel;
};

#endif /* AudioIPanel_hpp */
