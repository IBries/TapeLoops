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
	: thumbnail(sourceSamplesPerThumbnailSample, formatManager, cache)
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
		paintIfNoFileLoaded(g);
	else
		paintIfFileLoaded(g);
}

void Thumbnail::paintIfNoFileLoaded(Graphics& g)
{
	g.fillAll(Colours::white);
	g.setColour(Colours::darkgrey);
	g.drawFittedText("No File Loaded", getLocalBounds(), Justification::centred, 1.0f);
}

void Thumbnail::paintIfFileLoaded(Graphics& g)
{
	g.fillAll(Colours::white);
	g.setColour(Colours::red);
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
