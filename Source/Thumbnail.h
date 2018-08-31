/********************************
 *	@file Thumbnail.h
 *	@author Isaac Bries
 *	@date Created:	8/01/2018
 *	@date Edited:	8/01/2018
 *******************************/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/************************************************************************//**
 *	@class Thumbnail
 *
 *	@brief Displays the contents of an audio buffer
 *
 *	Displays the waveform of an audio file loaded by a TapeDeck component	*
 ****************************************************************************/
class Thumbnail    : public Component,
					 private ChangeListener
{
public:
    Thumbnail(int sourceSamplesPerThumbnailSample, AudioFormatManager& formatManager);	// Default Constructor
    ~Thumbnail();																		// Deconstructor

	void setFile(const File& file);					// Sets file to display as waveform
	void setMessage(Graphics& g, String Message);	// Sets text to display, if applicable

	void isLoading(bool loading);					// Sets loading state of this Thumbnail

    void paint (Graphics&) override;						// Draws this component
	void paintIfFileLoaded(Graphics& g);					// Drawing when a file is successfully loaded
	void changeListenerCallback(ChangeBroadcaster* source);	// ChangeListener callback
    void resized() override;								// Sets bounds of any subcomponents

	void thumbnailChanged();						// Callback when stated of thumbnail changes

private:
	bool isLoadingFile = false;				///< loading state of this Thumbnail
	AudioThumbnailCache thumbnailCache;		///< holds old waveform data for quicker loading
	AudioThumbnail thumbnail;				///< displays the waveform of an audio file
	Colour backgroundColour;				///< colour behind waveform
	Colour foregroundColour;				///< waveform color

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Thumbnail)
};
