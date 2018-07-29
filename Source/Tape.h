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
				public Slider::Listener
{
public:
    Tape();
    ~Tape();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button* button);
	void sliderValueChanged(Slider* slider);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tape)

	Counter counter;
	Control control;
	AudioBuffer<float> buffer;

};
