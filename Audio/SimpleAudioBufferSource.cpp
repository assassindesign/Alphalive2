//
//  SimpleAudioBufferSource.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 15/02/2016.
//
//

#include "SimpleAudioBufferSource.hpp"

SimpleAudioBufferSource::SimpleAudioBufferSource()
{
    readPosition = 0;
    
    buffer = new AudioBuffer<float>();
    //loadAudioFile
    
}

SimpleAudioBufferSource::~SimpleAudioBufferSource()
{
}




//==AudioSource======================================================
void SimpleAudioBufferSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    setNextReadPosition(0);
}

void SimpleAudioBufferSource::releaseResources()
{

}

void SimpleAudioBufferSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    
    bufferToFill.clearActiveBufferRegion();
    
//    bufferToFill.clearActiveBufferRegion();
    ScopedLock sl(audioBufferLock);
    
    readPosition = nextReadPosition;
    
    
    if (bufferToFill.numSamples > 0) //if buffer needs filling;
    {
        int channelsInBuffer = buffer->getNumChannels();
        int samplesInBuffer = buffer->getNumSamples();
        int samplesToCopy = 0;
        


        if (readPosition + bufferToFill.numSamples <= samplesInBuffer) //if copy range is within buffer
        {
            samplesToCopy = bufferToFill.numSamples;
        }
        else if (samplesInBuffer - readPosition > 0) //else if some of copy range is in buffer
        {
            samplesToCopy = samplesInBuffer - readPosition;
        }

        if (samplesToCopy > 0) //if samples should be copied
        {
            for (int channel = 0; channel < channelsInBuffer; channel++)
            {
                bufferToFill.buffer->copyFrom(channel, bufferToFill.startSample, *buffer, channel, readPosition, samplesToCopy);
                
            }
            
            playing = true;
            
            readPosition += samplesToCopy;
            nextReadPosition = readPosition;
    
        }
        else // nothing left in buffer to copy
        {
            playing = false;
        }

    }
}

//==PositionableAudioSource==========================================
void SimpleAudioBufferSource::setNextReadPosition (int64 newPosition)
{
    
    nextReadPosition = newPosition;
}


int64 SimpleAudioBufferSource::getNextReadPosition() const
{
    return readPosition;
}

int64 SimpleAudioBufferSource::getTotalLength() const
{
    return buffer->getNumSamples();
}

bool SimpleAudioBufferSource::isLooping() const
{
    return false;
}

/**
    Loads an audio file into the audio buffer
 */
void SimpleAudioBufferSource::loadAudioFile(File* newFile)
{
    
    if (newFile != 0) //if the file exists
    {
        AudioFormatManager formatManager;
        formatManager.registerBasicFormats(); // register .wav and .aif/aiff files as valid.
        ScopedPointer<AudioFormatReader> reader = formatManager.createReaderFor(*newFile); // set up reader to read file
        loadReaderIntoBuffer(reader); //actually make load data into buffer.
    }
    
}

/** 
    Uses a MemoryInputStream to convert a file stored in Binary data to audio.
 
    Will accept any data and try to load it into a buffer! be careful!
 */
void SimpleAudioBufferSource::loadBinaryResource(const char* data, const int sizeOf)
{
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    MemoryInputStream* inputStream = new MemoryInputStream(data, sizeOf, false);
    ScopedPointer<AudioFormatReader> reader = formatManager.createReaderFor(inputStream);
    loadReaderIntoBuffer(reader);
}


/** 
    Internal function that copies the content of an AudioFormatReader into the AudioBuffer.
 */
void SimpleAudioBufferSource::loadReaderIntoBuffer(AudioFormatReader* readerToLoad)
{
    if (readerToLoad != 0)
    {
        ScopedLock sl(audioBufferLock);
        buffer->clear();
        buffer = new AudioBuffer<float>(readerToLoad->numChannels, readerToLoad->lengthInSamples);
        readerToLoad->read(buffer, 0, readerToLoad->lengthInSamples, 0, true, true);
        buffer->applyGainRamp(0, 128, 0.0, 1.0);
        //buffer->applyGainRamp(0, buffer->getNumSamples(), 1.0, 0);

        buffer->applyGainRamp(buffer->getNumSamples()-512, 512, 1.0, 0.0);
        
        readPosition = 0;
    }
    else
    {
        jassertfalse;
        //Not an audio file! maybe error popup should go here?
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Whoops!", "That's not an audio file..");
    }
}

