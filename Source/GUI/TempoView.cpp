//
//  TempoView.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 16/06/2016.
//
//

#include "TempoView.hpp"

TempoView::TempoView()
{
    clockStartStopButton.setButtonText("PLAY");
    clockStartStopButton.addListener(this);
    addAndMakeVisible(&clockStartStopButton);
    
    tempoSlider.setRange(30, 240, 1);
    tempoSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    tempoSlider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_AutoDirection);
    tempoSlider.addListener(this);
    addAndMakeVisible(tempoSlider);
    
    
    engine = AppData::Instance()->getEnginePointer();
    
    engine->getMasterClockPointer()->addListener(&ticker);
    addAndMakeVisible(&ticker);
    
}

TempoView::~TempoView()
{
    
}

void TempoView::paint(Graphics& g)
{
    
}

void TempoView::resized()
{
    clockStartStopButton.setBounds(0, 0, getWidth()/2.0, getHeight()/2.0);
    tempoSlider.setBounds(clockStartStopButton.getBounds().translated(getWidth()/2.0, 0));
    ticker.setBounds(clockStartStopButton.getBounds().translated(0, getHeight()/2.0));
}

void TempoView::buttonClicked (Button* button)
{
    if (button == &clockStartStopButton)
    {
        if (!clockStartStopButton.getToggleState())
        {
            clockStartStopButton.setButtonText("STOP");
            clockStartStopButton.setToggleState(true, dontSendNotification);
            
            engine->getMasterClockPointer()->startClock();
        }
        else
        {
            clockStartStopButton.setButtonText("PLAY");
            clockStartStopButton.setToggleState(false, dontSendNotification);
            engine->getMasterClockPointer()->resetAndStopClock();

        }
        

    }
}

void TempoView::sliderValueChanged (Slider* slider)
{
    //DBG(slider->getValue());
    
    engine->getMasterClockPointer()->setTempo(slider->getValue());
    
}
