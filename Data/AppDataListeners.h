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

//=====================================================================================================

/* 
    UI elements that are updated frequently (like the pads) are set up so they pull the value of variables
    direct from an AppData class in the paint() function. Inheriting from this class allows a UI Component
    to receive a repaint() call when data it is listening to is changed. The refreshUI() call just sets a 
    boolean and the actual call is made by a timerCallback, this improves thread-safety and stops rapid 
    repaints from causing huge CPU usage.
 
    To make use of this functionality, the UI component should inherit from GUIRepaintListener and 
    call the addListener function of an AppDataFormat object.
*/
class GUIRepaintListener : public Component,
                           public Timer
{
public:
    const int refreshRate = 60;
    GUIRepaintListener()
    {
        timeOfLastPaint.set(Time::currentTimeMillis());
        startTimer(100);
    }
    virtual ~GUIRepaintListener(){}
    void refreshUI()
    {
        if ((Time::currentTimeMillis() - timeOfLastPaint.get()) > refreshRate)
        {
            repaint();
            timeOfLastPaint.set(Time::currentTimeMillis());
            repaintQueued.set(false);

        }
        else
        {
            repaintQueued.set(true);
        }
    }
    virtual void paint(Graphics&) override = 0;
    virtual void resized() override = 0;
private:
    void timerCallback() override
    {
        if ((Time::currentTimeMillis() - timeOfLastPaint.get()) > refreshRate && repaintQueued.get())
        {
            repaint();
            repaintQueued.set(false);
        }
        else if (Time::currentTimeMillis() - timeOfLastPaint.get() > 2000)
        {
            //stopTimer();
        }
    }
    Atomic<int64> timeOfLastPaint;
    Atomic<int> repaintQueued;
};


/* 
    Base Class for UI elements that need to react to changes in the app state. AppDataFormat classes 
    will call these functions when relevant data changes. To receive updates, inherit from AppDataListener
    and call the addListenerFunction on any AppDataFormat object.
 
    changedData refers to the DataIDs enum that each AppDataFormat class should have, which allows you to
    identify exactly which variables have changed and react accordingly.
 */
class AppDataListener
{
public:
    virtual ~AppDataListener(){};
    virtual void appDataChangeCallback(const int changedData) {};
    virtual void padDataChangeCallback(const int changedData) {};
    virtual void sphereDataChangeCallback(const int changedData) {};
    virtual void scaleDataChangeCallback(const int changedData) {};
    virtual void tempoDataChangeCallback(const int changedData) {};

};

/*
    Base class for all AppData objects. Keeps a ListenerList of both current types of data listener
    and triggers updates on the Message thread. To keep this class as light is possible, incoming
    calls are just stored as 2 integer values in a FIFO queue.
 
    All AppData objects should inherit from this  class, and contain a DataIDs enum that gives each
    accessible variable a unique ID. the 'set' variable functions should call the relevant callListeners(,)
    function once the input data has been validated and the variable set, this should trigger an update of 
    any UI elements that are listening to this object.
 */

class AppDataFormat : public ActionBroadcaster,
                      public ActionListener
{
public:
    enum AppDataListenerType{
        AppDataType = 0,
        PadDataType,
        SphereDataType,
        ScaleDataType,
        TempoDataType,
    };
    
    class DataChange
    {
    public:
        DataChange(const int data, const int listener) : dataID(data), listenerType(listener){};
        ~DataChange(){};
        int dataID;
        int listenerType;
    };
        
    AppDataFormat() : workingDataChange(0,0)
    {
        changedDataIDs.clear();
        changedDataIDs.ensureStorageAllocated(2048);
        addActionListener(this);
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
    
    void callListeners(const int changedData, const int listenerType)
    {
        changedDataIDs.add(DataChange(changedData, listenerType));
        
        sendActionMessage("DATA");
    }
    
    void callRepaintListeners()
    {
        sendActionMessage("REPAINT");
        //triggerAsyncUpdate();
    }
private:
    void actionListenerCallback (const String& message) override
    {
        if (message == "REPAINT")
        {
            repaintListeners.call(&GUIRepaintListener::refreshUI);
        }
        else
        {
            while(changedDataIDs.size() != 0)
            {
                workingDataChange = changedDataIDs.getUnchecked(0);
                
                switch(workingDataChange.listenerType)
                {
                    case AppDataType:
                        appDataListeners.call(&AppDataListener::appDataChangeCallback, workingDataChange.dataID);
                        break;
                    case PadDataType:
                        appDataListeners.call(&AppDataListener::padDataChangeCallback, workingDataChange.dataID);
                        break;
                    case SphereDataType:
                        appDataListeners.call(&AppDataListener::sphereDataChangeCallback, workingDataChange.dataID);
                        break;
                    case ScaleDataType:
                        appDataListeners.call(&AppDataListener::scaleDataChangeCallback, workingDataChange.dataID);
                        break;
                    case TempoDataType:
                        appDataListeners.call(&AppDataListener::tempoDataChangeCallback, workingDataChange.dataID);
                        break;
                    default:
                        break;
                }
             
                changedDataIDs.remove(0);
            }
        }
        
    }
    
    ListenerList<AppDataListener> appDataListeners;
    ListenerList<GUIRepaintListener> repaintListeners;
    Array<DataChange, CriticalSection> changedDataIDs;
    DataChange workingDataChange;

};

#endif /* AppDataListeners_h */
