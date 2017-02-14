//
//  AlphasphereConnectedButton.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 28/09/2016.
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

#ifndef AlphaSphereConnectedButton_hpp
#define AlphaSphereConnectedButton_hpp

#include "AppData.hpp"

class AlphaSphereConnectedButton : public Component,
                                   public AppDataListener,
                                   public SettableTooltipClient
{
public:
    AlphaSphereConnectedButton();
    ~AlphaSphereConnectedButton();
    
    void paint(Graphics& g) override;
    void resized () override;
    void mouseDown(const MouseEvent& event) override;
    void appDataChangeCallback(const int changedData) override;

private:
    bool alphaSphereConnected = false;
};
#endif /* AlphaSphereConnectedButton_hpp */
