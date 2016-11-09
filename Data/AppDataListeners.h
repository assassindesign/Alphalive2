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
//            if (!isTimerRunning()) {
//                startTimer(100);
//            }
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
