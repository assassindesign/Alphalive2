//
//  TreeManager.hpp
//  AlphaLearn
//
//  Created by Luke Saxton on 24/02/2016.
//
//

#ifndef TreeManager_hpp
#define TreeManager_hpp

#include "../JuceLibraryCode/JuceHeader.h"
//#include "MainAppComponent.hpp"

namespace Identifiers
{
    const Identifier AppDataType("AppData");
}

class TreeManager
{

public:
    
    TreeManager();
    ~TreeManager();
    
    
    
    
private:
//    class IncomingMessageCallback  : public CallbackMessage
//    {
//    public:
//        IncomingMessageCallback (MainAppComponent* o, const ValueTree t, const String p, const var v)
//        : owner (o), tree(t), property(p), value(v)
//        {}
//        
//        void messageCallback() override
//        {
//            if (owner != nullptr)
//            {
//                //owner->addMessageToList (message, source); Do own stuff here.
//            }
//        }
//        
//        Component::SafePointer<MainAppComponent> owner;
//        ValueTree tree;
//        String property;
//        var value;
//            
//    };
    
    ScopedPointer<ValueTree> appData;
    
};

#endif /* TreeManager_hpp */
