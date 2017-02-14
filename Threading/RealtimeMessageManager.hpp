//
//  RealtimeMessageManager.hpp
//  AlphaLive 2
//
//  Created by Luke Saxton on 01/02/2016.
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

#ifndef RealtimeMessageManager_hpp
#define RealtimeMessageManager_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "ThreadQueue.hpp"
#include "bond.hpp"

namespace RealtimeTreeIdentifiers
{
    const Identifier RealtimeDataTreeType ("RealtimeDataTree");
    
    namespace Playback
    {
        const Identifier PlaybackDataType ("playbackData");
        const Identifier PadIDType ("PadID");
        const Identifier PlaystateType ("Playstate");
        const Identifier PadPressureType ("PadPressure");
    };
    
    namespace Parameter
    {
        const Identifier ParameterDataType ("ParameterData");
        const Identifier MinimumValueType ("MinValue");
        const Identifier MaximumValueType ("MaxValue");
        const Identifier CurrentScaledValueType ("CurrentScaledValue");
    };



};


class RealtimeMessageDispatch: Thread
{
public:
  
    
    RealtimeMessageDispatch();
    
    ~RealtimeMessageDispatch();
    
    void run() override;
    
    
    
private:
    
    
    
    CriticalSection realtimeSharedMemory;
    
};


#endif /* RealtimeMessageManager_hpp */
