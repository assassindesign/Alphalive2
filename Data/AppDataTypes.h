//
//  AppDataTypes.h
//  AlphaLive Midi
//
//  Created by Luke Saxton on 16/06/2016.
//
//

#ifndef AppDataTypes_h
#define AppDataTypes_h

// This file contains the structs and patterns for data in the application.
#include "../JuceLibraryCode/JuceHeader.h"
#include "AppDataListeners.h"

class AppDataFormat : public AsyncUpdater
{
public:
    AppDataFormat(){}
    ~AppDataFormat()
    {
        listeners.clear();
    }
    
    void addListener(GUIRepaintListener* newListener)
    {
        listeners.add(newListener);
    }
    
    void removeListener(GUIRepaintListener* listenerToRemove)
    {
        listeners.remove(listenerToRemove);
    }
    
    void callListeners()
    {
        triggerAsyncUpdate();
    }
private:
    void handleAsyncUpdate() override
    {
        listeners.call(&GUIRepaintListener::refreshUI);

    }
private:
    ListenerList<GUIRepaintListener> listeners;
};

#endif /* AppDataTypes_h */
