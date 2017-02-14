//
//  VirtualPiano.hpp
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

#ifndef VirtualPiano_hpp
#define VirtualPiano_hpp

#include "VirtualInstrument.h"
#include "SimpleAudioBufferSource.hpp"
#include "FXUnit.hpp"

#define LOWEST_SAMPLED_NOTE 36
#define HIGHEST_SAMPLED_NOTE 96

/** 
    Virtual Piano instrument. Accepts midi and returns piano audio.
    
    Currently only using simple sampling engine, but will be expanded to support multi-sampling and pitching by using resamplingAudioSources.
 */

class VirtualPiano : public VirtualInstrument
{
public:
    VirtualPiano();
    ~VirtualPiano();
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    
    void handleInternalMidiNote(const int note, const int vel) override;
    
private:
    void loadResourcesIntoBuffers();

    
    OwnedArray<SimpleAudioBufferSource> notes;
    OwnedArray<ResamplingAudioSource> resamplingSources;
    OwnedArray<AudioTransportSource> transport;
    
    FXUnit pianoReverb;
    FXUnit hpf;
    
    
    
    TimeSliceThread timeSliceThread;
};

#endif /* VirtualPiano_hpp */
