//
//  SphereView.cpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 14/06/2016.
//
//

#include "SphereView.hpp"
#define NUM_SEGEMENTS 16
#define NUM_PADS 48

#include "MainComponent.h"


SphereView::SphereView(const int _sphereID, MainContentComponent &ref) : sphereID(_sphereID), mainComponent(&ref)
{
    sphereData = AppData::Instance()->getSphereData(sphereID);
    
    for (int i = 0 ; i < NUM_SEGEMENTS; i++)
    {
        segmentLines.add(new Line<float>());
    }
    
    for (int i = 0 ; i < sphereData->padData.size(); i++)
    {
        pads.add(new Pad(sphereData->padData[i], ref));
        pads.getLast()->setSize(50,50);
        
        addAndMakeVisible(pads.getLast());
        
        sphereData->padData[i]->addListener(pads.getLast());
        
        pads.getLast()->addMouseListener(this, true);
    }
    
    numRows = 6;
    
    for (int i = 0 ; i < numRows; i++)
    {
        rowRadii.add(0.0);
    }
    
    
    //set size and radius ratios for each row
    padSizeModifiers.add(0.68);
    padSizeModifiers.add(1);
    padSizeModifiers.add(1.3);
    padSizeModifiers.add(1.6);
    padSizeModifiers.add(1.3);
    padSizeModifiers.add(0.9);
    
    
    rowRadiusModifiers.add(0.166);
    rowRadiusModifiers.add(0.3);
    rowRadiusModifiers.add(0.48);
    rowRadiusModifiers.add(0.66);
    rowRadiusModifiers.add(0.81);
    rowRadiusModifiers.add(0.91);
    
    for (int i = 0; i < numRows; i++)
    {
        ratioSliders.add(new Slider());
        ratioSliders.getLast()->addListener(this);
        ratioSliders.getLast()->setRange(0.1, 2.5);
        ratioSliders.getLast()->setValue(padSizeModifiers[i]);
        addAndMakeVisible(ratioSliders.getLast());
    }
    
    for (int i = 0; i < numRows; i++)
    {
        ratioSliders.add(new Slider());
        ratioSliders.getLast()->addListener(this);
        ratioSliders.getLast()->setRange(0.1, 1);
        ratioSliders.getLast()->setValue(rowRadiusModifiers[i]);
        addAndMakeVisible(ratioSliders.getLast());

    }
    colourSelector.addChangeListener(this);
    colourSelector.setOpaque(true);
    colourSelector.setSize(300, 300);
    colourSelector.setTopLeftPosition(200, 200);
    addAndMakeVisible(colourSelector);
    colourSelector.addToDesktop(ComponentPeer::StyleFlags::windowHasCloseButton || ComponentPeer::StyleFlags::windowHasTitleBar);


}

SphereView::~SphereView()
{
    
}

void SphereView::sliderValueChanged (Slider* slider)
{
    for (int i = 0; i < ratioSliders.size(); i++)
    {
        if (slider == ratioSliders[i])
        {
            if (i < 6)
            {
                padSizeModifiers.set(i, slider->getValue());
            }
            else
            {
                rowRadiusModifiers.set(i - 6, slider->getValue());
            }
            
            resized();
        }
    }
}


void SphereView::paint(Graphics& g)
{
  
    
    g.setColour(*new Colour(0x33FFFFFF));

    
    g.drawEllipse(segmentLines[12]->getEnd().x+3, segmentLines[0]->getEnd().y+3, (segmentLines[0]->getLength()*2.0)-6, (segmentLines[0]->getLength()*2.0)-6, 3);
    
    Colour backgroundColour = Colour(GUIColours::Background);
    backgroundColour = backgroundColour.withBrightness(backgroundColour.getBrightness()+0.03);
    g.setColour(backgroundColour);
    g.fillEllipse(segmentLines[12]->getEnd().x+3, segmentLines[0]->getEnd().y+3, (segmentLines[0]->getLength()*2.0)-6, (segmentLines[0]->getLength()*2.0)-6);
    
    for (int i = numRows; i >= 0; i--)
    {
        backgroundColour = backgroundColour.withBrightness(backgroundColour.getBrightness()+0.02);
        g.setColour(backgroundColour);
        g.fillEllipse(segmentLines[12]->getPointAlongLine(rowRadii[i]).getX()+3, segmentLines[0]->getPointAlongLine(rowRadii[i]).getY(), rowRadii[i]*2-6, rowRadii[i]*2-6);
    }
    
    g.setColour(*new Colour(0x15FFFFFF));
    
    for (int i = 0 ; i < segmentLines.size(); i++)
    {
        g.drawLine(*segmentLines[i],1);
        
    }
    
}

void SphereView::resized()
{
    //set center point;
    centerPoint.setXY(getWidth()/2.0, getHeight()/2.0);

    //scale sphere down to fit inside smallest dimension.
    float trimLineBy = 0;;
    
    if (centerPoint.x < centerPoint.y)
    {
        trimLineBy = centerPoint.y - centerPoint.x;
    }
    
    
    //set start point and length for all lines
    for (int i = 0 ; i < segmentLines.size(); i++)
    {
        segmentLines[i]->setStart(centerPoint.x, centerPoint.y);
        segmentLines[i]->setEnd(centerPoint.x, 0 + trimLineBy);
    }


    
    //rotate lines to position them into a circle of segments.
    for (int i = 1 ; i < segmentLines.size(); i++)
    {
        segmentLines[i]->applyTransform(AffineTransform::rotation((M_PI/(NUM_SEGEMENTS/2.0)) * i, centerPoint.x, centerPoint.y));
    }
    
    //calculate radius and spacing info
    circleRadius = segmentLines[0]->getLength();
    padSpacing = circleRadius/(numRows+1);
    
    
    //set spacing for each row
    for (int i = 0; i < numRows; i++)
    {
        rowRadii.set(i, circleRadius*rowRadiusModifiers[i]);
    }

    
    
    //position first row of pads
    positionPad(pads[47], 3, 0);
    positionPad(pads[46], 5, 0);
    positionPad(pads[45], 7, 0);
    positionPad(pads[44], 9, 0);
    positionPad(pads[43], 11, 0);
    positionPad(pads[42], 13, 0);
    positionPad(pads[41], 15, 0);
    positionPad(pads[40], 1, 0);
    
    //position second row of pads
    positionPad(pads[39], 0, 1);
    positionPad(pads[38], 2, 1);
    positionPad(pads[37], 4, 1);
    positionPad(pads[36], 6, 1);
    positionPad(pads[35], 8, 1);
    positionPad(pads[34], 10, 1);
    positionPad(pads[33], 12, 1);
    positionPad(pads[32], 14, 1);

    //position third row of pads
    positionPad(pads[31], 13, 2);
    positionPad(pads[30], 15, 2);
    positionPad(pads[29], 1, 2);
    positionPad(pads[28], 3, 2);
    positionPad(pads[27], 5, 2);
    positionPad(pads[26], 7, 2);
    positionPad(pads[25], 9, 2);
    positionPad(pads[24], 11, 2);
    
    //position fourth row of pads
    positionPad(pads[23], 10, 3);
    positionPad(pads[22], 12, 3);
    positionPad(pads[21], 14, 3);
    positionPad(pads[20], 0, 3);
    positionPad(pads[19], 2, 3);
    positionPad(pads[18], 4, 3);
    positionPad(pads[17], 6, 3);
    positionPad(pads[16], 8, 3);
    
    //position fifth row of pads
    positionPad(pads[15], 7, 4);
    positionPad(pads[14], 9, 4);
    positionPad(pads[13], 11, 4);
    positionPad(pads[12], 13, 4);
    positionPad(pads[11], 15, 4);
    positionPad(pads[10], 1, 4);
    positionPad(pads[9], 3, 4);
    positionPad(pads[8], 5, 4);
    
    //position sixth row of pads
    positionPad(pads[7], 4, 5);
    positionPad(pads[6], 6, 5);
    positionPad(pads[5], 8, 5);
    positionPad(pads[4], 10, 5);
    positionPad(pads[3], 12, 5);
    positionPad(pads[2], 14, 5);
    positionPad(pads[1], 0, 5);
    positionPad(pads[0], 2, 5);
    
    
    
    //temp slider positioning
    //ratioSliders[0]->setBounds(getWidth() - getWidth()/3.0, 0, getWidth()/3.0, 30);
    for (int i = 1; i < ratioSliders.size(); i++)
    {
        //ratioSliders[i]->setBounds(ratioSliders[i-1]->getBounds().translated(0, 32));
    }
    
    colourSelector.setBounds(getWidth()-getWidth()/3.0, 0, getWidth()/3.0, getHeight()/3.0);

}

void SphereView::positionPad(Pad* padToPosition, const int segment, const int row)
{

    padToPosition->setSize((circleRadius/numRows+1) * padSizeModifiers[row], (circleRadius/numRows+1) * padSizeModifiers[row]);
    
    static Point<float> padCenterPoint;
    padCenterPoint = segmentLines[segment]->getPointAlongLine(rowRadii[row]);
    padToPosition->setCentrePosition(padCenterPoint.x, padCenterPoint.y);
    

}

void SphereView::setAllPadColours(const Colour newColour)
{
    for (int i = 0; i < pads.size(); i++)
    {
        pads[i]->setColour(newColour);
    }
}

void SphereView::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &colourSelector)
    {
        setAllPadColours(colourSelector.getCurrentColour());
    }
}

void SphereView::clearSelectedPads()
{
    for (int i = 0; i < selectedPads.size(); i++)
    {
        selectedPads[i]->setSelected(false);
    }
    selectedPads.clear();
}


//Mouse Listener============================
void SphereView::mouseEnter (const MouseEvent &event)
{
    
}

void SphereView::mouseExit (const MouseEvent &event)
{
    
}

void SphereView::mouseDown (const MouseEvent &event)
{
    if (event.eventComponent != this)
    {
        Pad* sourcePad = nullptr;
        for (int i = 0; i < pads.size(); i++)
        {
            if (event.eventComponent == pads[i])
            {
                sourcePad = pads[i];
                break;
            }
        }
        
        if (sourcePad != nullptr)
        {
            if (event.mods.isShiftDown())
            {
                sourcePad->setSelected(!sourcePad->getSelected());
                if (sourcePad->getSelected())
                {
                    selectedPads.add(sourcePad);
                }
                else
                {
                    for (int i = 0; i < selectedPads.size(); i++)
                    {
                        if (selectedPads[i] == sourcePad)
                        {
                            selectedPads.remove(i);
                        }
                    }
                }
            }
            else if (!event.mods.isAnyModifierKeyDown())
            {
                sourcePad->setSelected(!sourcePad->getSelected());
                
                clearSelectedPads();
                
                if (sourcePad->getSelected())
                {
                    selectedPads.add(sourcePad);
                }

            }
        }
        
       
    }
    else
    {
        DBG("Click in Sphere");
        //clearSelectedPads();
    }
    
}

void SphereView::mouseDrag (const MouseEvent &event)
{
//    //DBG("MouseDrag");
//    if (event.mods.isShiftDown())
//    {
//        Component* draggedOverComponent = getComponentAt(event.getPosition().getX(), event.getPosition().getY());
//
//        for (int i = 0; i < pads.size(); i++)
//        {
//            
//            if ( contains(event.getEventRelativeTo(mainComponent).getPosition()))
//            {
//                pads[i]->setSelected(!pads[i]->getSelected());
//                if (pads[i]->getSelected())
//                {
//                    selectedPads.add(pads[i]);
//                }
//                else
//                {
//                    for (int i = 0; i < selectedPads.size(); i++)
//                    {
//                        if (selectedPads[i] == pads[i])
//                        {
//                            selectedPads.remove(i);
//                        }
//                    }
//                }
//            }
//        }
//       
//    }
}

void SphereView::mouseUp (const MouseEvent &event)
{
    
    
}

void SphereView::mouseDoubleClick (const MouseEvent &event)
{
    
}


