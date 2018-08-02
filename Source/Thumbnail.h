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
			  AudioFormatManager& formatManager,
			  AudioThumbnailCache& cache);
    ~Thumbnail();

	void setFile(const File& file);

    void paint (Graphics&) override;
	void paintIfNoFileLoaded(Graphics& g);
	void paintIfFileLoaded(Graphics& g);
	void changeListenerCallback(ChangeBroadcaster* source);
    void resized() override;

private:
	void thumbnailChanged();

	AudioThumbnail thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Thumbnail)
};
