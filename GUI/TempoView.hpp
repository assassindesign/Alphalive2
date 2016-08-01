//
//  TempoView.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 16/06/2016.
//
//

#ifndef TempoView_hpp
#define TempoView_hpp

#include "AppData.hpp"
#include "Alphalive2Engine.hpp"
#include "NeonButton.hpp"

class MetronomeTickerUI : public Component,
                          public MasterClock::Listener
{
public:
    MetronomeTickerUI(){ toggleValue = true;}
    ~MetronomeTickerUI(){}
    void paint(Graphics& g) override
    {
        mainColour = Colour(GUIColours::Main);
        g.setColour(mainColour);
        
        g.drawEllipse(padding.getTopLeft().x-getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight(), 3);
        
        g.drawEllipse(padding.getCentreX()+getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight(), 3);
        
        g.setColour(mainColour.withAlpha(float(0.5)));
        
        if (toggleValue)
        {
            g.fillEllipse(padding.getTopLeft().x-getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight());
        }
        else
        {
            g.fillEllipse(padding.getCentreX()+getWidth()*0.03, padding.getTopLeft().y - getHeight()*0.02, padding.getWidth()/2.0, padding.getHeight());
        }
        
        g.setColour(mainColour);
        g.drawRect(1,1,getWidth()-2, getHeight()-2, 1);
    }
    void resized() override
    {
        padding = getBounds().transformedBy(AffineTransform::scale(0.8));
        padding.setCentre(getWidth()/2.0, getHeight()/2.0);
    }
    
    void clear()
    {
        toggleValue = true;
        repaint();
    }
    
    void toggle()
    {
        toggleValue = !toggleValue;
        repaint();
    }
    
    void barClockCallback() override
    {
        
    }
    
    void stepClockCallback(const int currentPositionInLoop) override
    {
        toggle();
    }
    
    void masterClockStopped() override
    {
        clear();
    }
    
private:
    Rectangle<int>padding;
    bool toggleValue;
    Colour mainColour;  
};

class TempoView : public Component,
                  public Button::Listener,
                  public Slider::Listener
{
public:
    TempoView();
    ~TempoView();
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void buttonClicked (Button*) override;
    
    void sliderValueChanged (Slider* slider) override;

private:
    NeonButton clockStartStopButton;
    
    Slider tempoSlider;
    
    Alphalive2Engine* engine;
    
    MetronomeTickerUI ticker;
};

#endif /* TempoView_hpp */
