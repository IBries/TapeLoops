/*
  ==============================================================================

    TapeDeck.cpp
    Created: 8 Aug 2018 9:50:34pm
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TapeDeck.h"

//==============================================================================
TapeDeck::TapeDeck() :
	state(Stopped),
	thumbnail(512, formatManager),
	loopBounds(startSampleSlider, endSampleSlider),
	Thread("Background Thread")
{
	addAndMakeVisible(openButton);
	openButton.setButtonText("Open...");
	openButton.onClick = [this] { openButtonClicked(); };

	addAndMakeVisible(playButton);
	playButton.setButtonText("Play");
	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setColour(TextButton::buttonColourId, Colour(0, 83, 5));
	playButton.setEnabled(false);

	addAndMakeVisible(stopButton);
	stopButton.setButtonText("Stop");
	stopButton.onClick = [this] { stopButtonClicked(); };
	stopButton.setColour(TextButton::buttonColourId, Colour(105, 3, 0));
	stopButton.setEnabled(false);

	addAndMakeVisible(clearButton);
	clearButton.setButtonText("Clear");
	clearButton.onClick = [this] { clearButtonClicked(); };
	clearButton.setEnabled(false);

	addAndMakeVisible(startSampleSlider);
	startSampleSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	startSampleSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	startSampleSlider.addListener(&loopBounds);
	startSampleSlider.addListener(this);

	addAndMakeVisible(endSampleSlider);
	endSampleSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	endSampleSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	endSampleSlider.addListener(&loopBounds);
	endSampleSlider.addListener(this);

	addAndMakeVisible(&thumbnail);
	addAndMakeVisible(&positionOverlay);
	addAndMakeVisible(&loopBounds);

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
		position = (int) startSampleSlider.getValue();
		return;
	}
	else if (state != Playing)
	{
		position = (int) startSampleSlider.getValue();
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
			if (startSampleSlider.getValue() >= position)
				position = (int) startSampleSlider.getValue();

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

			if (position >= endSampleSlider.getValue())
				position = (int) startSampleSlider.getValue() + loopFadeLengthInSamples;
		}

		retainedCurrentBuffer->position = position;
		retainedFadeBuffer->position = position;
		positionOverlay.setPosition(position);
	}
}


bool TapeDeck::nearBeginning(int position)
{
	if (position <= startSampleSlider.getValue() + (loopFadeLengthInSamples / 2))
	{
		return true;
	}

	return false;
}

bool TapeDeck::nearEnd(int position)
{
	if (position >= endSampleSlider.getValue() - (loopFadeLengthInSamples / 2))
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
	g.fillAll(Colour(62, 29, 0));
}

void TapeDeck::resized()
{
	openButton.setBounds(3 * BORDER + getThumbnailWidth() + BUTTON_WIDTH, BORDER, BUTTON_WIDTH, BUTTON_HEIGHT);
	playButton.setBounds(2 * BORDER + getThumbnailWidth(), BORDER, BUTTON_WIDTH, BUTTON_HEIGHT);
	stopButton.setBounds(2 * BORDER + getThumbnailWidth(), 2 * BORDER + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT);
	clearButton.setBounds(3 * BORDER + getThumbnailWidth() + BUTTON_WIDTH, 2 * BORDER + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT);
	startSampleSlider.setBounds(2 * BORDER + getThumbnailWidth(), 3 * BORDER + 2 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT);
	endSampleSlider.setBounds(3 * BORDER + getThumbnailWidth() + BUTTON_WIDTH, 3 * BORDER + 2 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT);

	Rectangle<int> thumbnailBounds(BORDER, BORDER, getWidth() - 4 * BORDER - 2 * BUTTON_WIDTH, getHeight() - 2 * BORDER);
	thumbnail.setBounds(thumbnailBounds);
	loopBounds.setBounds(thumbnailBounds);
	positionOverlay.setBounds(thumbnailBounds);
}

int TapeDeck::getThumbnailWidth()
{
	return getWidth() - 4 * BORDER - 2 * BUTTON_WIDTH;
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
				playButton.setEnabled(false);
			else
				playButton.setEnabled(true);
			break;

		case Starting:
			playButton.setEnabled(false);
			changeState(Playing);
			break;

		case Playing:
			stopButton.setEnabled(true);
			break;

		case Stopping:
			stopButton.setEnabled(false);
			positionOverlay.setPosition(0);
			changeState(Stopped);
			break;
		}
	}
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
			playButton.setEnabled(true);
			clearButton.setEnabled(true);
			startSampleSlider.setRange(0, lengthInSamples - 1);
			endSampleSlider.setRange(1, lengthInSamples);
			startSampleSlider.setValue(1);
			startSampleSlider.setValue(0);
			endSampleSlider.setValue(lengthInSamples);
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

void TapeDeck::sliderValueChanged(Slider* slider)
{
	if ((slider == &startSampleSlider || slider == &endSampleSlider) && currentFadeBuffer != nullptr)
	{
		for (int channel = 0; channel < currentFadeBuffer->getAudioSampleBuffer()->getNumChannels(); channel++)
		{
			if (endSampleSlider.getValue() - loopFadeLengthInSamples > 0)
			{
				auto* bufferToFade = currentFadeBuffer->getAudioSampleBuffer();

				bufferToFade->applyGainRamp(channel, (int) endSampleSlider.getValue() - loopFadeLengthInSamples, loopFadeLengthInSamples, 1.0f, 0.0f);

				AudioBuffer<float> fadeIn(2, loopFadeLengthInSamples);
				fadeIn.copyFrom(channel, 0, *(currentBuffer->getAudioSampleBuffer()), channel, (int)endSampleSlider.getValue() - loopFadeLengthInSamples, loopFadeLengthInSamples);
				auto* fadeInPtr = fadeIn.getWritePointer(channel);

				bufferToFade->addFromWithRamp(channel, (int) endSampleSlider.getValue() - loopFadeLengthInSamples, fadeInPtr, loopFadeLengthInSamples, 0.0f, 1.0f);
			}
		}
	}
}

//==============================================================================

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

//==============================================================================

void TapeDeck::playButtonClicked()
{
	changeState(Starting);
}

//==============================================================================

void TapeDeck::stopButtonClicked()
{
	changeState(Stopping);
}

//==============================================================================

void TapeDeck::clearButtonClicked()
{
	changeState(Stopping);
	playButton.setEnabled(false);
	stopButton.setEnabled(false);
	clearButton.setEnabled(false);
	currentBuffer = nullptr;
	thumbnail.setFile(File());
	loopBounds.clear();
}