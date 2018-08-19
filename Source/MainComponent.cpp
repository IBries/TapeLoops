/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "Dimensions.h"

//==============================================================================

MainComponent::MainComponent()
{
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks.add(new TapeDeck());
		addAndMakeVisible(tapeDecks[tape]);
	}

	setSize(800, 700);
	setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
	shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape]->prepareToPlay(samplesPerBlockExpected,sampleRate);
	}
}

//==============================================================================

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	bufferToFill.clearActiveBufferRegion();

	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape]->getNextAudioBlock(bufferToFill);
	}
}

//==============================================================================

void MainComponent::releaseResources()
{
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape]->releaseResources();
	}
}

//==============================================================================

void MainComponent::paint (Graphics& g)
{
	g.fillAll(Colour(86, 32, 1));
}

//==============================================================================

void MainComponent::resized()
{
	auto area = getLocalBounds();
	area.reduce(Dimensions::BORDER, Dimensions::BORDER);

	auto tapeHeight = (area.getHeight() - ((NUM_TAPES - 1) * Dimensions::BORDER)) / 3;
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape]->setBounds(area.removeFromTop(tapeHeight));
		if (tape != NUM_TAPES - 1)
			area.removeFromTop(Dimensions::BORDER);
	}
}
