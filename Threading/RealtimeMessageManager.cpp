//
//  RealtimeMessageManager.cpp
//  AlphaLive 2
//
//  Created by Luke Saxton on 01/02/2016.
//
//

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