//
//  RealtimeMessageManager.hpp
//  AlphaLive 2
//
//  Created by Luke Saxton on 01/02/2016.
//
//

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
