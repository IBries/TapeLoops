/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class MainComponent   : public AudioAppComponent,
						public ChangeListener,
						private Timer,
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

	//==============================================================================
	void changeListenerCallback(ChangeBroadcaster* source) override;

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
	void resetPlayPosition(const AudioSourceChannelInfo& bufferToFill, int& offsetSamples, int& offsetSamplesRemaining);

	void transportSourceChanged();
	void thumbnailChanged();

	void paintIfNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);
	void paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);

	void timerCallback() override;

	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();
	void clearButtonClicked();


	//==============================================================================
	TextButton openButton;
	TextButton playButton;
	TextButton stopButton;
	TextButton clearButton;

	AudioFormatManager formatManager;
	std::unique_ptr<AudioFormatReaderSource> readerSource;
	//AudioTransportSource transportSource;
	TransportState state;
	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;

	ReferenceCountedArray<ReferenceCountedBuffer> buffers;
	ReferenceCountedBuffer::Ptr currentBuffer;
	String chosenPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
