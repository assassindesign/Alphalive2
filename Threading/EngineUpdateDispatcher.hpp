//
//  EngineUpdateDispatcher.hpp
//  AlphaLive 2
//
//  Created by Luke Saxton on 04/02/2016.
//
//

#ifndef EngineUpdateDispatcher_hpp
#define EngineUpdateDispatcher_hpp

//#include "iAlphaHeader.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ThreadQueue.hpp"
//#include "bond.hpp"

/** 
 Super simple class that wraps a ThreadQueue with it's own thread for processing.
 to use: getQueue()->call(bond(&ClassWithFunction::Function, *ObjectToCallFunctionOn, parameter1, parameter2, parameter3));
 then use EngineUpdateDispatcher->Notify() to wake the processing thread.
 */

class EngineUpdateDispatcher : public Thread
{
public:
    EngineUpdateDispatcher();
    ~EngineUpdateDispatcher();
    
    void run();
    
    ThreadQueue* getQueue ();
    
private:
    ThreadQueue messageQueue;
    
};



#endif /* EngineUpdateDispatcher_hpp */
