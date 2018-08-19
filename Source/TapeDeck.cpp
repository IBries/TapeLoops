/*
  ==============================================================================

    TapeDeck.cpp
    Created: 8 Aug 2018 9:50:34pm
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TapeDeck.h"
#include "Dimensions.h"

//==============================================================================
TapeDeck::TapeDeck() :
	state(Stopped),
	thumbnail(512, formatManager),
	Thread("Background Thread")
{
	addAndMakeVisible(&thumbnail);
	addAndMakeVisible(&positionOverlay);
	addAndMakeVisible(&loopBounds);

	addAndMakeVisible(&controls);

	formatManager.registerBasicFormats();

	startThread();
}

TapeDeck::~TapeDeck()
{
	stopThread(4000);
}

void TapeDeck::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
}

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


bool TapeDeck::nearBeginning(int position)
{
	if (position <= controls.getStartSample() + (LOOP_FADE_LENGTH_IN_SAMPLES / 2))
	{
		return true;
	}

	return false;
}

bool TapeDeck::nearEnd(int position)
{
	if (position >= controls.getEndSample() - (LOOP_FADE_LENGTH_IN_SAMPLES / 2))
	{
		return true;
	}

	return false;
}

void TapeDeck::releaseResources()
{

}

void TapeDeck::paint (Graphics& g)
{
}

void TapeDeck::resized()
{
//TODO dynamically draw child components
}

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

void TapeDeck::playButtonClicked()
{
	changeState(Starting);
}

void TapeDeck::stopButtonClicked()
{
	changeState(Stopping);
}

void TapeDeck::clearButtonClicked()
{
	changeState(Stopping);
	clearCurrentBuffer();
	clearThumbnail();
	clearLoopBounds();

	controls.setButtonState("Play", false);
	controls.setButtonState("Stop", false);
	controls.setButtonState("Clear", false);
}

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

float TapeDeck::calculateDrawPosition(int sample, int lengthInSamples, int width)
{
	return (float)sample / (float)lengthInSamples * (float)width;
}

void TapeDeck::sliderValueChanged(Slider* slider)
{
	if (slider == controls.getSliderPtr("Start Sample"))
		startSampleChanged();

	else if (slider == controls.getSliderPtr("End Sample"))
		endSampleChanged();
}

int TapeDeck::getThumbnailWidth()
{
	return getWidth() - 4 * Dimensions::BORDER - 2 * Dimensions::BUTTON_WIDTH;
}

//==============================================================================

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

void TapeDeck::clearCurrentBuffer()
{
	currentBuffer = nullptr;
}

void TapeDeck::clearThumbnail()
{
	thumbnail.setFile(File());
}

void TapeDeck::clearLoopBounds()
{
	loopBounds.clear();
	controls.setSliderValue("Start Sample", 0);
	controls.setSliderValue("End Sample", getWidth());
}

//==============================================================================

void TapeDeck::run()
{
	while (!threadShouldExit())
	{
		checkForPathToOpen();
		checkForBuffersToFree();
		wait(500);
	}
}

//==============================================================================

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

//==============================================================================

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
