//
//  PadUI.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 12/07/2016.
//
//

#include "PadUI.hpp"
#include "MainComponent.h"

Pad::Pad(PadData* _padData, MainContentComponent &ref) : padData(_padData)
{
    setColour(Colours::whitesmoke);
    emulatingPadPress = false;
    
    padInspector = ref.getPadInspector();
}

Pad::~Pad()
{
    
}
void Pad::paint(Graphics& g)
{
    //g.setColour(Colour(GUIColours::Background));
    //g.fillEllipse(2, 2, getWidth()-4, getHeight()-4);
    
    
    float alpha = 0.15 + (padData->getPadPressure()/127.0) * 0.75;
    if (emulatingPadPress) {
        alpha += 0.05;
    }
    
    g.setColour(padColour.withAlpha(alpha));
    
    g.fillEllipse(2, 2, getWidth()-4, getHeight()-4);
    
    
    if (padData->getVelocity() > 0)
    {
        g.setColour(lightColour);
        g.drawEllipse(3, 3, getWidth()-6, getHeight()-6, 3);

    }
    else
    {
        g.setColour(padColour);
        g.drawEllipse(2, 2, getWidth()-4, getHeight()-4, 2);

    }
    
}
void Pad::resized()
{

}

void Pad::setColour(const Colour newColour)
{
    if (padColour != newColour)
    {
        actualColour = padColour = newColour;//.withMultipliedBrightness(1.5);
        lightColour = padColour.brighter().withAlpha(float(1.0));
        repaint();
    }
}

void Pad::setSelected(const bool padIsSelected)
{
    isSelected = padIsSelected;
    
    if (padIsSelected)
    {
        padColour = Colours::slategrey;
    }
    else
    {
        padColour = actualColour;
    }
    repaint();
}

bool Pad::getSelected()
{
    return isSelected;
}


//Mouse Listener============================
void Pad::mouseEnter (const MouseEvent &event)
{
    
}

void Pad::mouseExit (const MouseEvent &event)
{
    
}

void Pad::mouseDown (const MouseEvent &event)
{
    if (event.mods.isAltDown())
    {
        AppData::Instance()->getEnginePointer()->hitPad(padData->getPadID(), 110);
        emulatingPadPress = true;
    }
    else
    {
        padInspector->setPadDataToInspect(padData);
    }
 
}

void Pad::mouseDrag (const MouseEvent &event)
{
    if (event.mods.isAltDown())
    {
        int pressureValue = abs((getHeight()/2) - event.y) * 3 + 1;
        if (pressureValue > MAX_PRESSURE)
        {
            pressureValue = MAX_PRESSURE;
        }
        
        AppData::Instance()->getEnginePointer()->pressPad(padData->getPadID(), pressureValue);

        emulatingPadPress = true;
        
    }
}

void Pad::mouseUp (const MouseEvent &event)
{
    if (emulatingPadPress)
    {
        AppData::Instance()->getEnginePointer()->hitPad(padData->getPadID(), 0);
        AppData::Instance()->getEnginePointer()->pressPad(padData->getPadID(), 0);
        emulatingPadPress = false;
    }


}

void Pad::mouseDoubleClick (const MouseEvent &event)
{
    
}


