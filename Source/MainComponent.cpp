/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent() :
	state(Stopped),
	thumbnailCache(5),
	thumbnail(512, formatManager, thumbnailCache),
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

	setSize(800, 340);

	formatManager.registerBasicFormats();

	setAudioChannels(0, 2);
	startThread();
}

MainComponent::~MainComponent()
{
	stopThread(4000);
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
}

//==============================================================================

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	ReferenceCountedBuffer::Ptr retainedCurrentBuffer(currentBuffer);
	ReferenceCountedBuffer::Ptr retainedFadeBuffer(currentFadeBuffer);

	int position;

	if (state != Playing && retainedCurrentBuffer != nullptr)
	{
		position = startSampleSlider.getValue();
		retainedCurrentBuffer->position = position;
		bufferToFill.clearActiveBufferRegion();
		return;
	}
	else if (retainedCurrentBuffer == nullptr)
	{
		position = startSampleSlider.getValue();
		bufferToFill.clearActiveBufferRegion();
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
				position = startSampleSlider.getValue();

			auto bufferSamplesRemaining = currentAudioSampleBuffer->getNumSamples() - position;
			auto samplesThisTime = jmin(outputSamplesRemaining, bufferSamplesRemaining);

			for (auto channel = 0; channel < numOutputChannels; ++channel)
			{
				if (nearEnd(position))
				{
					// read from buffer with fades
					bufferToFill.buffer->copyFrom(channel,
						bufferToFill.startSample + outputSamplesOffset,
						*currentFadeSampleBuffer,
						channel % numInputChannels,
						position,
						samplesThisTime);
				}
				else
				{
					bufferToFill.buffer->copyFrom(channel,
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
				position = startSampleSlider.getValue() + loopFadeLengthInSamples;
		}

		retainedCurrentBuffer->position = position;
		retainedFadeBuffer->position = position;
		positionOverlay.setPosition(position);
	}
}

bool MainComponent::nearBeginning(int position)
{
	if (position <= startSampleSlider.getValue() + (loopFadeLengthInSamples / 2))
	{
		return true;
	}

	return false;
}

bool MainComponent::nearEnd(int position)
{
	if (position >= endSampleSlider.getValue() - (loopFadeLengthInSamples / 2))
	{
		return true;
	}

	return false;
}

//==============================================================================

void MainComponent::releaseResources()
{
}

//==============================================================================

void MainComponent::paint (Graphics& g)
{
	g.fillAll(Colour(62, 29, 0));
}

//==============================================================================

void MainComponent::resized()
{
	openButton.setBounds(3 * border + getThumbnailWidth() + buttonWidth, border, buttonWidth, buttonWidth);
	playButton.setBounds(2 * border + getThumbnailWidth(), border, buttonWidth, buttonWidth);
	stopButton.setBounds(2 * border + getThumbnailWidth(), 2*border + buttonWidth, buttonWidth, buttonWidth);
	clearButton.setBounds(3 * border + getThumbnailWidth() + buttonWidth, 2*border + buttonWidth, buttonWidth, buttonWidth);
	startSampleSlider.setBounds(2 * border + getThumbnailWidth(), 3 * border + 2 * buttonWidth, buttonWidth, buttonWidth);
	endSampleSlider.setBounds(3 * border + getThumbnailWidth() + buttonWidth, 3 * border + 2 * buttonWidth, buttonWidth, buttonWidth);

	Rectangle<int> thumbnailBounds(border, border, getWidth() - 4 * border - 2 * buttonWidth, getHeight() - 2 * border);
	thumbnail.setBounds(thumbnailBounds);
	loopBounds.setBounds(thumbnailBounds);
	positionOverlay.setBounds(thumbnailBounds);
}

int MainComponent::getThumbnailWidth()
{
	return getWidth() - 4 * border - 2 * buttonWidth;
}

//==============================================================================

void MainComponent::changeState(TransportState newState)
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

void MainComponent::run()
{
	while (!threadShouldExit())
	{
		checkForPathToOpen();
		checkForBuffersToFree();
		wait(500);
	}
}

//==============================================================================

void MainComponent::checkForBuffersToFree()
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

//==============================================================================

void MainComponent::checkForPathToOpen()
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
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader.get(), true));
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
			readerSource.reset(newSource.release());

			changeState(Stopping);
		}
	}
}

void MainComponent::sliderValueChanged(Slider* slider)
{
	if (slider == &startSampleSlider || slider == &endSampleSlider)
	{
		for (int channel = 0; channel < currentFadeBuffer->getAudioSampleBuffer()->getNumChannels(); channel++)
		{
			auto* bufferToFade = currentFadeBuffer->getAudioSampleBuffer();

			bufferToFade->applyGainRamp(endSampleSlider.getValue() - loopFadeLengthInSamples, loopFadeLengthInSamples, 1.0f, 0.0f);

			for (int channel = 0; channel < bufferToFade->getNumChannels(); channel++)
			{
				AudioBuffer<float> fadeIn(2, loopFadeLengthInSamples);
				fadeIn.copyFrom(channel, 0, *(currentBuffer->getAudioSampleBuffer()), channel, (int)endSampleSlider.getValue() - loopFadeLengthInSamples, loopFadeLengthInSamples);
				auto* fadeInPtr = fadeIn.getWritePointer(channel);

				bufferToFade->addFromWithRamp(channel, endSampleSlider.getValue() - loopFadeLengthInSamples, fadeInPtr, loopFadeLengthInSamples, 0.0f, 1.0f);
			}
		}
	}
}

//==============================================================================

void MainComponent::openButtonClicked()
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

void MainComponent::playButtonClicked()
{
	changeState(Starting);
}

//==============================================================================

void MainComponent::stopButtonClicked()
{
	changeState(Stopping);
}

//==============================================================================

void MainComponent::clearButtonClicked()
{
	changeState(Stopping);
	playButton.setEnabled(false);
	stopButton.setEnabled(false);
	clearButton.setEnabled(false);
	currentBuffer = nullptr;
	thumbnail.setFile(File());
	loopBounds.clear();
}