//
//  CustomIconButton.cpp
//  Alphalive 2
//
//  Created by Joseph Horton on 20/10/2016.
//
//


#include "CustomIconButton.hpp"

CustomIconButton::CustomIconButton(IconPath iconToDisplay)
{
    switch (iconToDisplay){
        case StandardIcon:
            iconPath.loadPathFromData (IconVectorPaths::StandardIcon, sizeof (IconVectorPaths::StandardIcon));
            buttonName = "Standard";
            break;
        case ToggleIcon:
            iconPath.loadPathFromData (IconVectorPaths::ToggleIcon, sizeof (IconVectorPaths::ToggleIcon));
            buttonName = "Toggle";
            break;
        case TriggerIcon:
            iconPath.loadPathFromData (IconVectorPaths::TriggerIcon, sizeof (IconVectorPaths::TriggerIcon));
            buttonName = "Trigger";
            break;
        case LatchIcon:
            iconPath.loadPathFromData (IconVectorPaths::LatchIcon, sizeof (IconVectorPaths::LatchIcon));
            buttonName = "Latch";
            break;
        default:
            break;
            
    }
    
    iconPath.addRectangle(iconPath.getBounds().withSizeKeepingCentre(iconPath.getBounds().getWidth()+2, iconPath.getBounds().getHeight()+2));
    
    if (!iconPath.isEmpty())
    {
        button = new ShapeButton (buttonName, Colours::white, Colours::lightgrey, Colours::hotpink);
        button->setShape(iconPath, 0, 1, 0);
        addAndMakeVisible(button);
    } else
    {
        button = new ShapeButton ("Default", Colours::white, Colours::white, Colours::white);
    }
}

CustomIconButton::~CustomIconButton(){}

void CustomIconButton::paint(Graphics& g)
{
    g.setColour(Colours::white);
    //g.drawRect(0, 0, getWidth(), getHeight());
}
void CustomIconButton::resized()
{
    button->setBounds(getLocalBounds());
    
    //button->setBounds(3, 3, getWidth()-6, getHeight()-6);
    
}
void CustomIconButton::addListener(juce::Button::Listener *newListener)
{
    button->addListener(newListener);
}
void CustomIconButton::removeListener(Button::Listener *listenerToRemove)
{
    button->removeListener(listenerToRemove);
}
Button* CustomIconButton::getButtonObject()
{
    return button.get();
}
