//
//  EngineUpdateDispatcher.cpp
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
