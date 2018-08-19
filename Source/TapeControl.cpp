/*
  ==============================================================================

    TapeControl.cpp
    Created: 19 Aug 2018 12:21:05am
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TapeControl.h"
#include "Dimensions.h"

//==============================================================================
TapeControl::TapeControl()
{
	addAndMakeVisible(&openButton);
	openButton.setButtonText("Open...");

	addAndMakeVisible(&playButton);
	playButton.setButtonText("Play");
	playButton.setColour(TextButton::buttonColourId, Colour(0, 83, 5));
	playButton.setEnabled(false);

	addAndMakeVisible(&stopButton);
	stopButton.setButtonText("Stop");
	stopButton.setColour(TextButton::buttonColourId, Colour(105, 3, 0));
	stopButton.setEnabled(false);

	addAndMakeVisible(&clearButton);
	clearButton.setButtonText("Clear");
	clearButton.setEnabled(false);

	addAndMakeVisible(&startSampleSlider);
	startSampleSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	startSampleSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

	addAndMakeVisible(&endSampleSlider);
	endSampleSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	endSampleSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
}

TapeControl::~TapeControl()
{
}

void TapeControl::paint (Graphics& g)
{
}

void TapeControl::resized()
{
	auto area = getLocalBounds();
	auto buttonHeight = (area.getHeight() - (2 * Dimensions::BORDER)) / 3;

	auto topRow = area.removeFromTop(buttonHeight);
	auto buttonWidth = (topRow.getWidth() - Dimensions::BORDER) / 2;
	startSampleSlider.setBounds(topRow.removeFromLeft(buttonWidth));
	topRow.removeFromLeft(Dimensions::BORDER);
	endSampleSlider.setBounds(topRow);

	area.removeFromTop(Dimensions::BORDER);

	auto midRow = area.removeFromTop(buttonHeight);
	openButton.setBounds(midRow.removeFromLeft(buttonWidth));
	midRow.removeFromLeft(Dimensions::BORDER);
	clearButton.setBounds(midRow);

	area.removeFromTop(Dimensions::BORDER);

	playButton.setBounds(area.removeFromLeft(buttonWidth));
	area.removeFromLeft(Dimensions::BORDER);
	stopButton.setBounds(area);
}

//==============================================================================

void TapeControl::setButtonState(String button, bool state)
{
	if (button.equalsIgnoreCase("Open"))
		openButton.setEnabled(state);

	else if (button.equalsIgnoreCase("Play"))
		playButton.setEnabled(state);
	
	else if (button.equalsIgnoreCase("Stop"))
		stopButton.setEnabled(state);
	
	else if (button.equalsIgnoreCase("Clear"))
		clearButton.setEnabled(state);
}

void TapeControl::setSliderState(String slider, bool state)
{
	if (slider.equalsIgnoreCase("Start Sample"))
		startSampleSlider.setEnabled(state);

	else if (slider.equalsIgnoreCase("End Sample"))
		endSampleSlider.setEnabled(state);
}

void TapeControl::setSliderRange(String slider, int min, int max)
{
	if (slider.equalsIgnoreCase("Start Sample"))
		startSampleSlider.setRange(min, max);

	else if (slider.equalsIgnoreCase("End Sample"))
		endSampleSlider.setRange(min, max);
}

void TapeControl::setSliderValue(String slider, int val)
{
	if (slider.equalsIgnoreCase("Start Sample"))
		startSampleSlider.setValue(val);

	else if (slider.equalsIgnoreCase("End Sample"))
		endSampleSlider.setValue(val);
}

//==============================================================================

int TapeControl::getStartSample()
{
	return (int) startSampleSlider.getValue();
}

int TapeControl::getEndSample()
{
	return (int) endSampleSlider.getValue();
}

Button* TapeControl::getButtonPtr(String button)
{
	if (button.equalsIgnoreCase("Open"))
		return &openButton;

	else if (button.equalsIgnoreCase("Play"))
		return &playButton;

	else if (button.equalsIgnoreCase("Stop"))
		return &stopButton;

	else if (button.equalsIgnoreCase("Clear"))
		return &clearButton;

	else
		return nullptr;
}

Slider * TapeControl::getSliderPtr(String slider)
{
	if (slider.equalsIgnoreCase("Start Sample"))
		return &startSampleSlider;

	else if (slider.equalsIgnoreCase("End Sample"))
		return &endSampleSlider;

	else
		return nullptr;
}
