//
//  MidiPressureSettingsPanel.cpp
//  Alphalive 2
//
//  Created by Luke Saxton on 20/09/2016.
//
//

#include "MidiPressureSettingsPanel.hpp"


MidiPressureSettingsPanel::MidiPressureSettingsPanel()
{
    polyATButton.setButtonText("PolyAT");
    polyATButton.addListener(this);
    polyATButton.setConnectedEdges(3);
    polyATButton.setTooltip(translate("Polyphonic Aftertouch"));
    addAndMakeVisible(polyATButton);
    
    channelATButton.setButtonText("ChnlAT");
    channelATButton.addListener(this);
    channelATButton.setConnectedEdges(3);
    channelATButton.setTooltip(translate("Channel Aftertouch"));
    addAndMakeVisible(channelATButton);
    
    pbUpButton.setButtonText("PB Up");
    pbUpButton.addListener(this);
    pbUpButton.setConnectedEdges(3);
    pbUpButton.setTooltip(translate("Pitch Bend Up"));
    addAndMakeVisible(pbUpButton);
    
    pbDownButton.setButtonText("PB Dn");
    pbDownButton.addListener(this);
    pbDownButton.setConnectedEdges(3);
    pbDownButton.setTooltip(translate("Pitch Bend Down"));
    addAndMakeVisible(pbDownButton);
    
    modWheelButton.setButtonText("Mod");
    modWheelButton.addListener(this);
    modWheelButton.setConnectedEdges(3);
    modWheelButton.setTooltip(translate("Modulation Wheel"));
    addAndMakeVisible(modWheelButton);
    
    midiCCButton.setButtonText("CC");
    midiCCButton.addListener(this);
    midiCCButton.setConnectedEdges(3);
    midiCCButton.setTooltip(translate("CC"));
    addAndMakeVisible(midiCCButton);
}

MidiPressureSettingsPanel::~MidiPressureSettingsPanel()
{
    
}

void MidiPressureSettingsPanel::refreshData()
{
    padData = AppData::Instance()->getCurrentlyInspectingPadDataPtr();
    padDataChangeCallback(PadData::DataIDs::PressureDestination);
}

void MidiPressureSettingsPanel::padDataChangeCallback(const int changedData)
{
    if (padData != nullptr)
    {
        if (changedData == PadData::DataIDs::PressureDestination)
        {
            polyATButton.setToggleState(false, dontSendNotification);
            channelATButton.setToggleState(false, dontSendNotification);
            pbUpButton.setToggleState(false, dontSendNotification);
            pbDownButton.setToggleState(false, dontSendNotification);
            modWheelButton.setToggleState(false, dontSendNotification);
            midiCCButton.setToggleState(false, dontSendNotification);

            
            
            switch (padData->getPressureDestination()){
                case PadData::PressureDestinations::PolyAT:
                    polyATButton.setToggleState(true, dontSendNotification);
                    break;
                case PadData::PressureDestinations::ChannetAT:
                    channelATButton.setToggleState(true, dontSendNotification);
                    break;
                case PadData::PressureDestinations::PBup:
                    pbUpButton.setToggleState(true, dontSendNotification);
                    break;
                case PadData::PressureDestinations::PBDown:
                    pbDownButton.setToggleState(true, dontSendNotification);
                    break;
                case PadData::PressureDestinations::ModWheel:
                    modWheelButton.setToggleState(true, dontSendNotification);
                    break;
                case PadData::PressureDestinations::MidiCC:
                    midiCCButton.setToggleState(true, dontSendNotification);
                    break;
                default:
                    break;
            }
        }
    }
    else
        jassertfalse;
   
}


void MidiPressureSettingsPanel::paint(Graphics &g)
{
    g.fillAll(GUIColours::Background);
    g.setColour(GUIColours::PanelBackground);
    g.fillRect(destinationsBox);
    g.fillRect(rangeBox);
    g.fillRect(settingsBox);
    
    g.setColour(GUIColours::AlphaGreen);
    g.drawRect(destinationsBox, 1);
    g.drawRect(rangeBox, 1);
    g.drawRect(settingsBox, 1);
    
    g.setColour(Colours::white);
    g.setFont(GUIFonts::Roboto);
    g.drawSingleLineText("DESTINATION", destinationsBox.getX()+5, destinationsBox.getY()+15);
    g.drawSingleLineText("RANGE", rangeBox.getX()+5, rangeBox.getY()+15);
    g.drawSingleLineText("SETTINGS", settingsBox.getX()+5, settingsBox.getY()+15);
    
}

void MidiPressureSettingsPanel::resized()
{
    float x = getWidth();
    
    destinationsBox.setBounds(0, 0, getWidth(), 65);
    rangeBox = settingsBox = destinationsBox;
    rangeBox.translate(0, 66);
    settingsBox.translate(0, 132);
    
    polyATButton.setBounds(x/12.0, 20, x/8.0, 40);
    channelATButton.setBounds(polyATButton.getBounds().translated(polyATButton.getWidth()*1.166, 0));
    pbUpButton.setBounds(channelATButton.getBounds().translated(polyATButton.getWidth()*1.166, 0));
    pbDownButton.setBounds(pbUpButton.getBounds().translated(polyATButton.getWidth()*1.166, 0));
    modWheelButton.setBounds(pbDownButton.getBounds().translated(polyATButton.getWidth()*1.166, 0));
    midiCCButton.setBounds(modWheelButton.getBounds().translated(polyATButton.getWidth()*1.166, 0));


}

void MidiPressureSettingsPanel::buttonClicked(Button* button)
{
    if (padData != nullptr)
    {
        if (button == &polyATButton)
        {
            padData->setPressureDestination(PadData::PressureDestinations::PolyAT);
        }
        else if (button == &channelATButton)
        {
            padData->setPressureDestination(PadData::PressureDestinations::ChannetAT);
        }
        else if (button == &pbUpButton)
        {
            padData->setPressureDestination(PadData::PressureDestinations::PBup);
        }
        else if (button == &pbDownButton)
        {
            padData->setPressureDestination(PadData::PressureDestinations::PBDown);
        }
        else if (button == &modWheelButton)
        {
            padData->setPressureDestination(PadData::PressureDestinations::ModWheel);
        }
        else if (button == &midiCCButton)
        {
            padData->setPressureDestination(PadData::PressureDestinations::MidiCC);
        }
    }

}

