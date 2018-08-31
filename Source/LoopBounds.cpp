/****************************//**
 *	@file LoopBounds.cpp
 *	@author Isaac Bries
 *	@date Created:	8/02/2018
 *	@date Edited:	8/02/2018
 *******************************/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LoopBounds.h"

/****************************//**
 *	@brief Default Constructor
 *******************************/
LoopBounds::LoopBounds()
{
	maxLength = 2;
	startSample = 0;
	endSample = maxLength;
	startDrawPosition = (float) (startSample / maxLength);
	endDrawPosition = (float) (endSample / maxLength);
}

/************************//**
 *	@brief Deconstructor
 ***************************/
LoopBounds::~LoopBounds()
{
}

/********************************************************//**
 *	@brief Draws both start and end point indicators and	
 *		   darkens the area between them.				
 *
 *	@param[in]	&g	the graphics context used to do the
 *					drawing operations
 *
 *	@return	void
 ***********************************************************/
void LoopBounds::paint (Graphics& g)
{
	drawLoopStartIndicator(g);
	drawLoopEndIndicator(g);
	drawFill(g);
}

/************************************************************//**
 *	@brief Called when this component's size has been changed.
 *		   Used for laying out any child components.
 *
 *	@return void
 ***************************************************************/
void LoopBounds::resized()
{
}

/****************************//**
 *	@brief Resets the drawing
 *
 *	@return void
 *******************************/
void LoopBounds::clear()
{
	startDrawPosition = 0;
	endDrawPosition = (float) getWidth();
	repaint();
}

/************************************************//**
 *	@brief Sets the maximum value for the endpoint.
 *
 *	@param[in] lengthInSamples
 *			   maximum value of the loop end point
 *
 *	@return void
 ***************************************************/
void LoopBounds::setMaxLength(int lengthInSamples)
{
	maxLength = lengthInSamples;
}

/************************************************//**
 *	@brief Gets the maximum value for the endpoint.
 *
 *	@return maximum value of the loop end point
 ***************************************************/
int LoopBounds::getMaxLength()
{
	return maxLength;
}

/************************************************//**
 *	@brief Sets location of the loop start point
 *
 *	@param[in] sample
 *			   sample at which to begin loop
 *
 *	@return void
 ***************************************************/
void LoopBounds::setStartSample(int sample)
{
	startSample = sample;
}

/************************************************//**
 *	@brief Gets location of the loop start point
 *
 *	@return sample at which to begin loop
 ***************************************************/
int LoopBounds::getStartSample()
{
	return startSample;
}

/************************************************//**
 *	@brief Sets location of the loop end point
 *
 *	@param[in] sample
 *			   sample at which to end loop
 *
 *	@return void
 ***************************************************/
void LoopBounds::setEndSample(int sample)
{
	endSample = sample;
}

/********************************************//**
 *	@brief Gets location of the loop end point
 *
 *	@return sample at which to end loop
 ***********************************************/
int LoopBounds::getEndSample()
{
	return endSample;
}

/****************************************************//**
 *	@brief Sets location of the loop start point
 *		   as a ratio of the component width
 *
 *	@param[in] pos
 *			   location to draw loop start indicator
 *
 *	@return void
 ********************************************************/
void LoopBounds::setStartDrawPosition(float pos)
{
	startDrawPosition = pos;
}

/************************************************//**
 *	@brief Sets location of the loop end point
 *		   as a ratio of the component width
 *
 *	@param[in] pos
 *			   location to draw loop end indicator
 *
 *	@return void
 ****************************************************/
void LoopBounds::setEndDrawPosition(float pos)
{
	endDrawPosition = pos;
}

/****************************************************//**
 *	@brief Draws the loop start point indicator
 *
 *	@param[in]	&g
 *				the graphics context used to do the
 *				drawing operations
 *
 *	@return void
 *******************************************************/
void LoopBounds::drawLoopStartIndicator(Graphics& g)
{
	g.setColour(Colours::darkblue);
	g.fillRect((float)startDrawPosition, 0.0f, 2.0f, (float)getHeight());
}

/****************************************************//**
 *	@brief Draws the loop end point indicator
 *
 *	@param[in]	&g
 *				the graphics context used to do the
 *				drawing operations
 *
 *	@return void
 *******************************************************/
void LoopBounds::drawLoopEndIndicator(Graphics& g)
{
	g.setColour(Colours::darkblue);
	if (endDrawPosition >= getWidth() - 1)
		endDrawPosition = (float)getWidth() - 2;
	g.fillRect(endDrawPosition, 0.0f, 2.0f, (float)getHeight());
}

/****************************************************//**
 *	@brief Draws the fill between loop boundary
 *		   indicators
 *
 *	@param[in]	&g
 *				the graphics context used to do the
 *				drawing operations
 *
 *	@return void
 *******************************************************/
void LoopBounds::drawFill(Graphics& g)
{
	uint8 red = 128;
	uint8 green = 128;
	uint8 blue = 128;
	float transparency = 0.25;
	g.setColour(Colour(red, green, blue, transparency));
	g.fillRect((float)startDrawPosition, 0.0f, endDrawPosition - startDrawPosition, (float)getHeight());
}
