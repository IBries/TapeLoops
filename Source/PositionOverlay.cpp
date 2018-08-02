/*
  ==============================================================================

    PositionOverlay.cpp
    Created: 1 Aug 2018 8:44:27am
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PositionOverlay.h"

//==============================================================================
PositionOverlay::PositionOverlay()
{
	startTimer(redrawDelayInMilliSeconds);
}

PositionOverlay::~PositionOverlay()
{
}

void PositionOverlay::paint (Graphics& g)
{
	if (lengthInSamples > 0.0)
	{
		auto drawPosition = (position / lengthInSamples) * getWidth();

		g.setColour(Colour(0, 49, 3));
		g.drawLine(drawPosition, 0.0f, drawPosition, (float)getHeight(), 2.0f);
	}
}

void PositionOverlay::resized()
{
}

void PositionOverlay::setPosition(int newPosition)
{
	position = (float) newPosition;
}

int PositionOverlay::getPosition()
{
	return (int) position;
}

void PositionOverlay::setLengthInSamples(int numSamples)
{
	lengthInSamples = (float) numSamples;
}

int PositionOverlay::getLengthInSamples()
{
	return (int) lengthInSamples;
}

void PositionOverlay::timerCallback()
{
	repaint();
}