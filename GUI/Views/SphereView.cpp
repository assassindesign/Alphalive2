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
    //setBufferedToImage(true);
    
    sphereData = AppData::Instance()->getSphereData(sphereID);
    
    for (int i = 0 ; i < NUM_SEGEMENTS; i++)
    {
        segmentLines.add(new Line<float>());
    }
    
    for (int i = 0 ; i < sphereData->getNumPadDataObjects(); i++)
    {
        pads.add(new Pad(sphereData->getPadData(i)));
        pads.getLast()->setSize(50,50);
        
        addAndMakeVisible(pads.getLast());
        
        sphereData->getPadData(i)->addListener(pads.getLast());
        
        pads.getLast()->addMouseListener(this, true);
    }
    
    numRows = 6;
    mouseCurrentlyOverCircle = -1;
    allPadsSelected = false;
    
    for (int i = 0 ; i < numRows; i++)
    {
        rowRadii.add(0.0);
        backgroundCircleBoxes.add(*new Rectangle<float>());
        padRowSelected.add(false);
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
    //addAndMakeVisible(colourSelector);
    //colourSelector.addToDesktop(ComponentPeer::StyleFlags::windowHasCloseButton || ComponentPeer::StyleFlags::windowHasTitleBar);

    
    openGLContext.setMultisamplingEnabled(true);
    //openGLContext.attachTo(*this)
        
    //setBufferedToImage(true);

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
    //g.fillAll(GUIColours::Background);
    //draw outer circle
    g.setColour(Colours::white.withAlpha(uint8(0x33)));
    
    g.drawEllipse(segmentLines[12]->getEnd().x+3, segmentLines[0]->getEnd().y+3, (segmentLines[0]->getLength()*2.0)-6, (segmentLines[0]->getLength()*2.0)-6, 3);
    
    //draw sphere background
    static Colour backgroundColour;
    
    backgroundColour = GUIColours::Background;
    
    backgroundColour = backgroundColour.withBrightness(backgroundColour.getBrightness()+0.03);
    g.setColour(backgroundColour);
    g.fillEllipse(backgroundCircleBoxes[0]);
    
    //draw inner stacking circles
    for (int i = numRows; i >= 1; i--)
    {
        backgroundColour = backgroundColour.withBrightness(backgroundColour.getBrightness()+0.02);
        g.setColour(backgroundColour);
        g.fillEllipse(backgroundCircleBoxes[i]);
    }
    
    //draw positioning lines
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
        backgroundCircleBoxes.getReference(i).setBounds(segmentLines[12]->getPointAlongLine(rowRadii[i]).getX(), segmentLines[0]->getPointAlongLine(rowRadii[i]).getY(), rowRadii[i]*2, rowRadii[i]*2);
    }
    
    //set background circle bounds
    backgroundCircleBoxes.getReference(0).setBounds(segmentLines[12]->getEnd().x+3, segmentLines[0]->getEnd().y+3, (segmentLines[0]->getLength()*2.0)-6, (segmentLines[0]->getLength()*2.0)-6);
    for (int i = 1; i < numRows; i++)
    {
        backgroundCircleBoxes.getReference(i).setBounds(segmentLines[12]->getPointAlongLine(rowRadii[i]).getX(), segmentLines[0]->getPointAlongLine(rowRadii[i]).getY(), rowRadii[i]*2, rowRadii[i]*2);
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
    selectedPads.clearQuick();
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
        bool clickOnPad = false;
        
        Pad* sourcePad = nullptr;
        for (int i = 0; i < pads.size(); i++) //search pad array to fetch padUI object for clicked pad
        {
            if (event.eventComponent == pads[i])
            {
                sourcePad = pads[i];
                break;
            }
        }
        
        
        if (sourcePad != nullptr) //if the source component was a pad that exists
        {
            if (sourcePad->isPointInsideCircle(event.getMouseDownPosition())) //if mouse is actually in the circle of the padUI
            {
                //DBG("Click in Pad");
                clickOnPad = true;
                
                if (event.mods.isShiftDown()) // clicking single pad with shift down - add/remove from selected
                {
                    sourcePad->setSelected(!sourcePad->getSelected()); // toggle boolean value
                    
                    if (sourcePad->getSelected()) //either add pad to selected group
                    {
                        selectedPads.add(sourcePad);
                    }
                    else // or remove it
                    {
                        for (int i = 0; i < selectedPads.size(); i++)
                        {
                            if (selectedPads[i] == sourcePad)
                            {
                                selectedPads.remove(i);
                            }
                        }
                    }
                    
                    if (selectedPads.size() != 1) //if there is more than one pad selcted at the moment, don't display inspector. This will probably change later to switch padinspector to group edit mode
                    {
                        AppData::Instance()->setCurrentlyInspectingPad(-1, -1);
                    }
                }
                else if (!event.mods.isAnyModifierKeyDown()) // clicking in single pad with no modifiers
                {
                    for (int i = 0; i < padRowSelected.size(); i++) // reset any padRowSelected flags
                    {
                        padRowSelected.set(i, false);
                    }
                    
                    sourcePad->setSelected(!sourcePad->getSelected());
                    
                    clearSelectedPads();
                    
                    if (sourcePad->getSelected()) // clicked pad is now selected
                    {
                        selectedPads.add(sourcePad);
                        sourcePad->setAsCurrentlyInspectedPad();//AppData::Instance()->setCurrentlyInspectingPad(sourcePad->, <#const int padID#>)
                    }
                    else
                    {
                        AppData::Instance()->setCurrentlyInspectingPad(-1, -1);
                    }
                    
                }
            }
            
        }
    }
    else // if click was not on a pad
    {
        if (!event.mods.isAltDown())
        {
            int circleID = isMouseInCircle(event);
            if (circleID > -1) //if a ring has been selected
            {
                //DBG("Clicked Within Circle: " + String(circleID));
                selectedPads.clearQuick();
                
                for (int i = 0; i < padRowSelected.size(); i++)
                {
                    if (i == circleID-1)
                    {
                        padRowSelected.set(i, !padRowSelected[i]);
                    }
                    else
                    {
                        padRowSelected.set(i, false);
                    }
                }
                
                if (circleID == 0) //center - select all pads
                {
                    allPadsSelected = !allPadsSelected;
                    
                    for (int i = 0; i < pads.size(); i++)
                    {
                        pads[i]->setSelected(allPadsSelected);
                        if (pads[i]->getSelected())
                        {
                            selectedPads.add(pads[i]);
                        }
                    }
                    
                    AppData::Instance()->setCurrentlyInspectingPad(-1, -1);
                    
                }
                else // select a ring of pads
                {
                    allPadsSelected = false;
                    int rangeMin = ((numRows - circleID)*8);
                    int rangeMax = rangeMin + 8;
                    for (int i = 0; i < pads.size(); i++)
                    {
                        
                        if (i >= rangeMin && i < rangeMax)
                        {
                            pads[i]->setSelected(padRowSelected[circleID-1]);
                        }
                        else
                        {
                            if (!event.mods.isShiftDown())
                            {
                                pads[i]->setSelected(false);
                            }
                        }
                        
                        
                        if (pads[i]->getSelected())
                        {
                            selectedPads.addIfNotAlreadyThere(pads[i]);
                        }
                        
                    }
                }
                
                if (selectedPads.size() == 0)
                {
                    AppData::Instance()->setCurrentlyInspectingPad(-1, -1);
                }
            }
        }
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

void SphereView::mouseMove (const MouseEvent& event)
{
    mouseCurrentlyOverCircle = isMouseInCircle(event);
    //repaint();
}


int SphereView::isMouseInCircle(const MouseEvent &event)
{
    Point<int> centerPointOnScreen = getScreenPosition();
    centerPointOnScreen.addXY(centerPoint.getX(), centerPoint.getY());
    for (int i = 0; i < rowRadii.size() + 1; i++)
    {
        int radius;
        if (i < rowRadii.size())
        {
            radius = rowRadii[i];
        }
        else
        {
            radius = getWidth()/2.0;
        }
        
        if (event.getMouseDownScreenPosition().getDistanceFrom(centerPointOnScreen) < radius)
        {
            return i;
        }
        
    }
    
    return -1;
}

