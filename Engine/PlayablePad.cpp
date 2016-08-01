//
//  PlayablePad.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
//
//

#include "PlayablePad.hpp"
#include "Alphalive2Engine.hpp"


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
    padData->setPadFunction(Midi);

}
PlayablePad::~PlayablePad()
{
    
}

void PlayablePad::hitPad(const int velocity)
{
    if (padData->getPadFunction() == Midi)
    {
        if (padData->setVelocity(velocity))
        {
            MidiMessage outputMessage = MidiMessage::noteOn(padData->getMidiChannel(), padData->getMidiNote(), float(velocity));
            router->sendMidiToDestination(padData->getMidiDestination(), &outputMessage);
        }
    }
    else if (padData->getPadFunction() == Chord)
    {
        if (padData->setVelocity(velocity))
        {
            Array<PadData::MidiNote> midiNoteArray = padData->getMidiNotes();
            
            for (int i = 0; i < midiNoteArray.size(); i++)
            {
                MidiMessage outputMessage = MidiMessage::noteOn (padData->getMidiChannel(), midiNoteArray[i].noteNumber, uint8((100/midiNoteArray[i].velocityPercentage)*velocity));	
                router->sendMidiToDestination(padData->getMidiDestination(), &outputMessage);
            }
            
            
        }
    }
}

void PlayablePad::pressPad(const int pressure)
{
    
}

void PlayablePad::setMidiNote(const int newMidiNote)
{
    padData->setMidiNote(newMidiNote);
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