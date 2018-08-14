/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent()
{
	tapeDecks = new TapeDeck[NUM_TAPES];
	buffers = new AudioBuffer<float>[NUM_TAPES];

	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		addAndMakeVisible(&tapeDecks[tape]);
	}

	setSize(800, TAPE_HEIGHT * NUM_TAPES);
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
		tapeDecks[tape].prepareToPlay(samplesPerBlockExpected,sampleRate);
	}
}

//==============================================================================

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	bufferToFill.clearActiveBufferRegion();

	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape].getNextAudioBlock(bufferToFill);
	}
}

//==============================================================================

void MainComponent::releaseResources()
{
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape].releaseResources();
	}
}

//==============================================================================

void MainComponent::paint (Graphics& g)
{
}

//==============================================================================

void MainComponent::resized()
{
	for (int tape = 0; tape < NUM_TAPES; tape++)
	{
		tapeDecks[tape].setBounds(0, tape * TAPE_HEIGHT, getWidth(), TAPE_HEIGHT);
	}
}
