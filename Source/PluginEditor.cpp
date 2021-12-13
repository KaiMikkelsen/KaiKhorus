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
    
    
    
    addAndMakeVisible(&Width);
    Width.setValue(1.0f);//This is milliseconds
    Width.setRange(0.0f, 2.0f, 0.1f);
    Width.onValueChange = [this] {
        audioProcessor.width= Width.getValue();
    };
    
    addAndMakeVisible(widthLabel);
    widthLabel.setText("Width", juce::dontSendNotification);
    widthLabel.attachToComponent(&Width, true);
    
    addAndMakeVisible(&Frequency);
    Frequency.setValue(1.0f);
    Frequency.setRange(0.0f, 10.0f, 1.0f);
    
    Frequency.onValueChange = [this] {
        audioProcessor.frequency= Frequency.getValue();
    };

    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    frequencyLabel.attachToComponent(&Frequency, true);
    
    
    
    setSize (400, 300);
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
    g.drawFittedText ("Kais Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void KaiKhorusAudioProcessorEditor::resized()
{
    Width.setBounds(50, 50, 200, 50);
    Frequency.setBounds(50, 150, 200, 50);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
