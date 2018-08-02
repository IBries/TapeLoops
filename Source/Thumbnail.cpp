/*
  ==============================================================================

    Thumbnail.cpp
    Created: 1 Aug 2018 8:44:13am
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Thumbnail.h"

//==============================================================================
Thumbnail::Thumbnail(int sourceSamplesPerThumbnailSample,
					 AudioFormatManager& formatManager,
					 AudioThumbnailCache& cache)
	: thumbnail(sourceSamplesPerThumbnailSample, formatManager, cache),
	  backgroundColour(189, 92, 11),
	  foregroundColour(0, 61, 64)
{
	thumbnail.addChangeListener(this);

}

Thumbnail::~Thumbnail()
{
}

void Thumbnail::setFile(const File& file)
{
	thumbnail.setSource(new FileInputSource(file));
}

void Thumbnail::paint (Graphics& g)
{
	if (thumbnail.getNumChannels() == 0)
		if (isLoadingFile)
			setMessage(g, "Loading File...");
		else
			setMessage(g, "No File Loaded");
	else
		paintIfFileLoaded(g);
}

void Thumbnail::setMessage(Graphics& g, String message)
{
	g.fillAll(backgroundColour);
	g.setColour(foregroundColour);
	g.drawFittedText(message, getLocalBounds(), Justification::centred, 1.0f);
}

void Thumbnail::isLoading(bool loading)
{
	isLoadingFile = loading;
}

void Thumbnail::paintIfFileLoaded(Graphics& g)
{
	g.fillAll(backgroundColour);
	g.setColour(foregroundColour);
	thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
}

void Thumbnail::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &thumbnail)
		thumbnailChanged();
}

void Thumbnail::resized()
{
}

//==============================================================================

void Thumbnail::thumbnailChanged()
{
	repaint();
}
