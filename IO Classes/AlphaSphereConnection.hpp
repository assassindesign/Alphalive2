//
//  AlphaSphereConnection.hpp
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

#ifndef AlphaSphereConnection_hpp
#define AlphaSphereConnection_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "HidComms.h"


//#include "AlphaLearnEngine.hpp"

/** 
    Class for handling the sphere connection. In Alphalive, the main engine class inherits from HidComms and manages the sphere connection, but in the interests of modularity, this has been moved to a separate entity. This could potentially allow for multiple spheres to be connected and should promote easier maintenence in future.
 */

struct MidiChannelPressureHolder
{
    int aftertouch;
    int pitchBend;
    int controlChange[128];
};

class Alphalive2Engine;

class AlphaSphereConnection : public HidComms
{
public:
//    class Listener
//    {
//    public:
//        Listener();
//        virtual ~Listener();
//        
//        virtual void hitPad (const int pad, const int velocity) = 0;
//        virtual void pressPad (const int pad, const int pressure) = 0;
//    };
    
    AlphaSphereConnection();
    ~AlphaSphereConnection();
    
    void setAppHasInitialised(const bool initialised);
    
    // Virtual functions from HIDComms
    void hidInputCallback (int pad, int value, int velocity) override;
    void processMidiInput (const MidiMessage midiMessage) override;
    void sendMidiMessage(MidiMessage midiMessage);
    void setDeviceType (int type) override; //1 - AlphaSphere, 2 - AlphaSphere elite
    void removeMidiInAndOut() override;
    void updateFirmware() override;
    void uploadFirmware (bool applyingUpdate = true);
    void setFirmwareUpdateStatus (bool status) override;
    void setDeviceStatus() override;
    void setFirmwareDetails (String version, String serial) override;
    void setLedSettings (uint8 setting, uint8 value) override;
    void setLedColour (uint8 colourNumber, Colour colour) override;

private:
    
    int recievedPad;
    float recievedValue,recievedVelocity;
    
    int padVelocity[48];
    int padPressure[48];
    
    int pressureCurve;
    int velocityCurve;
    int velocityMinRange;
    int velocityMaxRange;
    
    Alphalive2Engine* engine;
    
    bool appHasInitialised = false;
    bool hasDisplayedNoMidiDeviceWarning = true;
    
};
#endif /* AlphaSphereConnection_hpp */
