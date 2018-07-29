/*
  ==============================================================================

    Tape.cpp
    Created: 26 Jul 2018 11:21:26pm
    Author:  Isaac Bries

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Tape.h"

//==============================================================================
Tape::Tape() :
	thumbnailCache(5),
	thumbnail(512, formatManager, thumbnailCache)
{
	formatManager.registerBasicFormats();
	thumbnail.addChangeListener(this);

	addAndMakeVisible(&control);
	control.connectControls(this, this);
}

Tape::~Tape()
{
}

void Tape::paint (Graphics& g)
{
	Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);

	if (thumbnail.getNumChannels() == 0)
		paintIfNoFileLoaded(g, thumbnailBounds);
	else
		paintIfFileLoaded(g, thumbnailBounds);
}

void Tape::resized()
{
	control.setBounds(getLocalBounds());
}

void Tape::buttonClicked(Button* button)
{
	if (button == &control.loadButton)
		loadButtonClicked();
	else if (button == &control.playButton)
	{
		//TODO play the audio file
	}
	else if (button == &control.stopButton)
	{
		//TODO stop playing the audio file
	}
}

void Tape::sliderValueChanged(Slider* slider)
{
	if (slider == &control.volumeSlider)
	{
		//TODO change the volume of this tape
	}
	else if (slider == &control.startPointSlider)
	{
		//TODO change the loop start point
	}
	else if(slider == &control.endPointSlider)
	{
		//TODO change the loop end point
	}
}

void Tape::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &thumbnail)
		thumbnailChanged();
}

void Tape::loadButtonClicked()
{
	FileChooser chooser("Select a Wave file to play...", File::nonexistent, "*.wav");

	if (chooser.browseForFileToOpen())
	{
		File file(chooser.getResult());
		auto* reader = formatManager.createReaderFor(file);

		if (reader != nullptr)
		{
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
			control.playButton.setEnabled(true);
			thumbnail.setSource(new FileInputSource(file));
			readerSource.reset(newSource.release());
		}
	}
}

void Tape::thumbnailChanged()
{
	repaint();
}

void Tape::paintIfNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds)
{
	g.setColour(Colours::darkgrey);
	g.fillRect(thumbnailBounds);
	g.setColour(Colours::white);
	g.drawFittedText("No File Loaded", thumbnailBounds, Justification::centred, 1.0f);
}

void Tape::paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds)
{
	g.setColour(Colours::white);
	g.fillRect(thumbnailBounds);
	g.setColour(Colours::red);
	thumbnail.drawChannels(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 1.0f);
}
