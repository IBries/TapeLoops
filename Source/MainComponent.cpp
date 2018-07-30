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
	thumbnail(512, formatManager, thumbnailCache)
{
	addAndMakeVisible(&openButton);
	openButton.setButtonText("Open...");
	openButton.onClick = [this] { openButtonClicked(); };

	addAndMakeVisible(&playButton);
	playButton.setButtonText("Play");
	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setColour(TextButton::buttonColourId, Colours::green);
	playButton.setEnabled(false);

	addAndMakeVisible(&stopButton);
	stopButton.setButtonText("Stop");
	stopButton.onClick = [this] { stopButtonClicked(); };
	stopButton.setColour(TextButton::buttonColourId, Colours::red);
	stopButton.setEnabled(false);

	setSize(600, 400);

	formatManager.registerBasicFormats();
	transportSource.addChangeListener(this);
	thumbnail.addChangeListener(this);

	setAudioChannels(0, 2);
	startTimer(40);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	if (readerSource.get() == nullptr)
		bufferToFill.clearActiveBufferRegion();
	else
		transportSource.getNextAudioBlock(bufferToFill);
}

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

void MainComponent::resized()
{
	openButton.setBounds(10, 10, getWidth() - 20, 20);
	playButton.setBounds(10, 40, getWidth() - 20, 20);
	stopButton.setBounds(10, 70, getWidth() - 20, 20);
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

void MainComponent::transportSourceChanged()
{
	changeState(transportSource.isPlaying() ? Playing : Stopped);
}

void  MainComponent::thumbnailChanged()
{
	repaint();
}

void MainComponent::paintIfNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds)
{
	g.setColour(Colours::darkgrey);
	g.fillRect(thumbnailBounds);
	g.setColour(Colours::white);
	g.drawFittedText("No File Loaded:", thumbnailBounds, Justification::centred, 1.0f);
}

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

void MainComponent::timerCallback()
{
	repaint();
}

void MainComponent::openButtonClicked()
{
	FileChooser chooser("Select a Wave file to play...", File::nonexistent, "*.wav");

	if (chooser.browseForFileToOpen())
	{
		auto file = chooser.getResult();
		auto* reader = formatManager.createReaderFor(file);

		if (reader != nullptr)
		{
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
			transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
			playButton.setEnabled(true);
			thumbnail.setSource(new FileInputSource(file));
			readerSource.reset(newSource.release());
		}
	}
}

void MainComponent::playButtonClicked()
{
	changeState(Starting);
}

void MainComponent::stopButtonClicked()
{
	changeState(Stopping);
}