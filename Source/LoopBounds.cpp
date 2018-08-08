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
LoopBounds::LoopBounds(Slider &start, Slider &end)
{
	startSampleSlider = &start;
	endSampleSlider = &end;
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
	startSampleSlider->setValue(0);
	endSampleSlider->setValue(getWidth());
	startDrawPosition = 0;
	endDrawPosition = getWidth();
	repaint();
}

void LoopBounds::sliderValueChanged(Slider* slider)
{
	if (slider == startSampleSlider)
		startSampleChanged(slider);
	else if (slider == endSampleSlider)
		endSampleChanged(slider);
}

void LoopBounds::startSampleChanged(Slider* slider)
{
	startSample = slider->getValue();
	if (startSample >= maxLength)
		endSampleSlider->setRange(maxLength - 1, maxLength);
	else
		endSampleSlider->setRange(startSample, maxLength);
	startDrawPosition = (float)startSample / (float)maxLength * (float)getWidth();
	repaint();
}

void LoopBounds::endSampleChanged(Slider* slider)
{
	endSample = slider->getValue();
	if (endSample - 1 <= 0)
		startSampleSlider->setRange(0, 1);
	else
		startSampleSlider->setRange(0, endSample - 1);
	endDrawPosition = (float)endSample / (float)maxLength * (float)getWidth();
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
		endDrawPosition = getWidth() - 2;
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
