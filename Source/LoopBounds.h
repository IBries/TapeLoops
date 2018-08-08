/*
  ==============================================================================

    LoopBounds.h
    Created: 2 Aug 2018 10:56:03pm
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class LoopBounds    : public Component,
					  public Slider::Listener
{
public:
    LoopBounds(Slider &start, Slider &end);
    ~LoopBounds();

    void paint (Graphics&) override;
    void resized() override;
	void clear();

	void sliderValueChanged(Slider* slider);
	void setMaxLength(int lengthInSamples);

private:
	void startSampleChanged(Slider* slider);
	void endSampleChanged(Slider* slider);
	void drawLoopStartIndicator(Graphics& g);
	void drawLoopEndIndicator(Graphics& g);
	void drawFill(Graphics& g);

	int startSample;
	float startDrawPosition;
	int endSample;
	float endDrawPosition;
	int maxLength;

	Slider* startSampleSlider;
	Slider* endSampleSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopBounds)
};
