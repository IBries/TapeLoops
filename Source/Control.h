/*
  ==============================================================================

    Control.h
    Created: 26 Jul 2018 11:22:51pm
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Control    : public Component
{
public:
    Control();
    ~Control();

    void paint (Graphics&) override;
    void resized() override;
	void connectControls(Button::Listener* newButtonListener, Slider::Listener* newSliderListener);

	TextButton loadButton;
	Slider volumeSlider;
	TextButton playButton;
	TextButton stopButton;
	Slider startPointSlider;
	Slider endPointSlider;

private:


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Control)
};
