//
//  RealtimeMessageManager.cpp
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

#include "RealtimeMessageManager.hpp"

/*Class that manages playstate change messages from multiple threads, queueing the events as they arrive and 
 dispatching these events on it's own thread. The class mangaes these playstates using a Valuetree object */

RealtimeMessageDispatch::RealtimeMessageDispatch() : Thread("Realtime Dispatch Thread")
{
    
//    numManagedPads = numPadsToManage;
//    
//    realtimeDataTree = new ValueTree(RealtimeTreeIdentifiers::RealtimeDataTreeType);
//    
//    for (int i = 0; i < numPadsToManage; i++)
//    {
//        realtimeDataTree->addChild(*new ValueTree(RealtimeTreeIdentifiers::Playback::PlaybackDataType), i, 0);
//        
//        realtimeDataTree->getChild(i).setProperty(RealtimeTreeIdentifiers::Playback::PadIDType, i, 0);
//        realtimeDataTree->getChild(i).setProperty(RealtimeTreeIdentifiers::Playback::PlaystateType, i, 0);
//        realtimeDataTree->getChild(i).setProperty(RealtimeTreeIdentifiers::Playback::PadPressureType, i, 0);
//        
//    }
//    
    
//    ReadOnlyValueTree treeToEdit = realtimeDataTree->getChildWithName(RealtimeTreeIdentifiers::Playback::PlaybackDataType);
    
    
//    if (valueTreeToManage->getType() == RealtimeTreeIdentifiers::Playback::RealtimeDataTreeType)
//    {
//        treeToManage = valueTreeToManage;
//    }
    
}

RealtimeMessageDispatch::~RealtimeMessageDispatch()
{
    
}


void RealtimeMessageDispatch::run()
{
    
}
