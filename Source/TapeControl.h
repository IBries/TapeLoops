/****************************//**
 *	@file TapeControl.h
 *	@author Isaac Bries
 *	@date Created:	08/19/2018
 *	@date Edited:	08/19/2018
 *******************************/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

 /***********************************************************//**
  *	@class TapeControl
  *
  *	@brief Controls the behavior of a single TapeDeck component
  *
  *	Contains a set of controls to open and close audio files,
  *	as well as basic transport and volume controls.
  ***************************************************************/
class TapeControl    : public Component
{
public:
    TapeControl();			// Default Constructor
    ~TapeControl();			// Deconstructor

    void paint (Graphics&) override;		// Draws this component
    void resized() override;				// Sets bounds of any subcomponents

	void setButtonState(String button, bool state);			// Sets the enable state of a button
	void setSliderState(String slider, bool state);			// Sets the enable state of a slider
	void setSliderRange(String slider, int min, int max);	// Sets the range of a slider
	void setSliderValue(String slider, int val);			// Sets the current value of a slider

	int getStartSample();					// Returns the first sample in the loop 
	int getEndSample();						// Returns the last sample in the loop

	Button* getButtonPtr(String button);	// Returns a Button pointer
	Slider* getSliderPtr(String slider);	// Returns a Slider pointer

private:
	TextButton openButton;			///< Opens a file
	TextButton playButton;			///< Plays the audio buffer
	TextButton stopButton;			///< Stops playback
	TextButton clearButton;			///< Clears the current audio buffer
	Slider startSampleSlider;		///< Controls position of the beginning of the loop
	Slider endSampleSlider;			///< Controls position of the end of the loop
	Slider volumeSlider;			///< Controls volume of this audio buffer

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeControl)
};
