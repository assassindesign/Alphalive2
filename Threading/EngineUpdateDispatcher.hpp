//
//  EngineUpdateDispatcher.hpp
//  AlphaLive 2
//
//  Created by Luke Saxton on 04/02/2016.
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
