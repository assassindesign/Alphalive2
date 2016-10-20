//
//  AudioMeterButton.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 23/09/2016.
//
//

#include "AudioMeterButton.hpp"
#include "Alphalive2Engine.hpp"

AudioMeterButton::AudioMeterButton()
{
    engine = AppData::Instance()->getEnginePointer();
    startTimer(30);
}

AudioMeterButton::~AudioMeterButton()
{
    
}

void AudioMeterButton::resized()
{
    leftChannelBox.setBounds(0, 0, getWidth()*audioAverageL, getHeight()*0.45);
    rightChannelBox.setBounds(0, getHeight()*0.5, getWidth()* audioAverageR, getHeight()*0.45);
}

void AudioMeterButton::paint(Graphics& g)
{
    g.fillAll(Colours::hotpink);
    
    if (audioAverageL >= 1)
    {
        g.setColour(Colours::red);

    }
    else
    {
        g.setColour(Colours::green);
    }
    g.fillRect(leftChannelBox);

    
    if (audioAverageR >= 1)
    {
        g.setColour(Colours::red);
    }
    else
    {
        g.setColour(Colours::green);
    }
    g.fillRect(rightChannelBox);
}

void AudioMeterButton::timerCallback()
{
    audioAverageL = engine->getAudioOutputAverage(0);
    audioAverageR = engine->getAudioOutputAverage(1);

    resized();
    repaint();
}


void AudioMeterButton::setToggleState(const bool newState)
{
    toggleState = newState;
    repaint();
}

bool AudioMeterButton::getToggleState()
{
    return toggleState;
}