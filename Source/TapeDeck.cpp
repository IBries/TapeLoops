/****************************//**
*	@file TapeDeck.cpp
*	@author Isaac Bries
*	@date Created: 8/08/2018
*	@date Edited: 8/08/2018
********************************/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TapeDeck.h"
#include "Dimensions.h"

/********************************
 *	@brief Default Constructor
 *******************************/
TapeDeck::TapeDeck() :
	state(Stopped),
	thumbnail(512, formatManager),
	Thread("Background Thread")
{
	addAndMakeVisible(&thumbnail);
	addAndMakeVisible(&positionOverlay);
	//addAndMakeVisible(&loopBounds);

	addAndMakeVisible(&controls);
	connectControls();

	level = (float)controls.getSliderPtr("Volume")->getMinimum();
	previousLevel = level;

	formatManager.registerBasicFormats();

	startThread();
}

/****************************
 *	@brief Deconstructor
 ***************************/
TapeDeck::~TapeDeck()
{
	stopThread(4000);
}

/****************************************************//**
 *	@brief Prepares this component for audio playback
 *
 *	@param[in] samplesPerBlockExpected
 *			   expected audio buffer block size
 *	@param[in] sampleRate
 *			   sample rate of the audio system
 *
 *	@return void
 *******************************************************/
void TapeDeck::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
}

/********************************************************//**
 *	@brief Fills the next buffer to send to audio system
 *
 *	@param[in] &bufferToFill
 *			   buffer to send to audio system
 *
 *	@return void
 ***********************************************************/
void TapeDeck::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	ReferenceCountedBuffer::Ptr retainedCurrentBuffer(currentBuffer);
	ReferenceCountedBuffer::Ptr retainedFadeBuffer(currentFadeBuffer);

	int position;

	if (retainedCurrentBuffer == nullptr)
	{
		position = controls.getStartSample();
		return;
	}
	else if (state != Playing)
	{
		position = controls.getStartSample();
		retainedCurrentBuffer->position = position;
		return;
	}
	else
	{
		auto* currentAudioSampleBuffer = retainedCurrentBuffer->getAudioSampleBuffer();
		auto* currentFadeSampleBuffer = retainedFadeBuffer->getAudioSampleBuffer();

		position = retainedCurrentBuffer->position;

		auto numInputChannels = currentAudioSampleBuffer->getNumChannels();
		auto numOutputChannels = bufferToFill.buffer->getNumChannels();

		auto outputSamplesRemaining = bufferToFill.numSamples;
		auto outputSamplesOffset = 0;

		while (outputSamplesRemaining > 0)
		{
			if (controls.getStartSample() >= position)
				position = controls.getStartSample();

			auto bufferSamplesRemaining = currentAudioSampleBuffer->getNumSamples() - position;
			auto samplesThisTime = jmin(outputSamplesRemaining, bufferSamplesRemaining);

			for (auto channel = 0; channel < numOutputChannels; ++channel)
			{
				if (nearEnd(position))
				{
					// read from buffer with fades
					bufferToFill.buffer->addFrom(channel,
						bufferToFill.startSample + outputSamplesOffset,
						*currentFadeSampleBuffer,
						channel % numInputChannels,
						position,
						samplesThisTime);
				}
				else
				{
					bufferToFill.buffer->addFrom(channel,
						bufferToFill.startSample + outputSamplesOffset,
						*currentAudioSampleBuffer,
						channel % numInputChannels,
						position,
						samplesThisTime);
				}
			}

			outputSamplesRemaining -= samplesThisTime;
			outputSamplesOffset += samplesThisTime;
			position += samplesThisTime;

			if (position >= controls.getEndSample())
				position = controls.getStartSample() + LOOP_FADE_LENGTH_IN_SAMPLES;
		}

		retainedCurrentBuffer->position = position;
		retainedFadeBuffer->position = position;
		positionOverlay.setPosition(position);
	}
}

/****************************************************************//**
 *	@brief Cleans up any mess when the audio system is shut down
 *
*	@return void
 *******************************************************************/
void TapeDeck::releaseResources()
{

}

/****************************************************************//**
 *	@brief Draws this component
 *
 *	@param[in] &g
 *			   the graphics context in which to draw this component
 *
 *	@return void
 *******************************************************************/
void TapeDeck::paint(Graphics& g)
{
}

/****************************************//**
 *	@brief Sets bounds of any subcomponents
 *
 *	@return void
 *******************************************/
void TapeDeck::resized()
{
	auto area = getLocalBounds();
	auto thumbnailArea = area.removeFromLeft((int)(area.getWidth() * 0.7));
	thumbnail.setBounds(thumbnailArea);
	positionOverlay.setBounds(thumbnailArea);
	loopBounds.setBounds(thumbnailArea);
	area.removeFromLeft(Dimensions::BORDER);
	controls.setBounds(area);
}

/********************************************//**
 *	@brief Button::Listener callback
 *
 *	@param[in] *button
 *			   button that invoked the callback
 *
 *	@return void
 ***********************************************/
void TapeDeck::buttonClicked(Button* button)
{
	if (button == controls.getButtonPtr("Open"))
		openButtonClicked();

	else if (button == controls.getButtonPtr("Play"))
		playButtonClicked();

	else if (button == controls.getButtonPtr("Stop"))
		stopButtonClicked();

	else if (button == controls.getButtonPtr("Clear"))
		clearButtonClicked();
}

/********************************************//**
 *	@brief Slider::Listener callback
 *
 *	@param[in] *slider
 *			   slider that invoked the callback
 *
 *	@return void
 ***********************************************/
void TapeDeck::sliderValueChanged(Slider* slider)
{
	if (slider == controls.getSliderPtr("Start Sample"))
		startSampleChanged();

	else if (slider == controls.getSliderPtr("End Sample"))
		endSampleChanged();

	else if (slider == controls.getSliderPtr("Volume"))
		volumeSliderChanged();
}

/****************************************************//**
 *	@brief Changes the playback state of this TapeDeck
 *
 *	@param[in] newState
 *			   new playback state of this TapeDeck
 *
 *	@return void
 *******************************************************/
void TapeDeck::changeState(TransportState newState)
{
	if (state != newState)
	{
		state = newState;

		switch (state)
		{
		case Stopped:
			if (currentBuffer == nullptr)
				controls.setButtonState("Play", false);
			else
				controls.setButtonState("Play", true);
			break;

		case Starting:
			controls.setButtonState("Play", false);
			changeState(Playing);
			break;

		case Playing:
			controls.setButtonState("Stop", true);
			break;

		case Stopping:
			controls.setButtonState("Stop", false);
			positionOverlay.setPosition(0);
			changeState(Stopped);
			break;
		}
	}
}

/********************************************//**
 *	@brief Clears the buffer held by TapeDeck
 *
 *	@return void
 ***********************************************/
void TapeDeck::clearCurrentBuffer()
{
	currentBuffer = nullptr;
}

/************************************************************//**
 *	@brief Clears the thumbnail drawing of the current buffer
 *
 *	@return void
 ***************************************************************/
void TapeDeck::clearThumbnail()
{
	thumbnail.setFile(File());
}

/****************************************//**
 *	@brief Clears the LoopBounds drawing
 *
 *	@return void
 *******************************************/
void TapeDeck::clearLoopBounds()
{
	loopBounds.clear();
	controls.setSliderValue("Start Sample", 0);
	controls.setSliderValue("End Sample", getWidth());
}

/********************************************************************//**
 *	@brief Starts a thread to handle file access and memory allocation
 *
 *	@return void
 ***********************************************************************/
void TapeDeck::run()
{
	while (!threadShouldExit())
	{
		checkForPathToOpen();
		checkForBuffersToFree();
		wait(500);
	}
}

/************************************************************************//**
 *	@brief Checks if any ReferenceCountedBuffer in buffers are not in use
 *
 *	@return void
 ***************************************************************************/
void TapeDeck::checkForBuffersToFree()
{
	for (auto i = buffers.size(); --i >= 0;)
	{
		ReferenceCountedBuffer::Ptr buffer(buffers.getUnchecked(i));

		if (buffer->getReferenceCount() == 2)
			buffers.remove(i);
	}

	for (auto i = fadeBuffers.size(); --i >= 0;)
	{
		ReferenceCountedBuffer::Ptr fadeBuffer(fadeBuffers.getUnchecked(i));

		if (fadeBuffer->getReferenceCount() == 2)
			fadeBuffers.remove(i);
	}
}

/****************************************************************//**
 *	@brief Checks if there is a file operation waiting to happen
 *
 *	@return void
 *******************************************************************/
void TapeDeck::checkForPathToOpen()
{
	String pathToOpen;
	pathToOpen.swapWith(chosenPath);

	if (pathToOpen.isNotEmpty())
	{
		File file(pathToOpen);
		std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));

		if (reader.get() != nullptr)
		{
			currentBuffer = nullptr;
			thumbnail.isLoading(true);

			ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(reader->numChannels, (int)reader->lengthInSamples);
			reader->read(newBuffer->getAudioSampleBuffer(), 0, (int)reader->lengthInSamples, 0, true, true);

			ReferenceCountedBuffer::Ptr newFadeBuffer = new ReferenceCountedBuffer(reader->numChannels, (int)reader->lengthInSamples);
			reader->read(newFadeBuffer->getAudioSampleBuffer(), 0, (int)reader->lengthInSamples, 0, true, (int)reader->lengthInSamples);

			currentBuffer = newBuffer;
			currentFadeBuffer = newFadeBuffer;

			buffers.add(newBuffer);
			fadeBuffers.add(newFadeBuffer);

			// Drawing Stuff
			int lengthInSamples = (int)reader->lengthInSamples;
			positionOverlay.setLengthInSamples(lengthInSamples);
			loopBounds.setMaxLength(lengthInSamples);

			const MessageManagerLock mmLock;

			controls.setButtonState("Play", true);
			controls.setButtonState("Clear", true);
			controls.setButtonState("Normalize", true);

			controls.setSliderRange("Start Sample", 0, lengthInSamples - 1);
			controls.setSliderValue("Start Sample", 0);
			controls.setSliderRange("End Sample", 1, lengthInSamples);
			controls.setSliderValue("End Sample", lengthInSamples);

			thumbnail.isLoading(false);
			thumbnail.setFile(file);

			changeState(Stopping);
		}
	}
}

/****************************************************************************//**
 *	@brief True if playback position is less than LOOP_FADE_LENGTH_IN_SAMPLES
 *		   samples past the beginning of the loop
 *
 *	@param[in] position
 *			   current playback position
 *
 *	@return true if close enough to beginning, false otherwise
 *******************************************************************************/
bool TapeDeck::nearBeginning(int position)
{
	if (position <= controls.getStartSample() + (LOOP_FADE_LENGTH_IN_SAMPLES / 2))
	{
		return true;
	}

	return false;
}

/****************************************************************************//**
 *	@brief True if playback position is less than LOOP_FADE_LENGTH_IN_SAMPLES
 *		   samples before the end of the loop
 *
 *	@param[in] position
 *			   current playback position
 *
 *	@return true if close enough to end, false otherwise
 *******************************************************************************/
bool TapeDeck::nearEnd(int position)
{
	if (position >= controls.getEndSample() - (LOOP_FADE_LENGTH_IN_SAMPLES / 2))
	{
		return true;
	}

	return false;
}

/****************************************************************//**
 *	@brief Creates Broadcaster/Listener relationships with controls
 *
 *	@return void
 *******************************************************************/
void TapeDeck::connectControls()
{
	controls.getButtonPtr("Play")->addListener(this);
	controls.getButtonPtr("Open")->addListener(this);
	controls.getButtonPtr("Stop")->addListener(this);
	controls.getButtonPtr("Clear")->addListener(this);
	controls.getSliderPtr("Start Sample")->addListener(this);
	controls.getSliderPtr("End Sample")->addListener(this);
	controls.getSliderPtr("Volume")->addListener(this);
}

/****************************//**
 *	@brief openButton callback
 *
 *	@return void
 *******************************/
void TapeDeck::openButtonClicked()
{
	FileChooser chooser("Select a Wave file to play...", File(), "*.wav");

	if (chooser.browseForFileToOpen())
	{
		auto file = chooser.getResult();
		auto path = file.getFullPathName();
		chosenPath.swapWith(path);
		notify();
	}
}

/****************************//**
 *	@brief playButton callback
 *
 *	@return void
 *******************************/
void TapeDeck::playButtonClicked()
{
	changeState(Starting);
}

/****************************//**
 *	@brief stopButton callback
 *
 *	@return void
 *******************************/
void TapeDeck::stopButtonClicked()
{
	changeState(Stopping);
}

/****************************//**
 *	@brief clearButton callback
 *
 *	@return void
 *******************************/
void TapeDeck::clearButtonClicked()
{
	changeState(Stopping);
	clearCurrentBuffer();
	clearThumbnail();
	clearLoopBounds();

	controls.setButtonState("Play", false);
	controls.setButtonState("Stop", false);
	controls.setButtonState("Clear", false);
	controls.setButtonState("Normalize", false);
}

/************************************//**
 *	@brief startSampleSlider callback
 *
 *	@return void
 ***************************************/
void TapeDeck::startSampleChanged()
{
	int startSample = loopBounds.getStartSample();
	int maxLength = loopBounds.getMaxLength();

	loopBounds.setStartSample(controls.getStartSample());

	if (startSample >= maxLength)
		controls.setSliderRange("End Sample", maxLength - 1, maxLength);
	else
		controls.setSliderRange("End Sample", startSample, maxLength);

	applyCrossFade();

	loopBounds.setStartDrawPosition(calculateDrawPosition(startSample, maxLength, loopBounds.getWidth()));
	loopBounds.repaint();
}

/********************************//**
 *	@brief endSampleSlider callback
 *
 *	@return void
 ***********************************/
void TapeDeck::endSampleChanged()
{
	int endSample = loopBounds.getEndSample();
	int maxLength = loopBounds.getMaxLength();

	if (endSample - 1 <= 0)
		controls.setSliderRange("Start Sample", 0, 1);
	else
		controls.setSliderRange("Start Sample", 0, endSample - 1);

	applyCrossFade();

	loopBounds.setEndDrawPosition(calculateDrawPosition(endSample, maxLength, loopBounds.getWidth()));
	loopBounds.repaint();
}

/********************************//**
 *	@brief volumeSlider callback
 *
 *	@return void
 ***********************************/
void TapeDeck::volumeSliderChanged()
{
	level = (float)(controls.getSliderPtr("Volume")->getValue());
}

/************************************************************************//**
 *	@brief Applies cross fade at loop boundary to remove discontinuities
 *
 *	@return void
 ***************************************************************************/
void TapeDeck::applyCrossFade()
{
	if (currentFadeBuffer != nullptr)
	{
		auto* bufferToFade = currentFadeBuffer->getAudioSampleBuffer();

		for (int channel = 0; channel < bufferToFade->getNumChannels(); channel++)
		{
			auto endSample = controls.getEndSample();

			if (endSample - LOOP_FADE_LENGTH_IN_SAMPLES > 0)
			{
				bufferToFade->applyGainRamp(channel, endSample - LOOP_FADE_LENGTH_IN_SAMPLES, LOOP_FADE_LENGTH_IN_SAMPLES, 1.0f, 0.0f);

				AudioBuffer<float> fadeIn(2, LOOP_FADE_LENGTH_IN_SAMPLES);
				fadeIn.copyFrom(channel, 0, *(currentBuffer->getAudioSampleBuffer()), channel, endSample - LOOP_FADE_LENGTH_IN_SAMPLES, LOOP_FADE_LENGTH_IN_SAMPLES);
				auto* fadeInPtr = fadeIn.getWritePointer(channel);

				bufferToFade->addFromWithRamp(channel, endSample - LOOP_FADE_LENGTH_IN_SAMPLES, fadeInPtr, LOOP_FADE_LENGTH_IN_SAMPLES, 0.0f, 1.0f);
			}
		}
	}
}

/****************************************************************//**
 *	@brief Calculates where to draw position indicator
 *
 *	@param[in] sample
 *			   current playback sample
 *	@param[in] lengthInSamples
 *			   length of current audio buffer, in samples
 *	@param[in] width
 *			   width of thumbnail, in pixels
 *
 *	@return draw position as ratio of position to component size
 *******************************************************************/
float TapeDeck::calculateDrawPosition(int sample, int lengthInSamples, int width)
{
	return (float)sample / (float)lengthInSamples * (float)width;
}
