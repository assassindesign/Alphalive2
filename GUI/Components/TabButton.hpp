//
//  TabButton.hpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 30/08/2016.
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

#ifndef TabButton_hpp
#define TabButton_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "GUIStyle.h"

class TabButton : public Component
{
public:
    
    class Listener
    {
    public:
        virtual ~Listener(){};
        virtual void tabButtonClicked(const TabButton* button) = 0;
        virtual void tabLightClicked(const TabButton* button) = 0;
    private:
        
    };
    
    TabButton(String _labelText);
    ~TabButton();
    
    void paint(Graphics &g) override;
    void resized() override;
    void mouseDown (const MouseEvent &event) override;
    bool getTabToggleState();
    bool getLightToggleState();
    void setTabToggleState(bool newTabToggleState);
    void setLightToggleState(bool newLightToggleState);
    void drawYCentredEllipse(Graphics &g, float x, float y, float radius, float lineThickness);
    
    void addListener(TabButton::Listener* newListener);
    void removeListener(TabButton::Listener* listenerToRemove);
    
private:
    String labelText;
    bool tabToggleState = 0;
    bool lightToggleState = 1;
    Rectangle<float> toggleLightBox;
    Rectangle<float> toggleLightAlignBox;
    Rectangle<float> toggleHitBox;
    float x, y;

    ListenerList<TabButton::Listener> listeners;
};


#endif /* TabButton_hpp */
