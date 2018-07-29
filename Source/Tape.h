/*
  ==============================================================================

    Tape.h
    Created: 26 Jul 2018 11:21:26pm
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Counter.h"
#include "Control.h"

//==============================================================================
/*
*/
class Tape    : public Component,
				public Button::Listener,
				public Slider::Listener,
				public ChangeListener
{
public:
    Tape();
    ~Tape();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button* button);
	void sliderValueChanged(Slider* slider);
	void changeListenerCallback(ChangeBroadcaster* source);

private:
	void loadButtonClicked();
	void thumbnailChanged();
	void paintIfNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);
	void paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);

	Counter counter;
	Control control;

	AudioFormatManager formatManager;
	std::unique_ptr<AudioFormatReaderSource> readerSource;
	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tape)
};
