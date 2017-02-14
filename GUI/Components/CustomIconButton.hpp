//
//  CustomIconButton.hpp
//  Alphalive 2
//
//  Created by Joseph Horton on 20/10/2016.
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

#ifndef CustomIconButton_hpp
#define CustomIconButton_hpp

#include "AppData.hpp"
#include "IconVectorPaths.h"

class CustomIconButton : public Component
{
public:
    enum IconPath {
        StandardIcon = 0,
        ToggleIcon,
        TriggerIcon,
        LatchIcon,
        FINAL_ICONPATH
    };
    
    CustomIconButton(IconPath iconToDisplay);
    ~CustomIconButton();
    
    void paint(Graphics& g);
    void resized();
    void addListener(juce::Button::Listener *newListener);
    void removeListener(Button::Listener *listenerToRemove);
    
    Button* getButtonObject();
    
private:
    ScopedPointer<ShapeButton> button;
    Path iconPath;
    String buttonName;
};


#endif /* CustomIconButton_hpp */
