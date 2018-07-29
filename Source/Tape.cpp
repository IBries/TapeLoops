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
	formatManager.registerBasicFormats();
	transportSource.addChangeListener(this);

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
	if (button == &control.loadButton)
	{
		//TODO load an audio file
	}
	else if (button == &control.playButton)
	{
		//TODO play the audio file
	}
	else if (button == &control.stopButton)
	{
		//TODO stop playing the audio file
	}
}

void Tape::sliderValueChanged(Slider* slider)
{
	if (slider == &control.volumeSlider)
	{
		//TODO change the volume of this tape
	}
	else if (slider == &control.startPointSlider)
	{
		//TODO change the loop start point
	}
	else if(slider == &control.endPointSlider)
	{
		//TODO change the loop end point
	}
}

void Tape::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &transportSource)
	{
		if (transportSource.isPlaying())
			changeState(Playing);
		else
			changeState(Stopped);
	}
}

void Tape::changeState(TransportState newState)
{
	if (state != newState)
	{
		state = newState;

		switch (state)
		{
			case Stopped:
				control.stopButton.setEnabled(false);
				control.playButton.setEnabled(true);
				transportSource.setPosition(0.0);
				break;

			case Starting:
				control.playButton.setEnabled(false);
				transportSource.start();
				break;

			case Playing:
				control.stopButton.setEnabled(true);
				break;

			case Stopping:
				transportSource.stop();
				break;
		}
	}
}
