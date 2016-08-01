//
//  AppDataListeners.h
//  AlphaLive Midi
//
//  Created by Luke Saxton on 16/06/2016.
//
//

#ifndef AppDataListeners_h
#define AppDataListeners_h
#include "../JuceLibraryCode/JuceHeader.h"

/* 
    This file contains the basic templates for classes that listen to changes in appdata.
    AppData data objects should inherit from AppDataFormat, allowing them to register and call 
    Listeners when data is updated.
 */

class GUIRepaintListener : public Component
{
public:
    GUIRepaintListener(){};
    virtual ~GUIRepaintListener(){};
    void refreshUI() {repaint();};
    virtual void paint(Graphics&) = 0;
    virtual void resized() = 0;
};

class AppDataListener
{
public:
    AppDataListener(){};
    virtual ~AppDataListener(){};
    virtual void appDataChangeCallback() = 0;
};

class AppDataFormat : public AsyncUpdater
{
public:
    AppDataFormat(){}
    ~AppDataFormat()
    {
        listeners.clear();
    }
    
    void addListener(AppDataListener* newListener)
    {
        listeners.add(newListener);
    }
    
    void removeListener(AppDataListener* listenerToRemove)
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
        listeners.call(&AppDataListener::appDataChangeCallback);
        
    }
private:
    ListenerList<AppDataListener> listeners;
};

#endif /* AppDataListeners_h */
