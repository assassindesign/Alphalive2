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

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "Alphalive2Engine.hpp"
#include "SphereView.hpp"
#include "TempoView.hpp"
#include "ScaleView.hpp"
#include "PadInspector.hpp"
#include "GUIStyle.h"
#include "AlphaSphereConnectedButton.hpp"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
public Button::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked (Button*) override;
    
    PadInspector* getPadInspector();
    
private:
    ScopedPointer<AppData> appData;
    ScopedPointer<Alphalive2Engine> alphalive2Engine;
    ScopedPointer<SphereView> sphereView;
    ScopedPointer<AlphaSphereConnectedButton> connectionStatus;
    ScopedPointer<TempoView> tempoView;
    ScopedPointer<ScaleView> scaleView;
    ScopedPointer<PadInspector> padInspector;
    ScopedPointer<TextButton> killButton;
    ScopedPointer<AudioMeterButton> audioMeter;
    

    ScopedPointer<TooltipWindow> tooltip;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
