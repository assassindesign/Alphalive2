//
//  MidiIPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
//
//

#ifndef MidiIPanel_hpp
#define MidiIPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"



class MidiIPanel : public PadInspectorPanelBase,
                   public Button::Listener
{
public:
    
    class SingleNotePanel : public PadInspectorPanelBase
    {
    public:
        SingleNotePanel()
        {
            
        }
        ~SingleNotePanel(){}
        void resized() override{}
        void paint(Graphics& g) override
        {
            g.fillAll(Colours::blue);
        }
        
        void refreshData() override
        {
            
        }
        
        void padDataChangeCallback(const int changedData) override
        {
            
        }
        


    private:
    };
    
    class MultiNotePanel : public PadInspectorPanelBase,
                           public Button::Listener
    {
    public:
        MultiNotePanel()
        {
            multiNoteModeBtn.addListener(this);
            addAndMakeVisible(multiNoteModeBtn);
            refreshModeButton();
        }
        ~MultiNotePanel(){}
        void resized() override
        {
            multiNoteModeBtn.setBounds(getBounds());
        }
        void paint(Graphics& g) override
        {
            g.fillAll(Colours::blue);
        }
        
        void refreshData() override
        {
            refreshModeButton();

        }
        
        void refreshModeButton()
        {
            switch (AppData::Instance()->getCurrentlyInspectingPadPtr()->getMultiNoteMode())
            {
                case PadData::MultiNoteModes::Chord:
                    multiNoteModeBtn.setButtonText("Chord");
                    break;
                case PadData::MultiNoteModes::Arpeggiate:
                    multiNoteModeBtn.setButtonText("Arp");
                    break;
                case PadData::MultiNoteModes::StepThrough:
                    multiNoteModeBtn.setButtonText("Step");
                    break;
                default:
                    break;
            }
        }
        
        void padDataChangeCallback(const int changedData) override
        {
            if (changedData == PadData::DataIDs::MultiNoteMode)
            {
                refreshModeButton();
            }
        }
        
        void buttonClicked (Button* button) override
        {
            PadData* padData = AppData::Instance()->getCurrentlyInspectingPadPtr();
            if (button == &multiNoteModeBtn)
            {
                if (!padData->setMultiNoteMode(padData->getMultiNoteMode() +1))
                {
                    padData->setMultiNoteMode(0);
                }
            }
        }
        

    private:
        TextButton multiNoteModeBtn;
    };
    
    MidiIPanel()
    {
        singleNoteBtn.setButtonText("Single");
        singleNoteBtn.addListener(this);
        addAndMakeVisible(singleNoteBtn);
        
        multiNoteBtn.setButtonText("Multi");
        multiNoteBtn.addListener(this);
        addAndMakeVisible(multiNoteBtn);
        
        standardTriggerBtn.setButtonText("Standard");
        standardTriggerBtn.addListener(this);
        addAndMakeVisible(standardTriggerBtn);
        
        toggleTriggerBtn.setButtonText("Toggle");
        toggleTriggerBtn.addListener(this);
        addAndMakeVisible(toggleTriggerBtn);
        
    }
    ~MidiIPanel(){}
    void resized() override
    {
        float halfWidth = getWidth()/2.0;
        
        singleNoteBtn.setBounds(0, 0, halfWidth, 20);
        multiNoteBtn.setBounds(singleNoteBtn.getBounds().translated(halfWidth, 0));
        
        standardTriggerBtn.setBounds(singleNoteBtn.getBounds().translated(0, singleNoteBtn.getHeight()));
        toggleTriggerBtn.setBounds(multiNoteBtn.getBounds().translated(0, multiNoteBtn.getHeight()));

    }
    void paint(Graphics& g) override
    {
        //g.fillAll(Colours::blue);
    }
    
    void refreshData() override
    {
        
        multiNotePanel.setDataObject(getDataObject());
        padDataChangeCallback(PadData::DataIDs::PadMidiFunction);
        padDataChangeCallback(PadData::DataIDs::NoteTriggerMode);

    }
    
    
    void padDataChangeCallback(const int changedData) override
    {
        padData = AppData::Instance()->getCurrentlyInspectingPadPtr();
        if (changedData == PadData::DataIDs::PadMidiFunction)
        {
            if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::SingleNote)
            {
                singleNoteBtn.setToggleState(true, dontSendNotification);
                multiNoteBtn.setToggleState(false, dontSendNotification);
            }
            else if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::MultiNote)
            {
                singleNoteBtn.setToggleState(false, dontSendNotification);
                multiNoteBtn.setToggleState(true, dontSendNotification);
            }
        }
        else if (changedData == PadData::DataIDs::NoteTriggerMode)
        {
            if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::StandardNoteMode)
            {
                standardTriggerBtn.setToggleState(true, dontSendNotification);
                toggleTriggerBtn.setToggleState(false, dontSendNotification);
            }
            else if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::ToggleNoteMode)
            {
                standardTriggerBtn.setToggleState(false, dontSendNotification);
                toggleTriggerBtn.setToggleState(true, dontSendNotification);
            }
        }
    }
    
    void buttonClicked (Button* button) override
    {
        padData = AppData::Instance()->getCurrentlyInspectingPadPtr();
        if (button == &singleNoteBtn)
        {
            padData->setPadMidiFunction(PadData::PadMidiFunctions::SingleNote);
        }
        else if (button == &multiNoteBtn)
        {
            padData->setPadMidiFunction(PadData::PadMidiFunctions::MultiNote);
        }
        else if (button == &standardTriggerBtn)
        {
            padData->setNoteTriggerMode(PadData::NoteTriggerModes::StandardNoteMode);
        }
        else if (button == &toggleTriggerBtn)
        {
            padData->setNoteTriggerMode(PadData::NoteTriggerModes::ToggleNoteMode);
        }
    }
    
  
    
private:
    
    TextButton singleNoteBtn, multiNoteBtn, standardTriggerBtn, toggleTriggerBtn;
    MultiNotePanel multiNotePanel;
    PadData* padData;
    bool panelEnabled;
};

#endif /* MidiIPanel_hpp */
