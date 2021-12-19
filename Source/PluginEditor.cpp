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

/*
    addAndMakeVisible(&Width);
    Width.setValue(3.69f);//This is milliseconds
    Width.setRange(0.0f, audioProcessor.maxBufferDelay, 0.01f);
    //audioProcessor.width = 3.69f;
    Width.onValueChange = [this] {
        //audioProcessor.width= Width.getValue()/1000; //to get ms
    };
    
    addAndMakeVisible(widthLabel);
    widthLabel.setText("Width", juce::dontSendNotification);
    widthLabel.attachToComponent(&Width, true);
    
    addAndMakeVisible(&Frequency);
    Frequency.setValue(0.513f);
    Frequency.setRange(0.0f, 10.0f, 0.001f);
    //audioProcessor.frequency = 0.513f;
    Frequency.onValueChange = [this] {
       // audioProcessor.frequency= Frequency.getValue();
    };

    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    frequencyLabel.attachToComponent(&Frequency, true);
    
    */
    addAndMakeVisible(&wetDry);
    wetDry.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    //wetDry.setValue(1.0f);
    //wetDry.setRange(0.0f, 1.0f, 0.001f);
    //audioProcessor.wetDry = 1.0f;
    //wetDry.onValueChange = [this] {
        //audioProcessor.wetDry = wetDry.getValue();
    //};

    addAndMakeVisible(wetDryLabel);
    wetDryLabel.setText("Wet Dry", juce::dontSendNotification);
    wetDryLabel.attachToComponent(&wetDry, true);
    
    
    
    addAndMakeVisible(oneButton);
    //oneButton.addListener(this);
    //oneButton.setToggleable(true);
    oneButton.setClickingTogglesState(true);
    oneButton.setButtonText("1st Sound");
    
    addAndMakeVisible(twoButton);
    //twoButton.addListener(this);
    //twoButton.setToggleable(true);
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
    Width.setBounds(50, 50, 200, 50);
    Frequency.setBounds(50, 150, 200, 50);
    wetDry.setBounds(50, 250, 200, 50);
    oneButton.setBounds(50, 300, 50, 50);
    twoButton.setBounds(100, 300, 50, 50);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


