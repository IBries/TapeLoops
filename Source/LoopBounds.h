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
class LoopBounds    : public Component
{
public:
    LoopBounds();
    ~LoopBounds();

    void paint (Graphics&) override;
    void resized() override;
	void clear();

	void setMaxLength(int lengthInSamples);
	int getMaxLength();
	void setStartSample(int sample);
	int getStartSample();
	void setEndSample(int sample);
	int getEndSample();
	void setStartDrawPosition(float pos);
	void setEndDrawPosition(float pos);

private:
	void drawLoopStartIndicator(Graphics& g);
	void drawLoopEndIndicator(Graphics& g);
	void drawFill(Graphics& g);

	int startSample;
	float startDrawPosition;
	int endSample;
	float endDrawPosition;
	int maxLength;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopBounds)
};
