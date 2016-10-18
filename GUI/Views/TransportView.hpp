//
//  TransportView.hpp
//  Alphalive 2
//
//  Created by Joseph Horton on 21/09/2016.
//
//

#ifndef TransportView_hpp
#define TransportView_hpp

#include <stdio.h>
#include "AppData.hpp"
#include "Alphalive2Engine.hpp"
#include "NeonButton.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

//class MetronomeTickerUI : public Component,
//public MasterClock::Listener
//{
//public:
//    MetronomeTickerUI(){ toggleValue = true;}
//    ~MetronomeTickerUI(){}
//    void paint(Graphics& g) override
//    {
//        mainColour = Colour(GUIColours::Main);
//        g.setColour(mainColour);
//        
//        g.drawEllipse(padding.getTopLeft().x-getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight(), 3);
//        
//        g.drawEllipse(padding.getCentreX()+getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight(), 3);
//        
//        g.drawEllipse(padding.getCentreX()+getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight(), 3);
//
//        g.drawEllipse(padding.getCentreX()+getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight(), 3);
//
//        
//        g.setColour(mainColour.withAlpha(float(0.5)));
//        
//        if (toggleValue)
//        {
//            g.fillEllipse(padding.getTopLeft().x-getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight());
//        }
//        else
//        {
//            g.fillEllipse(padding.getCentreX()+getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight());
//        }
//        
//        g.setColour(mainColour);
//        g.drawRect(1,1,getWidth()-2, getHeight()-2, 1);
//    }
//    void resized() override
//    {
//        padding = getBounds().transformedBy(AffineTransform::scale(0.8));
//        padding.setCentre(getWidth()/2.0, getHeight()/2.0);
//    }
//    
//    void clear()
//    {
//        toggleValue = true;
//        repaint();
//    }
//    
//    void toggle()
//    {
//        toggleValue = !toggleValue;
//        repaint();
//    }
//    
//    void barClockCallback() override
//    {
//        
//    }
//    
//    void stepClockCallback(const int currentPositionInLoop) override
//    {
//        toggle();
//    }
//    
//    void masterClockStopped() override
//    {
//        clear();
//    }
//    
//private:
//    Rectangle<int>padding;
//    bool toggleValue;
//    Colour mainColour;
//};


class TransportView : public Component
{
public:
    TransportView();
    ~TransportView();
    
    void paint(Graphics &g) override;
    void resized() override;
    
private:
    //ToggleButton playButton, stopButton, metroButton, recordButton;
    ScopedPointer<TextButton> playButton, stopButton, metroButton, recordButton;
    ScopedPointer<ComboBox> tempoBox;


    
};

#endif /* TransportView_hpp */
