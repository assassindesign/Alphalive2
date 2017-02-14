//
//  PadUI.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 12/07/2016.
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

#include "PadUI.hpp"
#include "MainComponent.h"


Pad::Pad(PadData* _padData) : padData(_padData)
{
    setColour(Colours::whitesmoke);
    emulatingPadPress = false;
    isSelected = false;
    halfWidth = 0;
    
    ringGlowEffect.setGlowProperties(0, Colours::whitesmoke);
    setComponentEffect(&ringGlowEffect);
    
    for (int i = 0; i < 4; i++)
    {
        selectDots.add(*new Rectangle<float>(0,0,4,4));
        selectDotsALines.add(*new Line<float>());
    }
}

Pad::~Pad()
{
    
}
void Pad::paint(Graphics& g)
{
    //g.fillAll(GUIColours::Background);
    if (padData != nullptr)
    {
        static float alpha;
        alpha = 0.15 + (padData->getPadPressure()/MAX_PRESSURE) * 0.75;
        
        
        //DBG(String(padData->getPadPressure()) + " : " + String(padData->getPadID()));
        if (emulatingPadPress) {
            alpha += 0.07;
        }
        
        g.setColour(padColour.withAlpha(alpha));
        
        g.fillEllipse(2, 2, getWidth()-4, getHeight()-4);
        
        if (padData->getVelocity() > 0)
        {
            if (padData->getNoteTriggerMode() != PadData::NoteTriggerModes::StandardNoteMode)
            {
                g.setColour(lightColour.withAlpha(float(0.5)));
                g.drawEllipse(6, 6, getWidth()-12, getHeight()-12, 3);
            }
            
            g.setColour(lightColour);
            g.drawEllipse(3, 3, getWidth()-6, getHeight()-6, 3);
            
        }
        else
        {
            g.setColour(padColour);
            g.drawEllipse(2, 2, getWidth()-4, getHeight()-4, 2);
            
        }
        
        g.drawFittedText(String(padData->getPadID()), getLocalBounds(), Justification::centred, 1);
        
        //DBG("PAD Paint: " + String(padData->getPadID()));
    }
    else
        jassertfalse;
    
    
}
void Pad::resized()
{
    halfWidth = getWidth()*0.5;
    //centerPoint = getScreenPosition();
    centerPoint.setXY(halfWidth, getHeight()/2.0);
    
//    selectDotsALines.getReference(0).setStart(centerPoint.getX(), centerPoint.getY());
//    selectDots.getReference(0).setPosition(selectDotsALines.getReference(0).getPointAlongLine(halfWidth + 4));
//    for (int i = 1; i < selectDotsALines.size(); i++)
//    {
//        selectDotsALines.getReference(i) = selectDotsALines.getReference(i-1);
//        selectDotsALines.getReference(i).applyTransform(AffineTransform::rotation(M_PI*0.5, centerPoint.x, centerPoint.y));
//        selectDots.getReference(i).setPosition(selectDotsALines.getReference(i).getPointAlongLine(halfWidth + 4));
//
//    }



}

void Pad::setColour(const Colour newColour)
{
    if (padColour != newColour)
    {
        actualColour = padColour = newColour;//.withMultipliedBrightness(1.5);
        lightColour = padColour.brighter().withAlpha(float(1.0));
        if (isSelected)
        {
            ringGlowEffect.setGlowProperties(1, actualColour);
        }
        

        repaint();
    }
}

void Pad::setSelected(const bool padIsSelected)
{
    isSelected = padIsSelected;
    
    if (padIsSelected)
    {
        //padColour = Colours::slategrey;
        ringGlowEffect.setGlowProperties(1.5, actualColour);
    }
    else
    {
        ringGlowEffect.setGlowProperties(0, actualColour);
        //padColour = actualColour;
    }
    //lightColour = padColour.brighter();
    
    repaint();
}

bool Pad::getSelected()
{
    return isSelected;
}

bool Pad::isPointInsideCircle(const Point<int> point)
{
    if (point.getDistanceFrom(centerPoint) < halfWidth)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Pad::setAsCurrentlyInspectedPad()
{
    AppData::Instance()->setCurrentlyInspectingPad(padData->getParentSphereID(), padData->getPadID());
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
    if (isPointInsideCircle(event.getMouseDownPosition()))
    {
        if (event.mods.isAltDown())
        {
            AppData::Instance()->getEnginePointer()->hitPad(padData->getPadID(), 110);
            emulatingPadPress = true;
        }
        else
        {
            //padInspector->setPadDataToInspect(padData);
            //AppData::Instance()->setCurrentlyInspectingPad(padData->getParentSphere()->getSphereID(), padData->getPadID());
        }
    }
    
 
}

void Pad::mouseDrag (const MouseEvent &event)
{
    if (isPointInsideCircle(event.getMouseDownPosition()))
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


