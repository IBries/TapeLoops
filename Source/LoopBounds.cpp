/*
  ==============================================================================

    LoopBounds.cpp
    Created: 2 Aug 2018 10:56:03pm
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LoopBounds.h"

//==============================================================================
LoopBounds::LoopBounds()
{
}

LoopBounds::~LoopBounds()
{
}

void LoopBounds::paint (Graphics& g)
{
	drawLoopStartIndicator(g);
	drawLoopEndIndicator(g);
	drawFill(g);
}

void LoopBounds::resized()
{
}

void LoopBounds::clear()
{
	startDrawPosition = 0;
	endDrawPosition = (float) getWidth();
	repaint();
}

void LoopBounds::drawLoopStartIndicator(Graphics& g)
{
	g.setColour(Colours::darkblue);
	g.fillRect((float)startDrawPosition, 0.0f, 2.0f, (float)getHeight());
}

void LoopBounds::drawLoopEndIndicator(Graphics& g)
{
	g.setColour(Colours::darkblue);
	if (endDrawPosition >= getWidth() - 1)
		endDrawPosition = (float) getWidth() - 2;
	g.fillRect(endDrawPosition, 0.0f, 2.0f, (float)getHeight());
}

void LoopBounds::drawFill(Graphics& g)
{
	uint8 red = 128;
	uint8 green = 128;
	uint8 blue = 128;
	float transparency = 0.25;
	g.setColour(Colour(red, green, blue, transparency));
	g.fillRect((float)startDrawPosition, 0.0f, endDrawPosition - startDrawPosition, (float)getHeight());
}

void LoopBounds::setMaxLength(int lengthInSamples)
{
	maxLength = lengthInSamples;
}

int LoopBounds::getMaxLength()
{
	return maxLength;
}

void LoopBounds::setStartSample(int sample)
{
	startSample = sample;
}

int LoopBounds::getStartSample()
{
	return startSample;
}

void LoopBounds::
setEndSample(int sample)
{
	endSample = sample;
}

int LoopBounds::getEndSample()
{
	return endSample;
}

void LoopBounds::setStartDrawPosition(float pos)
{
	startDrawPosition = pos;
}

void LoopBounds::setEndDrawPosition(float pos)
{
	endDrawPosition = pos;
}
