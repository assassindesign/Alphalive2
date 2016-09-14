//
//  AlphaSphereConnection.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 29/02/2016.
//
//



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

    setAppHasInitialised();
    //setLedColour(0, Colours::black);
    
    setLedColour(0, Colours::orangered);
    setLedColour(1, Colours::blue);
    setLedColour(2, Colours::green);
    
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

void AlphaSphereConnection::hidInputCallback (int pad, int value, int velocity)
{


    recievedPad = pad;
    recievedValue = float(value);
    recievedVelocity = float(velocity);


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

void AlphaSphereConnection::processMidiInput (const MidiMessage midiMessage)
{

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

