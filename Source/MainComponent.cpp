/**//****************************
*	@file MainComponent.cpp
*	@author Isaac Bries
*	@date Created:	8/01/2018
*	@date Edited:	8/01/2018
********************************/

#include "MainComponent.h"
#include "Dimensions.h"

/****************************//**
 *	@brief Default Constructor
 *******************************/
MainComponent::MainComponent()
{
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks.add(new TapeDeck());
		addAndMakeVisible(tapeDecks[tape]);
	}

	addAndMakeVisible(&masterVolume);
	masterVolume.setRange(0.0, 1.0);
	masterVolume.setSkewFactorFromMidPoint(0.3);
	level = (float) masterVolume.getMaximum();
	previousLevel = level;
	masterVolume.setValue(level);
	masterVolume.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	masterVolume.setSliderStyle(Slider::SliderStyle::LinearVertical);
	masterVolume.addListener(this);

	setSize(800, 700);
	setAudioChannels(0, 2);
}

/************************//**
 *	@brief Deconstructor
 ***************************/
MainComponent::~MainComponent()
{
	shutdownAudio();
}

/************************************************************//**
 *	@brief Prepares this and any subcomponents to process audio
 *
 *	@param[in]	samplesPerBlockExpected
 *				the number of samples expected in each
 *				audio block
 *
 *	@param[in]	sampleRate
 *				sample rate of the system
 *
 *	@return void
 ***************************************************************/
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape]->prepareToPlay(samplesPerBlockExpected,sampleRate);
	}
}

/********************************************************//**
 *	@brief Audio callback
 *
 *	@param[out] &bufferToFill
 *				block to be sent to system audio processsor
 *
 *	@return void
 ***********************************************************/
void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	bufferToFill.clearActiveBufferRegion();

	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape]->getNextAudioBlock(bufferToFill);
	}

	if (previousLevel == level)
		bufferToFill.buffer->applyGain(level);
	else 
	{
		bufferToFill.buffer->applyGainRamp(0, bufferToFill.numSamples, previousLevel, level);
		previousLevel = level;
	}
}

/************************************************************//**
 *	@brief Releases any resources still held by subcomponents
 *
 *	@return void
 ***************************************************************/
void MainComponent::releaseResources()
{
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape]->releaseResources();
	}
}

/********************************************************//**
 *	@brief Draws this component
 *
 *	@param[in]	&g
 *				graphics context in which to do the drawing
 *
 *	@return void
 ***********************************************************/
void MainComponent::paint (Graphics& g)
{
	g.fillAll(Colour(86, 32, 1));
}

/************************************************//**
 *	@brief Defines locations of all subcomponents
 *
 *	@return void
 ***************************************************/
void MainComponent::resized()
{
	auto area = getLocalBounds();
	area.reduce(Dimensions::BORDER, Dimensions::BORDER);

	masterVolume.setBounds(area.removeFromLeft(Dimensions::MASTER_FADER_WIDTH));

	auto tapeHeight = (area.getHeight() - ((NUM_TAPES - 1) * Dimensions::BORDER)) / 3;
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape]->setBounds(area.removeFromTop(tapeHeight));
		if (tape != NUM_TAPES - 1)
			area.removeFromTop(Dimensions::BORDER);
	}
}

/************************************************//**
 *	@brief Slider::Listener callback
 *
 *	@param[in] *slider
 *			   the slider that made the callback
 *
 *	@return void
 ***************************************************/
void MainComponent::sliderValueChanged(Slider* slider)
{
	if (slider == &masterVolume)
		masterVolumeChanged();
}

/************************************//**
 *	@brief Master volume helper method
 *
 *	@return void
 ***************************************/
void MainComponent::masterVolumeChanged()
{
	level = (float) masterVolume.getValue();
}
