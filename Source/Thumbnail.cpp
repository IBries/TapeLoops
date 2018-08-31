/****************************//**
*	@file Thumbnail.cpp
*	@author Isaac Bries
*	@date Created:	8/01/2018
*	@date Edited:	8/01/2018
********************************/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Thumbnail.h"

/************************************************************//**
 *	@brief Default Constructor
 *
 *	@param[in] sourceSamplesPerThumbnailSample
 *			   sets resolution of waveform drawing
 *	@param[in] &formatManager
 *			   interprets file data for various audio formats
 ***************************************************************/
Thumbnail::Thumbnail(int sourceSamplesPerThumbnailSample,
	AudioFormatManager& formatManager)
	: thumbnailCache(3),
	  thumbnail(sourceSamplesPerThumbnailSample, formatManager, thumbnailCache),
	  backgroundColour(189, 92, 11),
	  foregroundColour(0, 61, 64)
{
	thumbnail.addChangeListener(this);

}

/********************//**
 *	@brief Destructor
 ***********************/
Thumbnail::~Thumbnail()
{
	thumbnailCache.clear();
}

/********************************************//**
 *	@brief Sets file to display as waveform
 *
 *	@param[in] &file
 *			   file holding audio data to draw
 *
 *	@return void
 ***********************************************/
void Thumbnail::setFile(const File& file)
{
	thumbnail.setSource(new FileInputSource(file));
}

/************************************************//**
 *	@brief Sets text to display, if applicable
 *
 *	@param[in] &g
 *			   graphics context with which to draw
 *	@param[in] message
 *			   text to display
 *
 *	@return void
 ***************************************************/
void Thumbnail::setMessage(Graphics& g, String message)
{
	g.fillAll(backgroundColour);
	g.setColour(foregroundColour);
	g.drawFittedText(message, getLocalBounds(), Justification::centred, 1, 1.0f);
}

/********************************************//**
 *	@brief Sets loading state of this Thumbnail
 *
 *	@param[in] loading
 *			   loading state of this Thumbnail
 *
 *	@return void
 ***********************************************/
void Thumbnail::isLoading(bool loading)
{
	isLoadingFile = loading;
}

/************************************************//**
 *	@brief Draws this component
 *
 *	@param[in] &g
 *			   graphics context with which to draw
 *
 *	@return void
 ***************************************************/
void Thumbnail::paint(Graphics& g)
{
	if (thumbnail.getNumChannels() == 0)
		if (isLoadingFile)
			setMessage(g, "Loading File...");
		else
			setMessage(g, "No File Loaded");
	else
		paintIfFileLoaded(g);
}

/****************************************************//**
 *	@brief Drawing when a file is successfully loaded
 *
 *	@param[in] &g
 *			   graphics context with which to draw
 *
 *	@return void
 *******************************************************/
void Thumbnail::paintIfFileLoaded(Graphics& g)
{
	g.fillAll(backgroundColour);
	g.setColour(foregroundColour);
	thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
}

/********************************************************//**
 *	@brief ChangeListener callback
 *
 *	@param[in] *source
 *			   ChangeBroadcaster that invoked the callback	*
 *
 *	@return void
 ***********************************************************/
void Thumbnail::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &thumbnail)
		thumbnailChanged();
}

/****************************************//**
 *	@brief Sets bounds of any subcomponents
 *
 *	@return void
 *******************************************/
void Thumbnail::resized()
{
}

/****************************************************//**
 *	@brief Callback when stated of thumbnail changes
 *
 *	@return void
 *******************************************************/
void Thumbnail::thumbnailChanged()
{
	repaint();
}
