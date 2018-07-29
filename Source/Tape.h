/*
  ==============================================================================

    Tape.h
    Created: 26 Jul 2018 11:21:26pm
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Counter.h"
#include "Control.h"

//==============================================================================
/*
*/
class Tape    : public Component,
				public Button::Listener,
				public Slider::Listener,
				public ChangeListener
{
public:
    Tape();
    ~Tape();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button* button);
	void sliderValueChanged(Slider* slider);
	void changeListenerCallback(ChangeBroadcaster* source);

private:
	enum TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};
	void changeState(TransportState newState);

	Counter counter;
	Control control;
	AudioFormatManager formatManager;
	std::unique_ptr<AudioFormatReaderSource> readerSource;
	AudioTransportSource transportSource;
	TransportState state;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tape)
};
