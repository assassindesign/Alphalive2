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
    public Slider::Listener
    {
    public:
        NoteSelectPanel()
        {
            noteNumberSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
            noteNumberSlider.setRange(0, 127, 1);
            //noteNumberSlider.setColour(Slider::ColourIds::, )
            addAndMakeVisible(noteNumberSlider);
            
            noteNumberSlider.addListener(this);
        }
        ~NoteSelectPanel(){}
        void resized() override
        {
            noteNumberSlider.setBounds(0,0,getWidth(),getHeight());
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
            if(slider == &noteNumberSlider)
            {
                PadData* padData = AppData::Instance()->getCurrentlyInspectingPadPtr();
                AppData::Instance()->getEnginePointer()->hitPad(padData->getPadID(), 0);
                padData->setMidiNote(noteNumberSlider.getValue());
            }
        }
        
        void padDataChangeCallback(const int changedData) override
        {
            if (changedData == PadData::DataIDs::MidiNotes)
            {
                int newNoteNumber = AppData::Instance()->getCurrentlyInspectingPadPtr()->getMidiNote();
                
                if (newNoteNumber != noteNumberSlider.getValue())
                {
                    noteNumberSlider.setValue(newNoteNumber);
                }
            }
        }
        
    private:
        Slider noteNumberSlider;
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
