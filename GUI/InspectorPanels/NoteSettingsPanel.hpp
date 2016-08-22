//
//  NoteSettingsPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 11/08/2016.
//
//

#ifndef NoteSettingsPanel_hpp
#define NoteSettingsPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"
#include "GUIColours.h"
#include "Alphalive2Engine.hpp"

class NoteSettingsPanel : public PadInspectorPanelBase,
                          public Button::Listener
{
public:
    class NoteSelectPanel : public PadInspectorPanelBase,
                            public Slider::Listener,
                            public Button::Listener
    {
    public:
        NoteSelectPanel()
        {
            addNewNoteNumberSlider();
            //addNewNoteNumberSlider();

            plusButton.setButtonText("+");
            plusButton.addListener(this);
            addAndMakeVisible(plusButton);
        }
        ~NoteSelectPanel(){}
        void resized() override
        {
            noteNumberSliders[0]->setBounds(0,0,getWidth(),25);
            
            for (int i = 1; i < noteNumberSliders.size(); i++)
            {
                noteNumberSliders[i]->setBounds(noteNumberSliders[i-1]->getBounds().translated(0, 30));
            }
            
            plusButton.setBounds(getWidth() - 25, noteNumberSliders.getLast()->getBottom() + 5, 20, 20);
        }
        void paint(Graphics& g) override
        {
            //g.fillAll(Colours::red);
        }
        
        void refreshData() override
        {
            padDataChangeCallback(PadData::DataIDs::MidiNotes);
        }
        
        void sliderValueChanged (Slider* slider) override
        {
            bool isNoteNumberSlider = false;
            for (int i = 0; i < noteNumberSliders.size(); i++)
            {
                if(slider == noteNumberSliders[i])
                {
                    isNoteNumberSlider = true;
                    break;
                }
            }
            
            if (isNoteNumberSlider)
            {
                PadData* tempPadData = AppData::Instance()->getCurrentlyInspectingPadPtr();
                
                bool wasPlaying = (tempPadData->getVelocity() > 0);
                AppData::Instance()->getEnginePointer()->getSpherePointer(tempPadData->getParentSphere()->getSphereID())->killPad(tempPadData->getPadID());
                
                tempPadData->clearAllMidiNotes();
                
                tempPadData->setMidiNote(noteNumberSliders[0]->getValue(), 100);
                
                for (int i = 1; i < noteNumberSliders.size(); i++)
                {
                    tempPadData->addMidiNote(noteNumberSliders[i]->getValue(), 60);
                }
                
                if (tempPadData->getPadFunction() == PadData::PadFunctions::Midi && wasPlaying)
                {
                    AppData::Instance()->getEnginePointer()->hitPad(tempPadData->getParentSphere()->getSphereID(), tempPadData->getVelocity());
                }
            }
            
        }
        
        void buttonClicked (Button* button) override
        {
            if (button == &plusButton)
            {
                PadData* tempPadData = AppData::Instance()->getCurrentlyInspectingPadPtr();
                tempPadData->addMidiNote(60, 50);
            }
        }
        
        void padDataChangeCallback(const int changedData) override
        {
            if (changedData == PadData::DataIDs::MidiNotes)
            {
                Array<PadData::MidiNote> newNoteNumbers = AppData::Instance()->getCurrentlyInspectingPadPtr()->getMidiNotes();
                
                for (int i = 0; i < newNoteNumbers.size(); i++)
                {
                    if (i < noteNumberSliders.size())
                    {
                        noteNumberSliders[i]->setValue(newNoteNumbers[i].noteNumber, dontSendNotification);
                    }
                    else
                    {
                        addNewNoteNumberSlider();
                        noteNumberSliders.getLast()->setValue(newNoteNumbers[i].noteNumber, dontSendNotification);
                    }
                }
            }
        }
        
        void addNewNoteNumberSlider(const int withValue = 0)
        {
            noteNumberSliders.add(new Slider());
            noteNumberSliders.getLast()->setSliderStyle(Slider::SliderStyle::IncDecButtons);
            noteNumberSliders.getLast()->setRange(0, 127, 1);
            addAndMakeVisible(noteNumberSliders.getLast());
            noteNumberSliders.getLast()->addListener(this);
            
            resized();
        }
        
    private:
        OwnedArray<Slider> noteNumberSliders;
        TextButton plusButton;
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


    enum ChildPanelIDs
    {
        Note = 0,
        Pressure
    };
    
    
    NoteSettingsPanel();
    
    ~NoteSettingsPanel();
    
    void resized() override;
    
    void paint(Graphics& g) override;
    
    void refreshData() override;
    
    void padDataChangeCallback(const int changedData) override;
    
    void setCurrentlyViewingPanel(const int childPanelID);
    
    void buttonClicked (Button*) override;
    
private:
    TextButton singleNoteBtn, multiNoteBtn, standardTriggerBtn, toggleTriggerBtn;
    PadData* padData;
    
    ScopedPointer<NoteSelectPanel> noteSelectPanel;

};



#endif /* NoteSettingsPanel_hpp */
