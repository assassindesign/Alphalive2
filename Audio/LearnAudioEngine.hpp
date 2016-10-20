//
//  LearnAudioEngine.hpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
//
//

#ifndef LearnAudioEngine_hpp
#define LearnAudioEngine_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"
#include "VirtualPiano.hpp"
#include "MasterClock.hpp"
/**
    Audio I/O for app. 
 
    Should contain list of audiosources and mix them for output.
 */
class AudioEngine : public AudioAppComponent
{
public:
    AudioEngine();
    ~AudioEngine();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    InternalMidiIn* getVirtualPiano();
    
    AudioDeviceManager* getDeviceManager();
    
    MasterClock* getMasterClock();

private:
    VirtualPiano piano;
    MixerAudioSource masterMixer;
    MasterClock masterClock;
};
#endif /* LearnAudioEngine_hpp */
