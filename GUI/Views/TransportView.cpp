//
//  TransportView.cpp
//  Alphalive 2
//
//  Created by Joseph Horton on 21/09/2016.
//
//

#include "TransportView.hpp"

TransportView::TransportView()
{
    playButton = new TextButton("Play");
    //playButton->addListener(this);
    addAndMakeVisible(playButton);
    
    stopButton = new TextButton("Stop");
    addAndMakeVisible(stopButton);
    
    recordButton = new TextButton("rec");
    addAndMakeVisible(recordButton);
    
    metroButton = new TextButton("metro");
    addAndMakeVisible(metroButton);
    
    tempoBox = new ComboBox();
    addAndMakeVisible(tempoBox);
}

TransportView::~TransportView()
{
    
}

void TransportView::paint(Graphics &g)
{
    g.setColour(Colours::hotpink);
    g.fillAll();
}

void TransportView::resized()
{
    float middle = getWidth() * 0.5;
    float buttonWidth = playButton->getWidth();
    
    stopButton->setBounds(middle - buttonWidth * 0.5, 0, 40, 40);
    playButton->setBounds(stopButton->getX() - 4, 0, 40, 40);
    recordButton->setBounds(stopButton->getRight() + 4, 0, 40, 40);
    
}