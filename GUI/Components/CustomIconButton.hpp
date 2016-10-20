//
//  CustomIconButton.hpp
//  Alphalive 2
//
//  Created by Joseph Horton on 20/10/2016.
//
//

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
