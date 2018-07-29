/*
  ==============================================================================

    Tape.cpp
    Created: 26 Jul 2018 11:21:26pm
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Tape.h"

//==============================================================================
Tape::Tape()
{
	addAndMakeVisible(&control);
	control.connectControls(this, this);
}

Tape::~Tape()
{
}

void Tape::paint (Graphics& g)
{
}

void Tape::resized()
{
	control.setBounds(getLocalBounds());
}

void Tape::buttonClicked(Button* button)
{

}

void Tape::sliderValueChanged(Slider* slider)
{

}
