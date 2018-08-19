/*
  ==============================================================================

    TapeDeck.h
    Created: 8 Aug 2018 9:50:34pm
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Thumbnail.h"
#include "PositionOverlay.h"
#include "LoopBounds.h"
#include "TapeControl.h"

//==============================================================================
class TapeDeck    : public Component,
					public Thread,
					public Button::Listener,
					public Slider::Listener
{
public:
	class ReferenceCountedBuffer : public ReferenceCountedObject
	{
	public:
		typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;

		ReferenceCountedBuffer(int numChannels, int numSamples) :
			buffer(numChannels, numSamples)
		{
		}

		~ReferenceCountedBuffer()
		{
		}

		AudioSampleBuffer* getAudioSampleBuffer()
		{
			return &buffer;
		}

		int position = 0;

	private:
		AudioSampleBuffer buffer;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedBuffer)
	};
    TapeDeck();
    ~TapeDeck();

	ReferenceCountedBuffer::Ptr currentBuffer;
	ReferenceCountedBuffer::Ptr currentFadeBuffer;

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
	void releaseResources();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button* button);

	void sliderValueChanged(Slider* slider);

	enum TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};

	void changeState(TransportState newState);
	void clearCurrentBuffer();
	void clearThumbnail();
	void clearLoopBounds();

	//==============================================================================
private:
	const int LOOP_FADE_LENGTH_IN_SAMPLES = 4410;

	TapeControl controls;

	AudioFormatManager formatManager;
	TransportState state;
	Thumbnail thumbnail;
	PositionOverlay positionOverlay;
	LoopBounds loopBounds;

	ReferenceCountedArray<ReferenceCountedBuffer> buffers;
	String chosenPath;

	ReferenceCountedArray<ReferenceCountedBuffer> fadeBuffers;

	void run() override;
	void checkForBuffersToFree();
	void checkForPathToOpen();

	bool nearBeginning(int position);
	bool nearEnd(int position);

	void connectControls();

	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();
	void clearButtonClicked();

	void startSampleChanged();
	void endSampleChanged();

	void applyCrossFade();
	float calculateDrawPosition(int sample, int lengthInSamples, int width);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeDeck)
};
