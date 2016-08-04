//
//  PadInspectorPanelBase.h
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
//
//

#ifndef PadInspectorPanelBase_h
#define PadInspectorPanelBase_h

#include "../JuceLibraryCode/JuceHeader.h"

class PadInspectorPanelBase : public Component,
                              public AppDataListener
{
public:
    PadInspectorPanelBase(){};
    PadInspectorPanelBase(AppDataFormat* _dataObject)
    {
        setDataObject(_dataObject);
    }
    ~PadInspectorPanelBase(){};
    void setDataObject(AppDataFormat* newObject = nullptr)
    {
        if (newObject != nullptr)
        {
            if (dataObject != nullptr)
            {
                dataObject->removeListener(this);
            }
            dataObject = newObject;
            dataObject->addListener(this);
            refreshData();
        }
    }
    
    virtual void refreshData() = 0;
    
    AppDataFormat* getDataObject()
    {
        return dataObject;
    }
private:
    AppDataFormat* dataObject = nullptr;
};


#endif /* PadInspectorPanelBase_h */
