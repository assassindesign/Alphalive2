//
//  VirtualPiano.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 15/02/2016.
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

#include "VirtualPiano.hpp"


VirtualPiano::VirtualPiano() : timeSliceThread("PianoPlaybackThread")
{
    timeSliceThread.startThread();
    
    ScopedPointer<File> currentFile;

    for (int i = 0; i < 61; i++)
    {
        //notes.add(new DoubleBufferAudioSource(timeSliceThread, ));
        notes.add(new SimpleAudioBufferSource());
        //notes.getLast()->loadAudioFile(new File("/Assets/malletinst_4test/" + String(i+36) + ".wav"));
        
        ScopedPointer<String> resourceName;
        int resourceSize = 0;
        
        resourceName = new String("_" + String(i+36) + "_wav");
        
        notes.getLast()->loadBinaryResource( BinaryData::getNamedResource(resourceName->toUTF8(), resourceSize), resourceSize);

        transport.add(new AudioTransportSource());
        transport.getLast()->setSource(notes.getLast(), 16384, &timeSliceThread);
        //resamplingSources.add(new ResamplingAudioSource(transport.getLast(), false));
        //resamplingSources.getLast()->setResamplingRatio(0.99);
        
        getMixer()->addInputSource(transport.getLast(), false);
    }
    
    pianoReverb.setFXType(FXData::FXType::ReverbEffect);
    pianoReverb.setParameter(0, 0.3);
    pianoReverb.setParameter(1, 0.5);
    pianoReverb.setParameter(2, 0.2);
    pianoReverb.setParameter(3, 1);
    
    hpf.setFXType(FXData::FXType::HPFEffect);
    hpf.setParameter(1, 50);
    
}


VirtualPiano::~VirtualPiano()
{
    
}

void VirtualPiano::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    getMixer()->getNextAudioBlock(bufferToFill);
    
    //transport.getUnchecked(note-LOWEST_SAMPLED_NOTE)->setGain(powf(vel/127.0, 3));

    hpf.getNextAudioBlock(bufferToFill);
    pianoReverb.getNextAudioBlock(bufferToFill);
}

void VirtualPiano::handleInternalMidiNote(const int note, const int vel)
{
    if (note <= HIGHEST_SAMPLED_NOTE && note >= LOWEST_SAMPLED_NOTE)
    {
        if (vel > 0)
        {
            transport.getUnchecked(note-LOWEST_SAMPLED_NOTE)->setGain(powf(vel/127.0, 3));
            //transport.getUnchecked(note-LOWEST_SAMPLED_NOTE)->stop();
            transport.getUnchecked(note-LOWEST_SAMPLED_NOTE)->setNextReadPosition(0);
            transport.getUnchecked(note-LOWEST_SAMPLED_NOTE)->start();
        }
        else
        {
            transport.getUnchecked(note-LOWEST_SAMPLED_NOTE)->stop();
            transport.getUnchecked(note-LOWEST_SAMPLED_NOTE)->setNextReadPosition(0);
        }


    }
    else
    {
        DBG("Note Number out of range: " + String(note));
    }
    
}

void VirtualPiano::loadResourcesIntoBuffers()
{
    
    
}



