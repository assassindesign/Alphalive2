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
    virtual void appDataChangeCallback(const int changedData) = 0;
};

class AppDataFormat : public AsyncUpdater
{
public:
    AppDataFormat()
    {
        changedDataIDs.clear();
    }
    ~AppDataFormat()
    {
        appDataListeners.clear();
        repaintListeners.clear();
    }
    
    void addListener(AppDataListener* newListener)
    {
        appDataListeners.add(newListener);
    }
    
    void removeListener(AppDataListener* listenerToRemove)
    {
        appDataListeners.remove(listenerToRemove);
    }
    
    void addListener(GUIRepaintListener* newListener)
    {
        repaintListeners.add(newListener);
    }
    
    void removeListener(GUIRepaintListener* listenerToRemove)
    {
        repaintListeners.remove(listenerToRemove);
    }
    
    void callListeners(const int changedData)
    {
        changedDataIDs.add(changedData);
        triggerAsyncUpdate();
    }
    
    void callRepaintListeners()
    {
        triggerAsyncUpdate();
    }
private:
    void handleAsyncUpdate() override
    {
        repaintListeners.call(&GUIRepaintListener::refreshUI);
        while(changedDataIDs.size() != 0)
        {
            workingDataID = changedDataIDs.getUnchecked(0);
            changedDataIDs.remove(0);
            
            appDataListeners.call(&AppDataListener::appDataChangeCallback, workingDataID);
        }
        
    }
private:
    ListenerList<AppDataListener> appDataListeners;
    ListenerList<GUIRepaintListener> repaintListeners;
    Array<int, CriticalSection> changedDataIDs;
    int workingDataID = 0;
};

#endif /* AppDataListeners_h */
