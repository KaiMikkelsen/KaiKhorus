#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "myLookAndFeel.h"

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
    myLookAndFeel myLookAndFeelV1;
    void initializeButton(juce::TextButton *button);
    void activateLedState(juce::Button* button);
    
    juce::Slider mix;
    juce::Label mixLabel;
    juce::Label titleLabel;
    
    juce::TextButton oneButton;
    juce::TextButton twoButton;
    
    juce::Image ledOff;
    juce::Image ledOn;
    
    
    juce::ImageComponent oneButtonLED;
    juce::ImageComponent twoButtonLED;
    
    juce::Colour buttonOnColour;
    juce::Colour buttonOffColour;
 
    
public://Bottom Up destruction, need this destroyed before the actual slider itself, see https://www.youtube.com/watch?v=NE8d91yYBJ8&ab_channel=TheAudioProgrammer
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> mixValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> buttonOne;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> buttonTwo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KaiKhorusAudioProcessorEditor)
};
