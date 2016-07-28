//
//  EngineUpdateDispatcher.cpp
//  AlphaLive 2
//
//  Created by Luke Saxton on 04/02/2016.
//
//

#include "EngineUpdateDispatcher.hpp"
//#include "TreeManager.hpp"
#include "bond.hpp"

EngineUpdateDispatcher::EngineUpdateDispatcher() : Thread("Engine Update Thread")
{
    messageQueue.open();
    
//    var updateValue = OneShotType.toString();
//    messageQueue.call(bond (&TreeManager::updateValueTree, TreeManager::Instance(), TreeManager::Instance()->getPadTree(0, 0), PadFunctionType, updateValue));
    
    
    //messageQueue.call(bond(&TreeManager::testing, TreeManager::Instance()));
    
    startThread();
}

EngineUpdateDispatcher::~EngineUpdateDispatcher()
{

    stopThread(500);
    messageQueue.close();
}

void EngineUpdateDispatcher::run()
{
    while (!threadShouldExit())
    {
        //process whatever is in the queue, then wait to be notified of a new message.
        
        messageQueue.process();
        wait(2000);
    }
    
}

ThreadQueue* EngineUpdateDispatcher::getQueue ()
{
    return &messageQueue;
}