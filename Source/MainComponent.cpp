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

	addAndMakeVisible(&thumbnail);
	addAndMakeVisible(&positionOverlay);

	setSize(800, 230);

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
	int position;

	if (state != Playing && retainedCurrentBuffer != nullptr)
	{
		position = 0;
		retainedCurrentBuffer->position = position;
		bufferToFill.clearActiveBufferRegion();
		return;
	}
	else if (retainedCurrentBuffer == nullptr)
	{
		position = 0;
		bufferToFill.clearActiveBufferRegion();
		return;
	}
	else
	{
		auto* currentAudioSampleBuffer = retainedCurrentBuffer->getAudioSampleBuffer();
		position = retainedCurrentBuffer->position;

		auto numInputChannels = currentAudioSampleBuffer->getNumChannels();
		auto numOutputChannels = bufferToFill.buffer->getNumChannels();

		auto outputSamplesRemaining = bufferToFill.numSamples;
		auto outputSamplesOffset = 0;

		while (outputSamplesRemaining > 0)
		{
			auto bufferSamplesRemaining = currentAudioSampleBuffer->getNumSamples() - position;
			auto samplesThisTime = jmin(outputSamplesRemaining, bufferSamplesRemaining);

			for (auto channel = 0; channel < numOutputChannels; ++channel)
			{
				bufferToFill.buffer->copyFrom(channel,
				bufferToFill.startSample + outputSamplesOffset,
				*currentAudioSampleBuffer,
				channel % numInputChannels,
				position,
				samplesThisTime);
			}

			outputSamplesRemaining -= samplesThisTime;
			outputSamplesOffset += samplesThisTime;
			position += samplesThisTime;

			if (position == currentAudioSampleBuffer->getNumSamples())
				position = 0;
		}
	
		retainedCurrentBuffer->position = position;
		positionOverlay.setPosition(position);
	}
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

	Rectangle<int> thumbnailBounds(border, border, getWidth() - 4 * border - 2 * buttonWidth, getHeight() - 2 * border);
	thumbnail.setBounds(thumbnailBounds);
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
}

//==============================================================================

void MainComponent::checkForPathToOpen()
{
	String pathToOpen;
	pathToOpen.swapWith(chosenPath);

	if (pathToOpen.isNotEmpty())
	{
		File file(pathToOpen);

		if (auto* reader = formatManager.createReaderFor(file))
		{
			currentBuffer = nullptr;
			thumbnail.isLoading(true);

			ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(reader->numChannels, (int)reader->lengthInSamples);
			reader->read(newBuffer->getAudioSampleBuffer(), 0, (int)reader->lengthInSamples, 0, true, true);
			currentBuffer = newBuffer;
			buffers.add(newBuffer);

			// Drawing Stuff
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
			positionOverlay.setLengthInSamples((int) reader->lengthInSamples);

			const MessageManagerLock mmLock;
			playButton.setEnabled(true);
			clearButton.setEnabled(true);
			thumbnail.isLoading(false);
			thumbnail.setFile(file);
			readerSource.reset(newSource.release());

			changeState(Stopping);
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
}