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

//==============================================================================
class TapeDeck    : public Component,
					private Thread,
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

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
	void releaseResources();

    void paint (Graphics&) override;
    void resized() override;

	void sliderValueChanged(Slider* slider);

	//==============================================================================
private:
	enum TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};
	void changeState(TransportState newState);

	void run() override;
	void checkForBuffersToFree();
	void checkForPathToOpen();

	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();
	void clearButtonClicked();

	int getThumbnailWidth();

	bool nearEnd(int position);
	bool nearBeginning(int position);

	TextButton openButton;
	TextButton playButton;
	TextButton stopButton;
	TextButton clearButton;
	Slider startSampleSlider;
	Slider endSampleSlider;

	int border = 10;
	int buttonWidth = 100;

	AudioFormatManager formatManager;
	//std::unique_ptr<AudioFormatReaderSource> readerSource;
	AudioFormatReaderSource* readerSource;
	TransportState state;
	AudioThumbnailCache thumbnailCache;
	Thumbnail thumbnail;
	PositionOverlay positionOverlay;
	LoopBounds loopBounds;

	ReferenceCountedArray<ReferenceCountedBuffer> buffers;
	ReferenceCountedBuffer::Ptr currentBuffer;
	String chosenPath;

	ReferenceCountedArray<ReferenceCountedBuffer> fadeBuffers;
	ReferenceCountedBuffer::Ptr currentFadeBuffer;
	int loopFadeLengthInSamples = 4410;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeDeck)
};
