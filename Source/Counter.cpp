/*
  ==============================================================================

    Counter.cpp
    Created: 26 Jul 2018 11:36:30pm
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Counter.h"

//==============================================================================

Counter::Counter()
{
	Counter(0, 0, 0);
}

Counter::Counter(int startVal, int endVal, int step)
{
	start = startVal;
	end = endVal;
	stepSize = step;
	currentValue = start;
}

Counter::~Counter()
{
}

//==============================================================================


void Counter::paint (Graphics& g)
{
}

void Counter::resized()
{
}

//==============================================================================

void Counter::count()
{
	currentValue += stepSize;
	if (currentValue > end)
	{
		currentValue = start + (currentValue - end - 1);
	}
}

//==============================================================================

void Counter::setStartPoint(int startPoint)
{
	start = startPoint;
}

int Counter::getStartPoint()
{
	return start;
}

void Counter::setEndPoint(int endPoint)
{
	end = endPoint;
}

int Counter::getEndPoint()
{
	return end;
}

void Counter::setStepSize(int size)
{
	stepSize = size;
}

int Counter::getStepSize()
{
	return stepSize;
}

int Counter::getCurrentValue()
{
	return currentValue;
}