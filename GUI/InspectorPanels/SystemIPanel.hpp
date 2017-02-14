//
//  SystemIPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 04/08/2016.
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

#ifndef SystemIPanel_hpp
#define SystemIPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"

class SystemIPanel : public PadInspectorPanelBase
{
public:
    SystemIPanel()
    {
        
    }
    ~SystemIPanel() {}
    void resized() override{}
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::green);
    }
    
    void refreshData() override
    {
        
    }

    
    
private:
};

#endif /* SystemIPanel_hpp */
