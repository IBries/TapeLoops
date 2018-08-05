/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Thumbnail.h"
#include "PositionOverlay.h"
#include "LoopBounds.h"

//==============================================================================
class MainComponent : public AudioAppComponent,
					  private Thread
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
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
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


	//==============================================================================
	TextButton openButton;
	TextButton playButton;
	TextButton stopButton;
	TextButton clearButton;
	Slider startSampleSlider;
	Slider endSampleSlider;

	int border = 10;
	int buttonWidth = 100;

	AudioFormatManager formatManager;
	std::unique_ptr<AudioFormatReaderSource> readerSource;
	TransportState state;
	AudioThumbnailCache thumbnailCache;
	Thumbnail thumbnail;
	PositionOverlay positionOverlay;
	LoopBounds loopBounds;

	ReferenceCountedArray<ReferenceCountedBuffer> buffers;
	ReferenceCountedBuffer::Ptr currentBuffer;
	String chosenPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
