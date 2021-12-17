/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class KaiKhorusAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::Button::Listener
{
public:
    KaiKhorusAudioProcessorEditor (KaiKhorusAudioProcessor&);
    ~KaiKhorusAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KaiKhorusAudioProcessor& audioProcessor;
    
    juce::Label frequencyLabel;
    juce::Label widthLabel;
    juce::Slider Width;
    juce::Slider Frequency;
    
    juce::Slider wetDry;
    juce::Label wetDryLabel;
    
    juce::TextButton oneSoundButton;
    juce::TextButton twoSoundButton;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KaiKhorusAudioProcessorEditor)
};
