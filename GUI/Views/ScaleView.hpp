//
//  ScaleView.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 11/07/2016.
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

#ifndef ScaleView_hpp
#define ScaleView_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "AlphaLive2Engine.hpp"
#include "NeonButton.hpp"

/* 
    Ancient implementation of scale moving and shifting UI, should not be used 
    for anything other than debugging. 
 */

class ScaleView : public Button::Listener,
                  public AppDataListener,
                  public Component
                  
{
public:
    
    
    ScaleView();
    ~ScaleView();
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    void scaleDataChangeCallback(const int changedData) override;
    
    void buttonClicked (Button*) override;
private:
    NeonButton upButton, downButton, octPlusButton, octMinusButton;
    Label keyLabel;
    
    ScopedPointer<StringArray> labelStrings;
    
    ScaleData* scaleData;
    //const char*
    int currentKey, currentOctave, currentScale;
    
    ComboBox scaleSelectCB, rowConfigCB;


};

#endif /* ScaleView_hpp */
