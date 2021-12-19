/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KaiKhorusAudioProcessorEditor::KaiKhorusAudioProcessorEditor (KaiKhorusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    wetDryValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "wetDry", wetDry);
    buttonOne = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "buttonOne", oneButton);
    buttonTwo = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "buttonTwo", twoButton);

    addAndMakeVisible(&wetDry);
    wetDry.setSliderStyle(juce::Slider::SliderStyle::Rotary);

    addAndMakeVisible(wetDryLabel);
    wetDryLabel.setText("Wet Dry", juce::dontSendNotification);
    wetDryLabel.attachToComponent(&wetDry, true);
    
    
    addAndMakeVisible(oneButton);
    oneButton.setClickingTogglesState(true);
    oneButton.setButtonText("1st Sound");
    
    addAndMakeVisible(twoButton);
    twoButton.setClickingTogglesState(true);
    twoButton.setButtonText("2nd Sound");
    

    setSize (400, 400);
}

KaiKhorusAudioProcessorEditor::~KaiKhorusAudioProcessorEditor()
{
}

//==============================================================================
void KaiKhorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Kais Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void KaiKhorusAudioProcessorEditor::resized()
{

    wetDry.setBounds(50, 250, 200, 50);
    oneButton.setBounds(50, 300, 50, 50);
    twoButton.setBounds(100, 300, 50, 50);

}


