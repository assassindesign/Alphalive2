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
    virtual void appDataChangeCallback(const int changedData) {};
    virtual void padDataChangeCallback(const int changedData) {};
    virtual void sphereDataChangeCallback(const int changedData) {};
    virtual void scaleDataChangeCallback(const int changedData) {};
    virtual void tempoDataChangeCallback(const int changedData) {};

};

class AppDataFormat : public AsyncUpdater
{
public:
    enum AppDataListenerType{
        AppDataType = 0,
        PadDataType,
        SphereDataType,
        ScaleDataType,
        TempoDataType
    };
    
    struct DataChange{
        int dataID;
        int listenerType;
    };
    
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
    
    void callListeners(const int changedData, const int listenerType)
    {
        DataChange* change = new DataChange();
        change->dataID = changedData;
        change->listenerType = listenerType;
        changedDataIDs.add(*change);
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
private:
    ListenerList<AppDataListener> appDataListeners;
    ListenerList<GUIRepaintListener> repaintListeners;
    Array<DataChange, CriticalSection> changedDataIDs;
    DataChange workingDataChange;

};

#endif /* AppDataListeners_h */
