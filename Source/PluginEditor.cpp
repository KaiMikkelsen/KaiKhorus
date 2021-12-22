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
    //wetDry.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    wetDry.setLookAndFeel(&myLookAndFeelV1);
    wetDry.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    wetDry.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    

    //addAndMakeVisible(wetDryLabel);
    wetDryLabel.setText("Mix", juce::dontSendNotification);
    wetDryLabel.setJustificationType (juce::Justification::centred);
   // wetDryLabel.setColour (juce::Label::backgroundColourId, juce::Colours::lightgreen);
    
    /*
    addAndMakeVisible(myKnobSlider1);
    myKnobSlider1.setLookAndFeel(&myLookAndFeelV1);
    myKnobSlider1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myKnobSlider1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    */
     
    addAndMakeVisible(oneButton);
    oneButton.setClickingTogglesState(true);
    oneButton.setButtonText("I");
    oneButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(255, 209, 59, 255));
    oneButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGBA(145, 112, 0, 100));
    
    //oneButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(198, 163, 77, 200));
    
    addAndMakeVisible(twoButton);
    twoButton.setClickingTogglesState(true);
    twoButton.setButtonText("II");
    twoButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(255, 209, 59, 255));
    //twoButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(215, 126, 42, 200));
    
    
    
    setSize (400, 400);
}

KaiKhorusAudioProcessorEditor::~KaiKhorusAudioProcessorEditor()
{
}

//==============================================================================
void KaiKhorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.fillAll (juce::Colours::lightgreen);
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::SpaceBackground_jpeg, BinaryData::SpaceBackground_jpegSize);
    g.drawImageAt (background, 0, 0);
    
    
    juce::Image led = juce::ImageCache::getFromMemory(BinaryData::LED_png, BinaryData::LED_pngSize);
    //juce::GlowEffect glow;
    
    //glow.setGlowProperties(100, juce::Colours::red);
    //g.setOrigin(50,50);
    //glow.applyEffect(led, g, 50.0f, 1.0f);
    
    g.drawImageAt (led, getWidth()/2 - 50, 3* getHeight()/4 - 50);
    
   // glow.applyEffect(led, g, 1.0f, 1.0f);
    
    
   
    

    
    g.setColour (juce::Colours::red);
    //g.fillEllipse (getWidth()/2 - 50, 3* getHeight()/4 - 50 , 8, 8);
    

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
  
    
    
    //g.drawFittedText ("Kais Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void KaiKhorusAudioProcessorEditor::buttonClicked(juce::Button* button) override
{
    
    if(button == &oneButton)
    {
        
        DBG("button 2");
    }
    else
    {
        
        DBG("button 2");
    }
    
}

void KaiKhorusAudioProcessorEditor::resized()
{
    int sliderSize = 150;
    int labelWidth = 100;
    int labelHeight = 50;
    
    int buttonWidth = 100;
    int buttonHeight = 75;

    wetDry.setBounds(getWidth()/2 - sliderSize/2, getHeight()/2.5 - sliderSize/2, sliderSize, sliderSize);
    wetDryLabel.setBounds(getWidth()/2 - labelWidth/2, getHeight()/2, labelWidth, labelHeight);
    
    
    oneButton.setBounds(getWidth()/2 - 100, 3* getHeight()/4, buttonWidth, buttonHeight);
    twoButton.setBounds(getWidth()/2, 3* getHeight()/4, buttonWidth, buttonHeight);
    
    myKnobSlider1.setBounds(0, 0, 200, 200);

}


