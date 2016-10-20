//
//  DoubleBufferAudioSource.cpp
//  iAlpha
//
//  Created by Luke Saxton on 13/11/2015.
//  Copyright 2015 nu desine.
//
//  iAlpha is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  iAlpha is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "DoubleBufferAudioSource.hpp"


DoubleBufferAudioSource::DoubleBufferAudioSource(TimeSliceThread* sharedTimeSlice, MasterClock* masterClock, ValueTree positionToUpdate)
{
    timeSliceThread = sharedTimeSlice;
    tempo = masterClock->getTempo();
    positionData = positionToUpdate;
    masterClock->addListener(this);
    
    commonInit();
}


void DoubleBufferAudioSource::commonInit()
{
    for (int i = 0; i < 3; i++)
    {
        transportSources.add(new AudioTransportSource());
        transportSources.getLast()->addChangeListener(this);
        mixer.addInputSource(transportSources.getLast(), false);
    }
    
    for (int i = 0; i < 2; i++)
    {
        fileInfo.add(new FileInfo());
    }
    
    shouldStart = shouldStop = playing = looping = false;
    
    currentReadHead = HeadA;
}


DoubleBufferAudioSource::~DoubleBufferAudioSource()
{

    //deleteAndZero(buffer);
}



//===Audio Source===========================================

void DoubleBufferAudioSource::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
//    transportSources[HeadA]->getNextAudioBlock(bufferToFill);
//    transportSources[HeadB]->getNextAudioBlock(bufferToFill);
    mixer.getNextAudioBlock(bufferToFill);
}


void DoubleBufferAudioSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
   
    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DoubleBufferAudioSource::releaseResources()
{
    mixer.releaseResources();
}


bool DoubleBufferAudioSource::loadFileIntoBuffer(File fileToLoad, BufferType bufferToSet)
{
    if (playing)
    {
        setPlaying(false);
    }
    
    if (!timeSliceThread->isThreadRunning())
    {
        timeSliceThread->startThread();
    }
    
    // create a new file source from the file..
    // get a format manager and set it up with the basic types (wav, ogg and aiff).
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    formatReaders.set(bufferToSet, formatManager.createReaderFor (fileToLoad));
    
    FileInfo* infoForThisFile = fileInfo.getUnchecked(bufferToSet);
    infoForThisFile->beats = 0;
    
    double samplesInFile = formatReaders.getUnchecked(bufferToSet)->lengthInSamples;
    double samplesPerBeat = (formatReaders.getUnchecked(bufferToSet)->sampleRate) / (tempo / 60) ;
    
    while (samplesInFile > samplesPerBeat)
    {
        infoForThisFile->beats++;
        
        samplesInFile -= samplesPerBeat;
    }
    
    if (samplesInFile < (samplesPerBeat / 2.0))
    {
        infoForThisFile->endsAfterBeat = true;
        infoForThisFile->endsBeforeBeat = false;
    }
    else
    {
        infoForThisFile->beats++;
        infoForThisFile->endsAfterBeat = false;
        infoForThisFile->endsBeforeBeat = true;
    }
    
    std::cout << "\n" << infoForThisFile->beats << " Beats Long : ";
    
    //positionData.setProperty(PositionInLoopType, 0, nullptr);
    //positionData.setProperty(BeatsInFileType, infoForThisFile->beats, nullptr);

    
    if (formatReaders.getUnchecked(bufferToSet) != 0)
    {
        
        transportSources[HeadA]->setSource(0);
        transportSources[HeadB]->setSource(0);
        
        formatReaderSources.set(bufferToSet, new AudioFormatReaderSource(formatReaders.getUnchecked(bufferToSet), false));
        
        //formatReaderSources.getUnchecked(bufferToSet)->setLooping(looping);
        int sampleRate = formatReaders.getUnchecked(bufferToSet)->sampleRate;
        
        transportSources[HeadA]->setSource(formatReaderSources.getFirst(), 65536, timeSliceThread, sampleRate);
        transportSources[HeadB]->setSource(formatReaderSources.getFirst(), 65536, timeSliceThread, sampleRate);
        

        return true;
    }
    return false;
}


void DoubleBufferAudioSource::setGain(const float newGain)
{
    for (int i = 0; i < transportSources.size(); i++)
    {
        transportSources[i]->setGain(newGain);
    }
}


void DoubleBufferAudioSource::start()
{

    if (transportSources[HeadA]->isPlaying())
    {
        transportSources[HeadB]->setPosition(0.0);
        transportSources[HeadB]->start();
        transportSources[HeadA]->stop();

    }
    else if (transportSources[HeadB]->isPlaying())
    {
        transportSources[HeadA]->setPosition(0.0);
        transportSources[HeadA]->start();
        transportSources[HeadB]->stop();
    }
    else
    {
        transportSources[HeadA]->setPosition(0.0);

        transportSources[HeadA]->start();
    }
    fileInfo[Foreground]->position = 0;
}



void DoubleBufferAudioSource::stop()
{
    //playing = false;
    shouldBePlaying = false;
    
    if (transportSources[HeadA]->isPlaying())
    {
        stop(HeadA);
    }
    
    if (transportSources[HeadB]->isPlaying())
    {
        stop(HeadB);

    }
}


void DoubleBufferAudioSource::changeListenerCallback (ChangeBroadcaster *source)
{
    playing = transportSources.getUnchecked(HeadA)->isPlaying() || transportSources.getUnchecked(HeadB)->isPlaying() || transportSources.getUnchecked(HeadC)->isPlaying();
    
    if (!playing && shouldBePlaying && !looping)
    {
        shouldBePlaying = false;
    }
    
    //std::cout << "\nPlaying: " << transportSources[HeadA]->isPlaying() << " : " << transportSources[HeadB]->isPlaying();

    listeners.call(&Listener::sourceStartedOrStopped, nullptr);
}



bool DoubleBufferAudioSource::isLooping() const
{
    return looping;
}

bool DoubleBufferAudioSource::isPlaying() const
{
//    transportSources.getUnchecked(HeadA)->isPlaying() || transportSources.getUnchecked(HeadB)->isPlaying() || transportSources.getUnchecked(HeadC)->isPlaying();
    
    if (!playing)
    {
        return playing;
    }
    else
    {
        return shouldBePlaying;
    }
}

void DoubleBufferAudioSource::setLooping(bool shouldLoop)
{
    looping = shouldLoop;
}

//===Internal===============================================
void DoubleBufferAudioSource::start(ReadHead readerToStart)
{
    transportSources[readerToStart]->start();
    
}

//===Internal===============================================
void DoubleBufferAudioSource::stop(ReadHead readerToStop)
{
    transportSources[readerToStop]->stop();
    
    transportSources[readerToStop]->setPosition(0.0);
    
    fileInfo[Foreground]->position = 0;
}

void DoubleBufferAudioSource::setPlaying(bool newState)
{
    shouldBePlaying = newState;
    if (newState)
    {
        if (looping)
        {
            shouldStart = true;
            shouldStop = false;
        }
        else
        {
            start();
        }
    }
    else
    {
        if (looping)
        {

            shouldStop = true;
            shouldStart = false;

        }
        else
        {
            stop();
        }
    }
    
    
    playing = transportSources[HeadB]->isPlaying() || transportSources[HeadA]->isPlaying();
    listeners.call(&Listener::sourceStartedOrStopped, nullptr);
}



AudioFormatReader* DoubleBufferAudioSource::getAudioFormatReader(BufferType bufferToGet) const
{
    AudioFormatReader* readerToReturn = nullptr;
    
    switch (bufferToGet) {
        case Foreground:
            readerToReturn = formatReaders.getFirst();
            break;
        case Background:
            readerToReturn = formatReaders.getFirst();
            break;
        default:
            break;
    }
    
    return readerToReturn;
}

AudioFormatReaderSource* DoubleBufferAudioSource::getAudioFormatReaderSource(BufferType bufferToGet) const
{
    AudioFormatReaderSource* sourceToReturn = nullptr;
    switch (bufferToGet) {
        case Foreground:
            sourceToReturn = formatReaderSources.getFirst();
            break;
        case Background:
            sourceToReturn = formatReaderSources.getFirst();
            break;
        default:
            break;
    }
    
    return sourceToReturn;
}

AudioTransportSource* DoubleBufferAudioSource::getAudioTransportSource(BufferType bufferToGet) const
{
    AudioTransportSource* sourceToReturn = nullptr;
    
    switch (bufferToGet) {
        case Foreground:
            sourceToReturn = transportSources.getFirst();
            break;
        case Background:
            sourceToReturn = transportSources.getLast();
            break;
        default:
            break;
    }
    
    return sourceToReturn;
}


void DoubleBufferAudioSource::addListener(DoubleBufferAudioSource::Listener* const listenerToAdd)
{
    listeners.add(listenerToAdd);
}

void DoubleBufferAudioSource::removeListener(DoubleBufferAudioSource::Listener* const listenerToRemove)
{
    listeners.remove(listenerToRemove);
    
}

void DoubleBufferAudioSource::barClockCallback()
{
    //play loops that have been triggered recently.
    if (shouldStart)
    {
        start();
        shouldStart =false;
    }
    if (shouldStop)
    {
        stop();
        shouldStop = false;
    }
    
}

void DoubleBufferAudioSource::stepClockCallback(const int currentPositionInLoop)
{
    
    if (shouldBePlaying)
    {
        fileInfo[Foreground]->position++;
    }

    
    if (looping)
    {
        if (fileInfo[Foreground]->endsBeforeBeat && fileInfo[Foreground]->position == fileInfo[Foreground]->beats - 1)
        {
            //fileInfo[Foreground]->position++;
        }
        
        if (fileInfo[Foreground]->position >= fileInfo[Foreground]->beats)
        {
//            if (shouldBePlaying)
//            {
                setPlaying(true);
//            }
            
        }
    }
    
    if (fileInfo[Foreground]->position > fileInfo[Foreground]->beats)
    {
        fileInfo[Foreground]->position = 0;
    }
   
}

void DoubleBufferAudioSource::masterClockStopped()
{
    stop();
    //listeners.call(&Listener::sourceStartedOrStopped, nullptr);
}

void DoubleBufferAudioSource::masterTempoChanged(const int beatsInLoop, const float newTempo)
{
    tempo = newTempo;
}

