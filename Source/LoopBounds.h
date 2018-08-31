/****************************//**
 *	@file LoopBounds.h
 *	@author Isaac Bries
 *	@date Created:	8/02/2018
 *	@date Edited:	8/02/2018
 *******************************/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

 /************************************************************//**
 *	@class LoopBounds
 *
 *	@brief Draws the currently selected loop points
 *
 *	Draws start and end cursors at the edges of the loops, as
 *	well as slightly darkening the loop region itself.
 ***************************************************************/
class LoopBounds    : public Component
{
public:
    LoopBounds();								// Default Constructor
    ~LoopBounds();								// Deconstructor

    void paint (Graphics&) override;			// Draws this component
    void resized() override;					// Draws subcomponents
	void clear();								// Resets this component's drawing
	
	void setMaxLength(int lengthInSamples);		// Sets the maximum value for the endpoint.
	int getMaxLength();							// Returns the maximum value for the endpoint.
	void setStartSample(int sample);			// Sets location of the loop start point
	int getStartSample();						// Returns the location of the loop start point
	void setEndSample(int sample);				// Sets location of the loop end point
	int getEndSample();							// Returns the location of the loop end point
	void setStartDrawPosition(float pos);		// Sets location of the loop start point
	void setEndDrawPosition(float pos);			// Sets location of the loop end point

private:
	void drawLoopStartIndicator(Graphics& g);	// Draws the loop start point indicator
	void drawLoopEndIndicator(Graphics& g);		// Draws the loop end point indicator
	void drawFill(Graphics& g);					// Draws the fill between loop boundary

	int startSample;							///< First sample in the loop
	float startDrawPosition;					///< Loop start position as a ratio of component size
	int endSample;								///< Last sample in the loop
	float endDrawPosition;						///< Loop end position as a ratio of component size
	int maxLength;								///< Number of samples in the longest loop possible for a given buffer

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopBounds)
};
