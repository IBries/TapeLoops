/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TapeDeck.h"

//==============================================================================
class MainComponent : public AudioAppComponent
{
public:

    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

//==============================================================================
private:
	const int NUM_TAPES = 3;
	const int TAPE_HEIGHT = 240;

	TapeDeck* tapeDecks;
	AudioBuffer<float>* buffers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
