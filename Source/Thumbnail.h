/*
  ==============================================================================

    Thumbnail.h
    Created: 1 Aug 2018 8:44:13am
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Thumbnail    : public Component,
					 private ChangeListener
{
public:
    Thumbnail(int sourceSamplesPerThumbnailSample,
			  AudioFormatManager& formatManager);
    ~Thumbnail();

	void setFile(const File& file);
	void setMessage(Graphics& g, String Message);

	void isLoading(bool loading);

    void paint (Graphics&) override;
	void paintIfFileLoaded(Graphics& g);
	void changeListenerCallback(ChangeBroadcaster* source);
    void resized() override;

private:
	void thumbnailChanged();

	bool isLoadingFile = false;
	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;
	Colour backgroundColour;
	Colour foregroundColour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Thumbnail)
};
