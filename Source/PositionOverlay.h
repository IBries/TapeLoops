/****************************//**
 *	@file PositionOverlay.h
 *	@author Isaac Bries
 *	@date Created:	8/01/2018
 *	@date Edited:	8/01/2018
 *******************************/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/************************************************************//**
*	@class PositionOverlay
*
*	@brief Draws a playback indicator
*
*	Draws a position cursor at the current playback position.
****************************************************************/
class PositionOverlay    : public Component,
						   private Timer
{
public:
    PositionOverlay();					// Default Constructor
    ~PositionOverlay();					// Deconstructor

    void paint (Graphics&) override;	// Draws this component
    void resized() override;			// Sets bounds of subcomponents

	void timerCallback() override;		// Redraws position indicator when invoked by timer

	void setPosition(int newPosition);			// Sets indicator position
	int getPosition();							// Returns the indicator position
	void setLengthInSamples(int numSamples);	// Sets length of current audio buffer, in samples
	int getLengthInSamples();					// Returns the length of current audio buffer, in samples

private:
	float position = 0.0;					///< Current playback position
	float lengthInSamples = 0.0;			///< Length of current audio buffer, in samples
	int redrawDelayInMilliSeconds = 40;		///< Time between indicator redraws, in milliseconds

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionOverlay)
};
