/*
  ==============================================================================

    PositionOverlay.h
    Created: 1 Aug 2018 8:44:27am
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class PositionOverlay    : public Component,
						   private Timer
{
public:
    PositionOverlay();
    ~PositionOverlay();

    void paint (Graphics&) override;
    void resized() override;

	void timerCallback() override;

	void setPosition(int newPosition);
	int getPosition();
	void setLengthInSamples(int numSamples);
	int getLengthInSamples();

private:
	float position = 0.0;
	float lengthInSamples = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionOverlay)
};
