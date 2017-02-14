//
//  AudioMeterButton.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 23/09/2016.
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
