/*
  ==============================================================================

    TapeLooper.cpp
    Created: 26 Jul 2018 11:22:33pm
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TapeLooper.h"

//==============================================================================
TapeLooper::TapeLooper()
{
	tapes = new Tape[NUM_TAPES];
	for (int i = 0; i < NUM_TAPES; i++)
	{
		addAndMakeVisible(&tapes[i]);
	}
}

TapeLooper::~TapeLooper()
{
}

void TapeLooper::paint (Graphics& g)
{
}

void TapeLooper::resized()
{
	for (int i = 0; i < NUM_TAPES; i++)
	{
		tapes[i].setBounds(getLocalBounds());
	}
}

int TapeLooper::getNumTapes()
{
	return NUM_TAPES;
}
