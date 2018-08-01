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
	playButton.setColour(TextButton::buttonColourId, Colours::green);
	playButton.setEnabled(false);

	addAndMakeVisible(stopButton);
	stopButton.setButtonText("Stop");
	stopButton.onClick = [this] { stopButtonClicked(); };
	stopButton.setColour(TextButton::buttonColourId, Colours::red);
	stopButton.setEnabled(false);

	addAndMakeVisible(clearButton);
	clearButton.setButtonText("Clear");
	clearButton.onClick = [this] { clearButtonClicked(); };

	setSize(600, 400);

	formatManager.registerBasicFormats();
	transportSource.addChangeListener(this);
	thumbnail.addChangeListener(this);

	setAudioChannels(0, 2);
	startThread();
	startTimer(40);
}

MainComponent::~MainComponent()
{
	stopThread(4000);
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
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
	}
}

//==============================================================================

void MainComponent::releaseResources()
{
	transportSource.releaseResources();
}

//==============================================================================

void MainComponent::paint (Graphics& g)
{
	Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);

	if (thumbnail.getNumChannels() == 0)
		paintIfNoFileLoaded(g, thumbnailBounds);
	else
		paintIfFileLoaded(g, thumbnailBounds);
}

//==============================================================================

void MainComponent::resized()
{
	openButton.setBounds(10, 10, getWidth() - 20, 20);
	playButton.setBounds(10, 40, getWidth() - 20, 20);
	stopButton.setBounds(10, 70, getWidth() - 20, 20);
	clearButton.setBounds(10, 100, getWidth() - 20, 20);
}

//==============================================================================

void MainComponent::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &transportSource)
		transportSourceChanged();
	if (source == &thumbnail)
		thumbnailChanged();
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
			stopButton.setEnabled(false);
			if (currentBuffer == nullptr)
				playButton.setEnabled(false);
			else
				playButton.setEnabled(true);
			transportSource.setPosition(0.0);
			break;

		case Starting:
			playButton.setEnabled(false);
			transportSource.start();
			break;

		case Playing:
			stopButton.setEnabled(true);
			break;

		case Stopping:
			transportSource.stop();
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
		std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));
		auto* drawReader = formatManager.createReaderFor(file);

		if (reader.get() != nullptr)
		{
			auto duration = reader->lengthInSamples / reader->sampleRate;

			ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(reader->numChannels, (int)reader->lengthInSamples);
			reader->read(newBuffer->getAudioSampleBuffer(), 0, (int)reader->lengthInSamples, 0, true, true);
			currentBuffer = newBuffer;
			buffers.add(newBuffer);

			// Thumbnail Drawing Stuff
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(drawReader, true));
			transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
			
			const MessageManagerLock mmLock;
			playButton.setEnabled(true);
			thumbnail.setSource(new FileInputSource(file));
			readerSource.reset(newSource.release());
		}
	}
}

//==============================================================================

void MainComponent::transportSourceChanged()
{
	changeState(transportSource.isPlaying() ? Playing : Stopped);
}

//==============================================================================

void  MainComponent::thumbnailChanged()
{
	repaint();
}

//==============================================================================

void MainComponent::paintIfNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds)
{
	g.setColour(Colours::darkgrey);
	g.fillRect(thumbnailBounds);
	g.setColour(Colours::white);
	g.drawFittedText("No File Loaded:", thumbnailBounds, Justification::centred, 1.0f);
}

//==============================================================================

void MainComponent::paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds) {
	g.setColour(Colours::white);
	g.fillRect(thumbnailBounds);
	g.setColour(Colours::red);

	auto audioLength(thumbnail.getTotalLength());

	thumbnail.drawChannels(g, thumbnailBounds, 0.0, audioLength, 1.0f);

	g.setColour(Colours::green);

	auto audioPosition(transportSource.getCurrentPosition());
	auto drawPosition((audioPosition / audioLength) * thumbnailBounds.getWidth() + thumbnailBounds.getX());

	g.drawLine(drawPosition, thumbnailBounds.getY(), drawPosition, thumbnailBounds.getBottom(), 2.0f);
}

//==============================================================================

void MainComponent::timerCallback()
{
	repaint();
}

//==============================================================================

void MainComponent::openButtonClicked()
{
	FileChooser chooser("Select a Wave file to play...", File::nonexistent, "*.wav");

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
	currentBuffer = nullptr;
	thumbnail.clear();
}