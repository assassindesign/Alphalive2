//
//  SimpleAudioBufferSource.hpp
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

#ifndef SimpleAudioBufferSource_hpp
#define SimpleAudioBufferSource_hpp

#include "../JuceLibraryCode/JuceHeader.h"

/** 
    Simple class to convert an audio buffer into a positionable audio source. Uses a thread
    queue to move any property setting function calls to the Audio thread.
 */

class SimpleAudioBufferSource : public PositionableAudioSource
{
public:
    SimpleAudioBufferSource();
    
    ~SimpleAudioBufferSource();
    
    
    //==AudioSource======================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    void releaseResources() override;
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    //==PositionableAudioSource==========================================
    void setNextReadPosition (int64 newPosition) override;

    int64 getNextReadPosition() const override;

    int64 getTotalLength() const override;

    bool isLooping() const override;

    void loadAudioFile(File* newFile);
        
    void loadBinaryResource(const char* data, const int sizeOf);
    
private:
    void loadReaderIntoBuffer(AudioFormatReader* readerToLoad);
    
    ScopedPointer<AudioBuffer<float>> buffer;
    
    int64 readPosition, nextReadPosition;
    
    CriticalSection audioBufferLock;
    
    bool playing;
    
};

#endif /* SimpleAudioBufferSource_hpp */
