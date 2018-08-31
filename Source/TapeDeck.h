/****************************//**
*	@file TapeDeck.h
*	@author Isaac Bries
*	@date Created:	8/08/2018
*	@date Edited:	8/08/2018
********************************/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Thumbnail.h"
#include "PositionOverlay.h"
#include "LoopBounds.h"
#include "TapeControl.h"

/********************************************************************************//**
*	@class TapeDeck
*
*	@brief Contains a single audio loop, cursor drawings, and a control panel
*
*	Holds a single audio buffer and the controls to manipulate it. Also contains
*	the graphics components which draw playback and loop bounds cursors.
************************************************************************************/
class TapeDeck    : public Component,
					public Thread,
					public Button::Listener,
					public Slider::Listener
{
public:
	/********************************************************************//**
	 *	@class ReferenceCountedBuffer
	 *
	 *	@brief Scoped pointer to an audio buffer
	 *
	 *	@section DESCRIPTION
	 *
	 *	Allows for some clever multi-threading to free up the audio thread
	 ************************************************************************/
	class ReferenceCountedBuffer : public ReferenceCountedObject
	{
	public:
		typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;		///< scoped pointer to this class

		/********************************************************//**
		 *	@brief Default Constructor
		 *
		 *	@param[in] numChannels
		 *			   the number of channels in the audio buffer
		 *	@param[in] numSamples
		 *			   the number of samples in the audio buffer
		 ***********************************************************/
		ReferenceCountedBuffer(int numChannels, int numSamples) :
			buffer(numChannels, numSamples)
		{
		}

		/************************//**
		 *	@brief Deconstructor
		 ***************************/
		~ReferenceCountedBuffer()
		{
		}

		/************************************************//**
		 *	@brief returns a pointer to the audio buffer
		 *
		 *	@return a pointer to the audio buffer
		 ***************************************************/
		AudioSampleBuffer* getAudioSampleBuffer()
		{
			return &buffer;
		}

		int position = 0;			///< current playback position

	private:
		AudioSampleBuffer buffer;	///< audio buffer

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedBuffer)
	};

    TapeDeck();			// Default Constructor
    ~TapeDeck();		// Deconstructor

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);		// Prepares this component for audio playback
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);		// Fills the next buffer to send to audio system
	void releaseResources();												// Cleans up any mess when the audio system is shut down

    void paint (Graphics&) override;			// Draws this component
    void resized() override;					// Sets bounds of any subcomponents

	void buttonClicked(Button* button);			// Button::Listener callback
	void sliderValueChanged(Slider* slider);	// Slider::Listener callback

	enum TransportState			///< playback state of this TapeDeck
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};

	void changeState(TransportState newState);		// Changes the playback state of the TapeDeck
	void clearCurrentBuffer();						// Clears the buffer held by TapeDeck
	void clearThumbnail();							// Clears the thumbnail drawing of the current buffer
	void clearLoopBounds();							// Clears the LoopBounds drawing

	ReferenceCountedBuffer::Ptr currentBuffer;			///< current audio buffer
	ReferenceCountedBuffer::Ptr currentFadeBuffer;		///< current audio buffer with fades for loop boundaries

private:
	const int LOOP_FADE_LENGTH_IN_SAMPLES = 4410;	///< length of the click-removing fade at loop boundary

	TapeControl controls;				///< control panel

	AudioFormatManager formatManager;	///< contains information for reading various file formats
	TransportState state;				///< current playback state
	Thumbnail thumbnail;				///< waveform drawing of the current audio buffer
	PositionOverlay positionOverlay;	///< current playback position drawing
	LoopBounds loopBounds;				///< drawing of loop boundaries

	ReferenceCountedArray<ReferenceCountedBuffer> buffers;	///< scoped array of audio buffers
	String chosenPath;										///< filepath of file to open

	ReferenceCountedArray<ReferenceCountedBuffer> fadeBuffers;	///< scoped array of audio buffers with fade data for loop boundaries

	void run() override;				// Starts a thread to handle file access and memory allocation
	void checkForBuffersToFree();		// Checks if any ReferenceCountedBuffer in buffers are not in use
	void checkForPathToOpen();			// Checks if there is a file operation waiting to happen

	bool nearBeginning(int position);	// True if playback position is less than LOOP_FADE_LENGTH_IN_SAMPLES samples past the beginning of the loop
	bool nearEnd(int position);			// True if playback position is less than LOOP_FADE_LENGTH_IN_SAMPLES samples before the end of the loop

	void connectControls();			// Creates Broadcaster/Listener relationships with controls

	void openButtonClicked();		// openButton callback
	void playButtonClicked();		// playButton callback
	void stopButtonClicked();		// stopButton callback
	void clearButtonClicked();		// clearButton callback

	void startSampleChanged();		// startSampleSlider callback
	void endSampleChanged();		// endSampleSlider callback

	float previousLevel;			///< previous volume level, used to calculate gain ramps
	float level;					///< current volume level
	void volumeSliderChanged();		// volumeSlider callback

	void applyCrossFade();			// Applies cross fade at loop boundary to remove discontinuities
	float calculateDrawPosition(int sample, int lengthInSamples, int width);	// Calculates where to draw position indicator

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeDeck)
};
