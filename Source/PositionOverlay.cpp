/****************************//**
 *	@file PositionOverlay.cpp
 *	@author Isaac Bries
 *	@date Created: 8/01/2018
 *	@date Edited: 8/01/2018
 *******************************/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PositionOverlay.h"

/****************************//**
 *	@brief	Default Constructor
 *******************************/
PositionOverlay::PositionOverlay()
{
	startTimer(redrawDelayInMilliSeconds);
}

/************************//**
 *	@brief	Deconstructor
 ***************************/
PositionOverlay::~PositionOverlay()
{
}

/************************************************//**
 *	@brief Draws this component
 *
 *	@param[in] &g	
 *			   the graphics context used to do the
 *			   drawing operations
 *
 *	@return void
 ***************************************************/
void PositionOverlay::paint (Graphics& g)
{
	if (lengthInSamples > 0.0)
	{
		auto drawPosition = (position / lengthInSamples) * getWidth();

		g.setColour(Colour(0, 49, 3));
		g.drawLine(drawPosition, 0.0f, drawPosition, (float)getHeight(), 2.0f);
	}
}

/****************************************//**
 *	@brief Sets bounds of any subcomponents
 *
 *	@return void
 *******************************************/
void PositionOverlay::resized()
{
}

/****************************************************//**
 *	@brief Sets indicator position
 *
 *	@param[in] newPosition
 *			   position at which to set the indicator
 *
 *	@return void
 *******************************************************/
void PositionOverlay::setPosition(int newPosition)
{
	position = (float) newPosition;
}

/********************************//**
 *	@brief Gets indicator position
 *
 *	@return Indicator position
 ***********************************/
int PositionOverlay::getPosition()
{
	return (int) position;
}

/****************************************************//**
 *	@brief Sets length of current audio buffer
 *
 *	@param[in] numSamples
 *			   Number of samples in the currrent buffer	*
 *
 *	@return void
 *******************************************************/
void PositionOverlay::setLengthInSamples(int numSamples)
{
	lengthInSamples = (float) numSamples;
}

/********************************************//**
 *	@brief Gets length of current audio buffer
 *
 *	@return Indicator position
 ***********************************************/
int PositionOverlay::getLengthInSamples()
{
	return (int) lengthInSamples;
}

/********************************************************************//**
 *	@brief Redraws this component when the timer invokes the callback
 *
 *	@return void
 ***********************************************************************/
void PositionOverlay::timerCallback()
{
	repaint();
}
