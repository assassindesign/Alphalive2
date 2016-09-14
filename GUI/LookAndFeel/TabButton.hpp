//
//  TabButton.hpp
//  AlphaLive GUI Components
//
//  Created by Joseph Horton on 30/08/2016.
//
//

#ifndef TabButton_hpp
#define TabButton_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

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
    ScopedPointer<Font> titleFont;
    bool tabToggleState = 0;
    bool lightToggleState = 1;
    Rectangle<float> toggleLightBox;
    Rectangle<float> toggleLightAlignBox;
    Rectangle<float> toggleHitBox;
    float x, y;

    ListenerList<TabButton::Listener> listeners;
};


#endif /* TabButton_hpp */
