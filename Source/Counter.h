/*
  ==============================================================================

    Counter.h
    Created: 26 Jul 2018 11:36:30pm
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Counter    : public Component
{
public:
	Counter();
	Counter(int startVal, int endVal, int step);
    ~Counter();

	// Inherited from Component class
    void paint (Graphics&) override;
    void resized() override;
	
	// Actual Functionality
	void count();
	
	// Setters and Getters
	void setStartPoint(int startPoint);
	int getStartPoint();
	void setEndPoint(int endPoint);
	int getEndPoint();
	void setStepSize(int size);
	int getStepSize();
	int getCurrentValue();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Counter)
	
	int start;
	int end;
	int stepSize;
	int currentValue;
};
