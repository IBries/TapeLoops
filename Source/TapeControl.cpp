/****************************//**
*	@file TapeControl.cpp
*	@author Isaac Bries
*	@date Created:	8/19/2018
*	@date Edited:	8/19/2018
********************************/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TapeControl.h"
#include "Dimensions.h"

/****************************//**
 *	@brief Default Constructor
 *******************************/
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
	startSampleSlider.setEnabled(false);

	addAndMakeVisible(&endSampleSlider);
	endSampleSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	endSampleSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	endSampleSlider.setEnabled(false);

	addAndMakeVisible(&volumeSlider);
	volumeSlider.setRange(0.0, 1.0);
	volumeSlider.setSkewFactorFromMidPoint(0.3);
	volumeSlider.setValue(volumeSlider.getMaximum());
	volumeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	volumeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
}

/************************//**
 *	@brief	Deconstructor
 ***************************/
TapeControl::~TapeControl()
{
}

/********************************************************//**
 *	@brief Draws both start and end point indicators and
 *		   darkens the area between them.
 *
 *	@param[in] &g
 *			   the graphics context used to do the
 *			   drawing operations
 *
 *	@return	void
 ***********************************************************/
void TapeControl::paint (Graphics& g)
{
}


/************************************************************//**
 *	@brief Called when this component's size has been changed.
 *		   Used for laying out any child components.
 *
 *	@return void
 ***************************************************************/
void TapeControl::resized()
{
	auto area = getLocalBounds();
	auto buttonHeight = (area.getHeight() - (3 * Dimensions::BORDER)) / 4;


	volumeSlider.setBounds(area.removeFromRight(Dimensions::VOLUME_SLIDER_WIDTH));

	auto row0 = area.removeFromTop(buttonHeight);
	auto buttonWidth = (row0.getWidth() - Dimensions::BORDER) / 2;
	startSampleSlider.setBounds(row0.removeFromLeft(buttonWidth));
	row0.removeFromLeft(Dimensions::BORDER);
	endSampleSlider.setBounds(row0);

	area.removeFromTop(Dimensions::BORDER);

	auto row1 = area.removeFromTop(buttonHeight);
	openButton.setBounds(row1.removeFromLeft(buttonWidth));
	row1.removeFromLeft(Dimensions::BORDER);
	clearButton.setBounds(row1);

	area.removeFromTop(Dimensions::BORDER);

	auto row2 = area.removeFromTop(buttonHeight);
	playButton.setBounds(row2.removeFromLeft(buttonWidth));
	row2.removeFromLeft(Dimensions::BORDER);
	stopButton.setBounds(row2);
}

/********************************************************//**
 *	@brief Sets the enable state of a button
 *
 *	@param[in] button
 *			   the button whose state will be changed
 *	@param[in] state
 *			   the state to be set (enable/disable)
 *
 *	@return void
 ***********************************************************/
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

/************************************************//**
 *	@brief Sets the enable state of a slider
 *
 *	@param[in] slider
 *			   the button to modify
 *	@param[in] state
 *			   the state to be set (enable/disable)
 *
 *	@return void
 ***************************************************/
void TapeControl::setSliderState(String slider, bool state)
{
	if (slider.equalsIgnoreCase("Start Sample"))
		startSampleSlider.setEnabled(state);

	else if (slider.equalsIgnoreCase("End Sample"))
		endSampleSlider.setEnabled(state);
}

/****************************************************//**
 *	@brief Sets the range of values a slider can take
 *
 *	@param[in] slider
 *			   the slider to modify
 *	@param[in] min
 *			   the minimum value slider can take
 *	@param[in] max
 *			   the maximum value slider can take
 *
 *	@return void
 *******************************************************/
void TapeControl::setSliderRange(String slider, int min, int max)
{
	if (slider.equalsIgnoreCase("Start Sample"))
		startSampleSlider.setRange(min, max);

	else if (slider.equalsIgnoreCase("End Sample"))
		endSampleSlider.setRange(min, max);
}

/************************************************//**
 *	@brief Directly set a slider to a given value
 *
 *	@param[in] slider
 *			   the slider to modify
 *	@param[in] val
 *			   the desired slider value
 *
 *	@return void
 ***************************************************/
void TapeControl::setSliderValue(String slider, int val)
{
	if (slider.equalsIgnoreCase("Start Sample"))
		startSampleSlider.setValue(val);

	else if (slider.equalsIgnoreCase("End Sample"))
		endSampleSlider.setValue(val);
}

/********************************************************//**
 *	@brief Returns the current value of startSampleSlider
 *
 *	@return current value of startSampleSlider
 ***********************************************************/
int TapeControl::getStartSample()
{
	return (int) startSampleSlider.getValue();
}

/****************************************************//**
 *	@brief Returns the current value of endSampleSlider
 *
 *	@return current value of endSampleSlider
 *******************************************************/
int TapeControl::getEndSample()
{
	return (int) endSampleSlider.getValue();
}

/****************************************//**
 *	@brief	Returns a pointer to a button
 *
 *	@param[in] button
 *			   the button to retrieve
 *
 *	@return a pointer to a button
 *******************************************/
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

/****************************************//**
 *	@brief	Returns a pointer to a slider
 *
 *	@param[in] slider
 *			   the slider to retrieve
 *
 *	@return a pointer to a slider
 *******************************************/
Slider * TapeControl::getSliderPtr(String slider)
{
	if (slider.equalsIgnoreCase("Start Sample"))
		return &startSampleSlider;

	else if (slider.equalsIgnoreCase("End Sample"))
		return &endSampleSlider;

	else if (slider.equalsIgnoreCase("Volume"))
		return &volumeSlider;

	else
		return nullptr;
}
