//
//  PadInspectorPanels.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 02/08/2016.
//
//

#ifndef PadInspectorPanels_hpp
#define PadInspectorPanels_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "GUIColours.h"
#include "AppData.hpp"
#include "MidiIPanel.hpp"
#include "AudioIPanel.hpp"
#include "PadInspectorPanelBase.h"


class InspectorTopPanel : public PadInspectorPanelBase,
                          public Button::Listener
{
public:
   
    
    InspectorTopPanel()
    {
        audioButton.setButtonText("Audio");
        audioButton.addListener(this);
        addAndMakeVisible(&audioButton);
        
        midiButton.setButtonText("MIDI");
        midiButton.addListener(this);
        addAndMakeVisible(&midiButton);
        
        systemButton.setButtonText("System");
        systemButton.addListener(this);
        addChildComponent(&systemButton);
        
        if (AppData::Instance()->getAdvancedFeaturesEnabled())
        {
            systemButton.setVisible(true);
        }
        
        AppData::Instance()->addListener(this);
        
        //AppData::Instance()->setAdvancedFeaturesEnabled(true);
        
    }
    ~InspectorTopPanel(){}
    
    void resized() override
    {
        
        if (AppData::Instance()->getAdvancedFeaturesEnabled())
            buttonWidth = getWidth()/3.0;
        else
            buttonWidth = getWidth()/2.0;
        
        audioButton.setBounds(0, 0, buttonWidth, 25);
        midiButton.setBounds(audioButton.getBounds().translated(buttonWidth, 0));
        
        if (AppData::Instance()->getAdvancedFeaturesEnabled())
            systemButton.setBounds(midiButton.getBounds().translated(buttonWidth, 0));

    }
    
    void paint (Graphics& g) override
    {
        g.fillAll(Colour(GUIColours::Background));
    }
    
    void appDataChangeCallback(const int changedData) override
    {
        if (changedData == AppData::DataIDs::AdvancedEnabled)
        {
            systemButton.setVisible(AppData::Instance()->getAdvancedFeaturesEnabled());
            
        }
       
    }
    
    void padDataChangeCallback(const int changedData) override
    {
        if (changedData == PadData::DataIDs::PadFunction)
        {
            switch (AppData::Instance()->getCurrentlyInspectingPadPtr()->getPadFunction()) {
                case PadData::PadFunction::Audio:
                    audioPanel.setVisible(true);
                    midiPanel.setVisible(false);
                    break;
                    
                default:
                    break;
            }
        }
    }

    
    void buttonClicked (Button* button) override
    {
        if (button == &audioButton)
        {
            //set pad mode to audio
            AppData::Instance()->getCurrentlyInspectingPadPtr()->setPadFunction(PadData::PadFunction::Audio);
        }
        else if (button == &midiButton)
        {
            //set pad mode to midi
            AppData::Instance()->getCurrentlyInspectingPadPtr()->setPadFunction(PadData::PadFunction::Midi);
        }
        if (button == &systemButton)
        {
            //set pad mode to system
            AppData::Instance()->getCurrentlyInspectingPadPtr()->setPadFunction(PadData::PadFunction::System);
        }
    }
    
    void refreshData() override
    {
        audioPanel.setDataObject(getDataObject());
        midiPanel.setDataObject(getDataObject());
    }
    
    
private:
    TextButton audioButton, midiButton, systemButton;
    float buttonWidth;
    AudioIPanel audioPanel;
    MidiIPanel midiPanel;
};

class InspectorBottomPanel : public PadInspectorPanelBase
{
public:
    InspectorBottomPanel(){}
    ~InspectorBottomPanel(){}
    
    void resized() override
    {
        
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll(Colour(GUIColours::Background).brighter());
    }
    
    void refreshData() override
    {
        
    }
private:
    
    
};


class MainInspectorPanel : public PadInspectorPanelBase
{
public:
    MainInspectorPanel()
    {
        addAndMakeVisible(&top);
        addAndMakeVisible(&bottom);
    }
    ~MainInspectorPanel(){}
    
    void resized() override
    {
        top.setBounds(0, 0, getWidth(), getHeight()*0.618);
        bottom.setBounds(0, top.getBottom(), getWidth(), getHeight()-top.getHeight());
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll(Colour(GUIColours::Background));
    }
    
    void refreshData() override
    {
        top.setDataObject(getDataObject());
        bottom.setDataObject(getDataObject());
    }
private:
    InspectorTopPanel top;
    InspectorBottomPanel bottom;
    
};

#endif /* PadInspectorPanels_hpp */

