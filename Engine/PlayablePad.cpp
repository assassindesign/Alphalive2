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

}
PlayablePad::~PlayablePad()
{
    
}

void PlayablePad::hitPad(const int velocity)
{
    
    if (rawVelocity != velocity) //avoid unneccessary processing
    {
        rawVelocity = velocity;
        //===Velocity Curve===============================================================================
        
        static float recievedVelocity;;
        switch (padData->getVelocityCurve())
        {
            case PadData::CurveTypes::Logarithmic:
                //logarithmic mapping of velocity
                recievedVelocity = log(velocity+1);
                recievedVelocity = recievedVelocity * (MAX_VELOCITY/4.85); // not sure why 4.85 here!
                if (recievedVelocity > MAX_VELOCITY)
                    recievedVelocity = MAX_VELOCITY;
                break;
                
            default:
                recievedVelocity = velocity;
                break;
        }
        
        //recievedVelocity = scaleValue (recievedVelocity, 0, 127.0, 0, 127.0);
        
        //===Pad Function================================================================================
        
        if (padData->getPadFunction() == PadData::PadMidiFunctions::SingleNote)
        {
            if (padData->setVelocity(recievedVelocity))
            {
                MidiMessage outputMessage = MidiMessage::noteOn(padData->getMidiChannel(), padData->getMidiNote(), uint8(recievedVelocity));
                router->sendMidiToDestination(padData->getMidiDestination(), &outputMessage);

            }
        }
        else if (padData->getPadFunction() == PadData::PadMidiFunctions::MultiNote)
        {
            if (padData->getMultiNoteMode() == PadData::MultiNoteModes::Chord)
            {
                if (padData->setVelocity(recievedVelocity))
                {
                    Array<PadData::MidiNote> midiNoteArray = padData->getMidiNotes();
                    
                    for (int i = 0; i < midiNoteArray.size(); i++)
                    {
                        MidiMessage outputMessage = MidiMessage::noteOn (padData->getMidiChannel(), midiNoteArray[i].noteNumber, uint8((100/midiNoteArray[i].velocityPercentage)*recievedVelocity));
                        router->sendMidiToDestination(padData->getMidiDestination(), &outputMessage);
                    }
                    
                    
                }
            }
           
        }
    }
    
}

void PlayablePad::pressPad(const float pressure)
{
    float receivedPressure = pressure;
    
    if (pressure != padData->getPadPressure()) //avoid unneccessary processing
    {
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