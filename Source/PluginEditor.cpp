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
    wetDry.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    

    addAndMakeVisible(wetDryLabel);
    wetDryLabel.setText("Mix", juce::dontSendNotification);
    wetDryLabel.setJustificationType (juce::Justification::centred);
   // wetDryLabel.setColour (juce::Label::backgroundColourId, juce::Colours::lightgreen);
    
    
    
    addAndMakeVisible(oneButton);
    oneButton.setClickingTogglesState(true);
    oneButton.setButtonText("I");
    
    addAndMakeVisible(twoButton);
    twoButton.setClickingTogglesState(true);
    twoButton.setButtonText("II");
    

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
    int sliderSize = 200;
    int labelWidth = 100;
    int labelHeight = 50;
    
    int buttonWidth = 100;
    int buttonHeight = 75;

    wetDry.setBounds(getWidth()/2 - sliderSize/2, getHeight()/3 - sliderSize/2, sliderSize, sliderSize);
    wetDryLabel.setBounds(getWidth()/2 - labelWidth/2, getHeight()/2, labelWidth, labelHeight);
    
    
    oneButton.setBounds(getWidth()/2 - 100, 3* getHeight()/4.5, buttonWidth, buttonHeight);
    twoButton.setBounds(getWidth()/2, 3* getHeight()/4.5, buttonWidth, buttonHeight);

}


