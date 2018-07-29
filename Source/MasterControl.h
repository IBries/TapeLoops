/*
  ==============================================================================

    MasterControl.h
    Created: 26 Jul 2018 11:21:56pm
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MasterControl    : public Component
{
public:
    MasterControl();
    ~MasterControl();

    void paint (Graphics&) override;
    void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterControl)

	Slider masterVolume;
};
