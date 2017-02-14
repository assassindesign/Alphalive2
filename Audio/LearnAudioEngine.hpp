//
//  LearnAudioEngine.hpp
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
