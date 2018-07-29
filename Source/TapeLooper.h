/*
  ==============================================================================

    TapeLooper.h
    Created: 26 Jul 2018 11:22:33pm
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Tape.h"
#include "MasterControl.h"

//==============================================================================
/*
*/
class TapeLooper    : public Component
{
public:
    TapeLooper();
    ~TapeLooper();

    void paint (Graphics&) override;
    void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TapeLooper)
		
	static const int NUM_TAPES = 1;
	Tape* tapes;
	MasterControl globalControls;
};
