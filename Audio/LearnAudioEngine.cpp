//
//  LearnAudioEngine.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
//
//

#include "LearnAudioEngine.hpp"


AudioEngine::AudioEngine()
{
    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
    masterMixer.addInputSource(&piano, false);
    masterMixer.addInputSource(&masterClock, false);
}


AudioEngine::~AudioEngine()
{
    shutdownAudio();
}


//=======================================================================
void AudioEngine::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    
    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
    
    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    masterMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioEngine::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()
    
    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    //
    bufferToFill.clearActiveBufferRegion();

    masterMixer.getNextAudioBlock(bufferToFill);
}

void AudioEngine::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    // For more details, see the help for AudioProcessor::releaseResources()
    
    masterMixer.releaseResources();
}


InternalMidiIn* AudioEngine::getVirtualPiano()
{
    return &piano;
}

AudioDeviceManager* AudioEngine::getDeviceManager()
{
    return &deviceManager;
}

MasterClock* AudioEngine::getMasterClock()
{
    return &masterClock;
}

