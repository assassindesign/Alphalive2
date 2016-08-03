//
//  AudioIPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
//
//

#ifndef AudioIPanel_hpp
#define AudioIPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"

class AudioIPanel : public AppDataListener
{
public:
    AudioIPanel()
    {
        
    }
    ~AudioIPanel(){}
    void resized(){}
    void paint(Graphics& g)
    {
        g.fillAll(Colours::red);
    }
private:
};

#endif /* AudioIPanel_hpp */
