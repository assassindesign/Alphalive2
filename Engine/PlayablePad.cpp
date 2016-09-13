//
//  PlayablePad.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
//
//

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
    
    rawVelocity = 0;
    killingPad = false;

}
PlayablePad::~PlayablePad()
{
    
}

void PlayablePad::hitPad(const int velocity)
{
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
                    if (padData->setVelocity(recievedVelocity))
                    {
                        
                        MidiMessage outputMessage = MidiMessage::noteOn (padData->getMidiChannel(),
                                                                         midiNoteArray[0].noteNumber,
                                                                         uint8((midiNoteArray[0].velocityPercentage / 100.0)*recievedVelocity));
                        
                        router->sendMidiToDestination(padData->getMidiDestination(), &outputMessage);
                        
                    }
                }
                else if (padData->getPadMidiFunction() == PadData::PadMidiFunctions::MultiNote)
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
        
        if (killingPad)
        {
            killingPad = !killingPad;
        }
    }
    
    
}

void PlayablePad::pressPad(const float pressure)
{
    if (padData->getPressureEnabled() || killingPad)
    {
        float receivedPressure = pressure;
        
        if (padData->getPressureDestination() != PadData::PressureDestinations::OSC) // if pressure mode requires value between 0-127
        {
            receivedPressure = (pressure / 511.0) * 127.0;
            
            if (padData->setPadPressure(receivedPressure));
            {
                MidiMessage message;
                message = MidiMessage::aftertouchChange(padData->getMidiChannel(), padData->getMidiNote(), receivedPressure);
                router->sendMidiToDestination(InternalMidiRouter::MidiOut, &message);
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
    killingPad = true;
    pressPad(0);
    hitPad(0); 
}

