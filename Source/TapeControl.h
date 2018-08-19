/*
  ==============================================================================

    TapeControl.h
    Created: 19 Aug 2018 12:21:05am
    Author:  Isaac Bries

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class TapeControl    : public Component
{
public:
    TapeControl();
    ~TapeControl();

    void paint (Graphics&) override;
    void resized() override;

	void setButtonState(String button, bool state);
	void setSliderRange(String slider, int min, int max);
	void setSliderValue(String slider, int val);

	int getStartSample();
	int getEndSample();

	Button* getButtonPtr(String button);
	Slider* getSliderPtr(String slider);

private:

	TextButton openButton;
	TextButton playButton;
	TextButton stopButton;
	TextButton clearButton;
	Slider startSampleSlider;
	Slider endSampleSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeControl)
};
