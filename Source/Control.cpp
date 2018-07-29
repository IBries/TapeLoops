/*
  ==============================================================================

    Control.cpp
    Created: 28 Jul 2018 8:39:01pm
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Control.h"

//==============================================================================
Control::Control()
{
	addAndMakeVisible(&loadButton);
	loadButton.setButtonText("Load");

	addAndMakeVisible(&playButton);
	playButton.setButtonText("Play");
	playButton.setColour(TextButton::buttonColourId, Colours::green);
	playButton.setEnabled(false);

	addAndMakeVisible(&stopButton);
	stopButton.setButtonText("Stop");
	stopButton.setColour(TextButton::buttonColourId, Colours::red);
	stopButton.setEnabled(false);

	addAndMakeVisible(&volumeSlider);
	volumeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	volumeSlider.setRange(0, 1);
	volumeSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

	addAndMakeVisible(startPointSlider);
	startPointSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	//TODO pass reference to audio buffer when loading a new audio file
	//startPointSlider.setRange(0, buffer.length, 1);
	startPointSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	
	addAndMakeVisible(endPointSlider);
	endPointSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	//TODO pass reference to audio buffer when loading a new audio file
	//endPointSlider.setRange(0, buffer.length, 1);
	endPointSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

}

Control::~Control()
{
}

void Control::paint (Graphics& g)
{
}

void Control::resized()
{
	volumeSlider.setBounds(0, 0, 100, 100);
	startPointSlider.setBounds(100, 0, 100, 100);
	endPointSlider.setBounds(200, 0, 100, 100);
	loadButton.setBounds(300, 0, 100, 100);
	playButton.setBounds(400, 0, 100, 100);
	stopButton.setBounds(500, 0, 100, 100);
}

void Control::connectControls(Button::Listener* newButtonListener, Slider::Listener* newSliderListener)
{
	loadButton.addListener(newButtonListener);
	playButton.addListener(newButtonListener);
	stopButton.addListener(newButtonListener);
	volumeSlider.addListener(newSliderListener);
	startPointSlider.addListener(newSliderListener);
	endPointSlider.addListener(newSliderListener);
}
