//
//  AlphaSphereConnection.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 29/02/2016.
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


#include "AlphaSphereConnection.hpp"
#include "AppData.hpp"
#include "Alphalive2Engine.hpp"
#include "ScaleValue.h"

#if JUCE_LINUX
#include <unistd.h>
#endif // JUCE_LINUX



//static MidiSequencerEngine* engine = AppData::Instance()->getEnginePointer();

AlphaSphereConnection::AlphaSphereConnection()
{
    engine = AppData::Instance()->getEnginePointer();

    #if JUCE_WINDOWS || JUCE_LINUX
    //start HID thread.
    //For OS X, see bottom of this function.
    startThread();

    //sleep to give the HID thread time to attempt to connect to the device.
    //I may be able to reduce the sleep time to 500ms.
    #ifdef WIN32
    sleep(1000);
    #else
    usleep(1000*1000);
    #endif

    #endif

    velocityMinRange = 0;
    velocityMaxRange = 127;


    #if JUCE_MAC
    //Start HID thread here, and then remove the MIDI out stuff if the device exists.
    //For some reason if started any earlier in this function or within the HidComms constructor
    //it causes weird lagging issues with the MIDI stuff on OS X 10.8 if the AlphaSphere is connected
    //at launch. However this is just a quick fix - it is convoluted to create the MIDI out above
    //and then probably removing it here, so this issue needs to be resolved properly asap!!
    startThread();

    //sleep to give the HID thread time to attempt to connect to the device.
    //I may be able to reduce the sleep time to 500ms.
    usleep(1000*1000);

    if (getDeviceStatus() != 0)
        removeMidiInAndOut();
    #endif

    //setLedColour(0, Colours::black);
    
//    setLedColour(0, Colours::orangered);
//    setLedColour(1, Colours::blue);
//    setLedColour(2, Colours::green);
    
    for (int i = 0; i < 48; i++)
    {
        padVelocity[i] = 0;
        padPressure[i] = 0;
    }

}

AlphaSphereConnection::~AlphaSphereConnection()
{
    stopThread(1000);

}

void AlphaSphereConnection::setAppHasInitialised(const bool initialised)
{
    appHasInitialised = initialised;
    HidComms::setAppHasInitialised();
}

void AlphaSphereConnection::hidInputCallback (int pad, int value, int velocity)
{
    if (appHasInitialised)
    {
        recievedPad = pad;
        recievedValue = float(value);
        recievedVelocity = float(velocity);
        
        //DBG(String(pad) + ":" + String(value) + ":" + String(velocity));
        
        if (pad < 48)
        {
            if (padVelocity[pad] != velocity)
            {
                //insert pad velocity curve mapping here
                
                //            //exponential mapping of velocity
                //            recievedVelocity = exp((float)recievedVelocity/MAX_VELOCITY)-1;
                //            recievedVelocity = recievedVelocity * (MAX_VELOCITY/1.71828);
                //            if (recievedVelocity > MAX_VELOCITY)
                //                recievedVelocity = MAX_VELOCITY;
                //            if (recievedVelocity > 0 && recievedVelocity < 1) //value 1 = 0.6, which is rounded to 0
                //                recievedVelocity = 1;
                
                engine->hitPad(recievedPad, recievedVelocity);
                padVelocity[pad] = velocity;
            }
            
            if (padPressure[pad] != value)
            {
                engine->pressPad(pad, value);
                padPressure[pad] = value;
            }
            
            
        }
    }
    

}

void AlphaSphereConnection::processMidiInput (const MidiMessage midiMessage)
{

        if (midiMessage.isSongPositionPointer() || midiMessage.isMidiStart() || midiMessage.isMidiContinue() || midiMessage.isMidiStop() || midiMessage.isMidiClock())
        {
            static MasterClock* masterClock = engine->getMasterClockPointer();
            masterClock->handleExternalMidiClock(midiMessage);
            
            //        if (midiMessage.isMidiClock())
            //        {
            //            DBG("EXT Clock Tick");
            //        }
            //
            
        }
        else if (midiMessage.isQuarterFrame())
        {
            DBG(midiMessage.getQuarterFrameValue());
        }
        else if (midiMessage.isTempoMetaEvent())
        {
            //static int tempo;
            midiMessage.getTempoSecondsPerQuarterNote();
            DBG(midiMessage.getTempoSecondsPerQuarterNote());
        }
        else
        {
            DBG("ASC:" + String(*midiMessage.getRawData()));
        }
    
  
    
}

void AlphaSphereConnection::sendMidiMessage(MidiMessage midiMessage)
{
    //sharedMemoryMidi.enter();
    
    if (getDeviceStatus() != 0)
    {
        //===============================================================
        //Sending MIDI over HID
        
        unsigned char dataToSend[4];
        
        const uint8 *rawMidiMessage = midiMessage.getRawData();
        
        dataToSend[0] = 0x00; //MIDI command ID
        dataToSend[1] = rawMidiMessage[0]; //midi status byte
        dataToSend[2] = rawMidiMessage[1]; //midi data byte 1
        dataToSend[3] = rawMidiMessage[2]; //midi data byte 2
        
        addMessageToHidOutReport (dataToSend);
    }
    
    else
    {
        //===============================================================
        //Sending MIDI using MidiOutput object
        
#if JUCE_MAC || JUCE_LINUX
//        if(midiOutputDevice)
//        {
//            midiOutputDevice->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
//        }
//        else
//        {
            if (!hasDisplayedNoMidiDeviceWarning)
            {
                String instructionString = translate("AlphaLive cannot currently send any MIDI messages as the AlphaSphere has been disconnected. To start sending MIDI messages again please reconnect the AlphaSphere, or if you would like to use AlphaLive's virtual MIDI port, quit and relaunch AlphaLive without the AlphaSphere connected.");
                AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                  translate("No MIDI device available!"),
                                                  translate(instructionString));
                hasDisplayedNoMidiDeviceWarning = true;

            }
            
 //       }
        
#elif JUCE_WINDOWS
        
        jassertfalse;
//        //If midi output exists (it won't if the user hasn't chosen an output device...)
//        if (audioDeviceManager.getDefaultMidiOutput())
//        {
//            audioDeviceManager.getDefaultMidiOutput()->startBackgroundThread();
//            audioDeviceManager.getDefaultMidiOutput()->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
//        }
//        else
//        {
//            if (!hasDisplayedNoMidiDeviceWarning)
//            {
//                String instructionString = translate("AlphaLive cannot currently send any MIDI messages as the AlphaSphere has been disconnected or no MIDI output port has be selected. To start sending MIDI messages please reconnect the AlphaSphere, or select an external MIDI output device from the Preferences view (if the option to select a MIDI output port is not available here, quit and relaunch AlphaLive without the AlphaSphere connected).");
//                AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
//                                                  translate("No MIDI device available!"),
//                                                  translate(instructionString));
//            }
//            
//            hasDisplayedNoMidiDeviceWarning = true;
//        }
        
#endif
        
    }
    
}

void AlphaSphereConnection::setDeviceType (int type) //1 - AlphaSphere, 2 - AlphaSphere elite
{

}
void AlphaSphereConnection::removeMidiInAndOut()
{

}

void AlphaSphereConnection::updateFirmware()
{

}

void AlphaSphereConnection::uploadFirmware (bool applyingUpdate)
{

}

void AlphaSphereConnection::setFirmwareUpdateStatus (bool status)
{

}

void AlphaSphereConnection::setDeviceStatus()
{
    AppData::Instance()->refreshHIDDeviceConnected();
    if (hasDisplayedNoMidiDeviceWarning && getDeviceStatus())
    {
        hasDisplayedNoMidiDeviceWarning = false;
    }
}

void AlphaSphereConnection::setFirmwareDetails (String version, String serial)
{

}

void AlphaSphereConnection::setLedSettings (uint8 setting, uint8 value)
{
    //This is the declaration of the abstract HidComms function.
    //It is used to set the general settings of the LED of the hardware.
    //The 'setting' argument can have the following values:
    // 1 - LED on/off status - set 'value' to 0 or 1
    // 2 - LED pressure interaction status - set 'value' to 0 or 1
    // 3 - LED clock interaction -  set 'value' to 0 for 'off',
    //                              1 for 'fade to black', or
    //                              2 for 'fade from max colour to min colour' (not yet implemented).
    // 4 - LED mode - set 'value' to 0 for normal or 1 for MIDI CC controlled mode

    if (getDeviceStatus() != 0)
    {
        unsigned char dataToSend[4];
        dataToSend[0] = 0x01; //General LED settings command
        dataToSend[1] = setting;
        dataToSend[2] = value;
        dataToSend[3] = 0x00;
        addMessageToHidOutReport(dataToSend);
    }
}

void AlphaSphereConnection::setLedColour (uint8 colourNumber, Colour colour)
{
    /**
        Used to set the colours of the RGB LEDs, converts a juce colour into Red Green and Blue values,
        and sets these via HID.
     */
    if (getDeviceStatus() != 0)
    {
        unsigned char dataToSend[4];
        dataToSend[0] = colourNumber + 2; //LED colour command
        dataToSend[1] = colour.getRed();
        dataToSend[2] = colour.getGreen();
        dataToSend[3] = colour.getBlue();
        addMessageToHidOutReport(dataToSend);
    }
}

