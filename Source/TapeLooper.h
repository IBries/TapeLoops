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

	int getNumTapes();
	Tape* tapes;

private:	
	static const int NUM_TAPES = 1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TapeLooper)
};
