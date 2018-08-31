/****************************//**
 *	@file MainComponent.h
 *	@author Isaac Bries
 *	@date Created:	8/01/2018
 *	@date Edited:	8/01/2018
 *******************************/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TapeDeck.h"

/************************************************************************//**
*	@class MainComponent
*
*	@brief	Contains all subcomponents and provides some global controls
*
*	Contains an array of TapeDeck components from which to pull audio and
*	master controls (volume, etc.) which control global behavior.
****************************************************************************/
class MainComponent : public AudioAppComponent,
					  public Slider::Listener
{
public:
    MainComponent();			// Default Construtcor
    ~MainComponent();			// Deconstructor

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;	// Initializes audio playback
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;	// Fills buffer to send to audio system
    void releaseResources() override;												// Releases any leftover dependencies when the audio system shuts down

    void paint (Graphics& g) override;			// Draws this component
    void resized() override;					// Sets bounds of subcomponents

	void sliderValueChanged(Slider* slider);	// Slider::Listener callback

private:
	void masterVolumeChanged();		///< masterVolume callback

	const int NUM_TAPES = 3;		///< Number of independent audio buffers

	OwnedArray<TapeDeck> tapeDecks;	///< Scoped array of TapeDeck objects

	Slider masterVolume;			///< Controls master application volume
	float previousLevel;			///< Previous volume level, to calculate gain ramps
	float level;					///< Current volume lever

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
