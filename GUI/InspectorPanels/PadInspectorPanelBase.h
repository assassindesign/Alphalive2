//
//  PadInspectorPanelBase.h
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
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

#ifndef PadInspectorPanelBase_h
#define PadInspectorPanelBase_h

#include "../JuceLibraryCode/JuceHeader.h"

class PadInspectorPanelBase : public Component,
                              public AppDataListener
{
public:
    PadInspectorPanelBase()
    {
        setLookAndFeel(AppData::Instance()->getAlphaliveLookAndFeel());
    };
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
