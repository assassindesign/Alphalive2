//
//  AlphasphereConnectedButton.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 28/09/2016.
//
//

#ifndef AlphaSphereConnectedButton_hpp
#define AlphaSphereConnectedButton_hpp

#include "AppData.hpp"

class AlphaSphereConnectedButton : public Component,
                                   public AppDataListener
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
