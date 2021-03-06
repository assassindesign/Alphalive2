//
//  PlayablePad.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
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

#include "PlayablePad.hpp"
#include "Alphalive2Engine.hpp"
#include "ScaleValue.h"

PlayablePad::PlayablePad(PadData* dataForPad)
{
    if (dataForPad != nullptr)
    {
        padData = dataForPad;
    }
    else
    {
        jassert(false); //pad data object doesn't exist. Create object before passing in pointer.
    }
    
    router = AppData::Instance()->getEnginePointer()->getMidiRouterPointer();
    if (!router)
    {
        jassertfalse; // These objects should be created ahead of pad objects;
    }
    
    masterClock = AppData::Instance()->getEnginePointer()->getMasterClockPointer();
    
    if (masterClock)
    {
        masterClock->addListener(this);
    }
    else
    {
        jassertfalse; // These objects should be created ahead of pad objects;

    }
  

}
PlayablePad::~PlayablePad()
{
    
}

void PlayablePad::hitPad(const int velocity, const bool killingPad)
{
    if (padData->getPadEnabled())
    {
        static int rawVelocity;
        if (padData->getNoteEnabled() || killingPad)
        {
            //===Note Trigger Mode===============================================================================
            
            bool ignoreHit = false;
            if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::ToggleNoteMode)
            {
                if (velocity > 0) // is a note on message
                {
                    if (rawVelocity <= 0) // pad is currently off
                    {
                        rawVelocity = velocity;
                    }
                    else if (rawVelocity > 0) // pad is currently on
                    {
                        rawVelocity = 0;
                    }
                }
                else
                {
                    ignoreHit = true;
                }
                
            }
            else if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::StandardNoteMode)
            {
                rawVelocity = velocity;
            }
            else if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::LatchNoteMode)
            {
                if (padData->getVelocity() == 0)
                {
                    rawVelocity = velocity;
                }
                else
                {
                    ignoreHit = true;
                }
            }
            else if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::TriggerNoteMode && !killingPad)
            {
                if (velocity > 0)
                {
                    hitPad(0, true);
                    rawVelocity = velocity;
                }
                else
                {
                    ignoreHit = true;
                }
            }
            
            
            if (killingPad)
            {
                rawVelocity = 0;
            }
            
            if (!ignoreHit || killingPad) // if pad should trigger
            {
                //===Velocity Curve===============================================================================
                
                static float recievedVelocity;
                switch (padData->getVelocityCurve())
                {
                    case PadData::CurveTypes::Logarithmic:
                        //logarithmic mapping of velocity
                        recievedVelocity = log(rawVelocity+1);
                        recievedVelocity = recievedVelocity * (MAX_VELOCITY/4.85); // not sure why 4.85 here!
                        if (recievedVelocity > MAX_VELOCITY)
                            recievedVelocity = MAX_VELOCITY;
                        break;
                        
                    default:
                        recievedVelocity = rawVelocity;
                        break;
                }
                
                //recievedVelocity = scaleValue (recievedVelocity, 0, 127.0, 0, 127.0);
                
                //===Pad Function================================================================================
                if (padData->getPadFunction() == PadData::PadFunctions::Midi)
                {
                    static Array<PadData::MidiNote> midiNoteArray;
                    midiNoteArray = padData->getMidiNotes();
                    
                    if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::SingleNote)
                    {
                        
                        //                    if (padData->setVelocity(recievedVelocity))
                        //                    {
                        //
                        //                        MidiMessage outputMessage = MidiMessage::noteOn (padData->getMidiChannel(),
                        //                                                                         midiNoteArray[0].noteNumber,
                        //                                                                         uint8((midiNoteArray[0].velocityPercentage / 100.0)*recievedVelocity));
                        //
                        //                            router->sendMidiToDestination(padData->getMidiDestination(), &outputMessage);
                        //
                        //                    }
                        jassertfalse;
                    }
                    if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::MultiNote)
                    {
                        if (padData->getMultiNoteMode() == PadData::MultiNoteModes::Chord)
                        {
                            if (padData->setVelocity(recievedVelocity))
                            {
                                Array<PadData::MidiNote> midiNoteArray = padData->getMidiNotes();
                                
                                for (int i = 0; i < midiNoteArray.size(); i++)
                                {
                                    
                                    //DBG(midiNoteArray[i].noteNumber);
                                    // DBG(midiNoteArray[i].velocityPercentage);
                                    
                                    MidiMessage outputMessage = MidiMessage::noteOn (padData->getMidiChannel(),
                                                                                     midiNoteArray[i].noteNumber,
                                                                                     uint8((midiNoteArray[i].velocityPercentage / 100.0)*recievedVelocity));
                                    
                                    router->sendMidiToDestination(padData->getMidiDestination(), &outputMessage);
                                }
                                
                            }
                        }                
                    }
                }
                
            }
            
        }

    }
    
}

void PlayablePad::pressPad(const float pressure, const bool killingPad)
{
    if (padData->getPadEnabled())
    {

        //static float rawPressure;
        if (padData->getPressureEnabled() || killingPad)
        {
            static float receivedPressure;
            receivedPressure = pressure;
            
            static float prevPressure = 0;

            
            if (padData->getSticky()) // implement sticky pressure
            {

                if (pressure == 0.0)
                {
                    prevPressure = 0;
                }

                if (receivedPressure < prevPressure)
                {
                    receivedPressure = prevPressure;
                }
                
                // update previous pressure value
                prevPressure = receivedPressure;
            }
            
            
            if (padData->getReversePressure()) //apply pressure reverse
            {
                receivedPressure = MAX_PRESSURE - receivedPressure;
            }
            
            
            //apply range 
            receivedPressure = (receivedPressure * padData->getPressureRange()) + (padData->getPressureMin()*MAX_PRESSURE);
            
            
            if (padData->getPressureDestination() != PadData::PressureDestinations::OSC) // if pressure mode requires value between 0-127
            {
                receivedPressure = (receivedPressure / MAX_PRESSURE) * 127.0;
                
                if (padData->setPadPressure(pressure));
                {
                    MidiMessage message;

                    switch (padData->getPressureDestination()){
                        case PadData::PressureDestinations::PolyAT:
                            message = MidiMessage::aftertouchChange(padData->getMidiChannel(), padData->getMidiNote(), receivedPressure);
                            break;
                        case PadData::PressureDestinations::ChannetAT:
                            message = MidiMessage::channelPressureChange(padData->getMidiChannel(), receivedPressure);
                            break;
                        case PadData::PressureDestinations::MidiCC:
                            message = MidiMessage::controllerEvent(padData->getMidiChannel(), padData->getMidiCC(), receivedPressure);
                            break;
                        case PadData::PressureDestinations::ModWheel:
                            message = MidiMessage::controllerEvent(padData->getMidiChannel(), 1, receivedPressure);
                            break;
                        case PadData::PressureDestinations::PBup:
                            message = MidiMessage::pitchWheel(padData->getMidiChannel(), 8192.0 + (receivedPressure * 64.0));
                            break;
                        case PadData::PressureDestinations::PBDown:
                            message = MidiMessage::pitchWheel(padData->getMidiChannel(), 8192.0 - (receivedPressure * 64.0));
                            break;
                        default:
                            break;
                    }
                    
                    
                    router->sendMidiToDestination(InternalMidiRouter::MidiOut, &message);
                }
            }
        }
        
        
        // Processing for latch/trigger modes. Uses raw pressure value, ignoring any other processing.
        if (padData->getNoteTriggerMode() == PadData::NoteTriggerModes::LatchNoteMode || padData->getNoteTriggerMode() == PadData::NoteTriggerModes::TriggerNoteMode)
        {
            static int minPressure = MAX_PRESSURE;
            static bool stopNoteOnRelease = false;
            
            if (pressure < minPressure && padData->getVelocity() > 0)
            {
                minPressure = pressure;
            }
            
            //DBG(String(minPressure) + ":" + String(pressure));
            
            
            if (padData->Velocity > 0 && pressure >= MAX_PRESSURE) //if pad is on and pressure is at max
            {
                if (minPressure == 0)
                {
                    minPressure = MAX_PRESSURE;
                    stopNoteOnRelease = true;
                }
            }
            
            if (pressure == 0 && stopNoteOnRelease)
            {
                stopNoteOnRelease = false;
                hitPad(0, true);
            }
        }
        
 
    }
}

bool PlayablePad::setMidiNote(const int newMidiNote)
{
    return padData->setMidiNote(newMidiNote);
}

const int PlayablePad::getMidiNote()
{
    return padData->getMidiNote();
}

void PlayablePad::setMidiChannel(const int newMidiChannel)
{
    if (newMidiChannel > 0 && newMidiChannel <= 16)
    {
        padData->setMidiChannel(newMidiChannel);
    }
    else
    {
        jassertfalse;
        //what are you doing?
    }
}
const int PlayablePad::getMidiChannel()
{
    return padData->getMidiChannel();
}

void PlayablePad::killPad()
{
    pressPad(0, true);
    hitPad(0, true);
}

void PlayablePad::setPadEnabled(const bool enabled)
{
    if (enabled != padData->getPadEnabled())
    {
        
        if (!enabled)
        {
            killPad();
        }
        padData->setPadEnabled(enabled);

    }
}


// Master Clock ======================================================
void PlayablePad::barClockCallback()
{
    
}

void PlayablePad::stepClockCallback(const int currentPositionInLoop)
{
    
}

void PlayablePad::masterClockStopped()
{
    
}

void PlayablePad::masterTempoChanged(const int beatsInLoop, const float newTempo)
{
    
}

void PlayablePad::rawClockCallback(const int clock)
{
//    if (padData->getVelocity() > 0)
//    {
//        if ((clock % 6) == 0)
//        {
//            hitPad(127.0* padData->getPadPressure() / MAX_PRESSURE);
//            DBG(padData->getVelocity());
//
//        }
//    }
}

