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
    
    
    
    addAndMakeVisible(&Feedback);
    Feedback.setValue(0.8f);
    Feedback.setRange(0.0f, 1.0f, 0.1);
    
    addAndMakeVisible(&Time);
    Time.setValue(300.0f);
    Time.setRange(0.0f, 2000.0f, 1);
    
    Feedback.onValueChange = [this] {
        audioProcessor.feedback= Feedback.getValue();
    };
    
    Time.onValueChange = [this] {
        audioProcessor.time= Time.getValue();
    };

    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent(&Feedback, true);
    
    
    addAndMakeVisible(timeLabel);
    timeLabel.setText("Time", juce::dontSendNotification);
    timeLabel.attachToComponent(&Time, true);
    
    
    
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
    Time.setBounds(50, 50, 200, 50);
    Feedback.setBounds(50, 150, 200, 50);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
