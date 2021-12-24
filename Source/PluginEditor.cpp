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

    addAndMakeVisible(titleLabel);
    titleLabel.setText("Space Octopus Chorus", juce::dontSendNotification);
    //titleLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    
    
    
    juce::Font mixFont = (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::Micra_Bold_ttf, BinaryData::Micra_Bold_ttfSize)));
    juce::Font titleFont = (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::Micra_Bold_ttf, BinaryData::Micra_Bold_ttfSize)));
    

    //juce::Font font = (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::AstroSpaceeZ2Bg_ttf, BinaryData::AstroSpaceeZ2Bg_ttfSize)));
    
    
    mixFont.setHeight(17.0f);
    titleFont.setHeight(24.0f);
    titleLabel.setFont(titleFont);
    
    
    
    addAndMakeVisible(wetDryLabel);
    wetDryLabel.setFont(mixFont);
    wetDryLabel.setText("MIX", juce::dontSendNotification);
    wetDryLabel.setJustificationType (juce::Justification::centred);
    //wetDryLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    
    
    
    
    
    addAndMakeVisible(&wetDry);
    //wetDry.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    wetDry.setLookAndFeel(&myLookAndFeelV1);
    wetDry.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    wetDry.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    

    //wetDryLabel.setFont (juce::Font (18.0f, juce::Font::bold));
    //wetDryLabel.setFont(font);
   //
   // wetDryLabel.setColour (juce::Label::backgroundColourId, juce::Colours::lightgreen);
    
    /*
    addAndMakeVisible(myKnobSlider1);
    myKnobSlider1.setLookAndFeel(&myLookAndFeelV1);
    myKnobSlider1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myKnobSlider1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    */
     
    addAndMakeVisible(oneButton);
    
    oneButton.setLookAndFeel (&myLookAndFeelV1);
    
    oneButton.setClickingTogglesState(true);
    oneButton.setButtonText("I");
    oneButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(255, 209, 59, 255));
    oneButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGBA(242, 177, 39, 255));
    
    oneButton.addListener (this);
    
    
    //oneButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(198, 163, 77, 200));
    
    twoButton.setLookAndFeel (&myLookAndFeelV1);
    
    addAndMakeVisible(twoButton);
    twoButton.setClickingTogglesState(true);
    twoButton.setButtonText("II");
    twoButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(255, 209, 59, 255));
    twoButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGBA(242, 177, 39, 255));
    //twoButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(215, 126, 42, 200));
    
    twoButton.addListener (this);
    
    
    
    //led = juce::ImageCache::getFromMemory(BinaryData::LED_png, BinaryData::LED_pngSize);
    //ledOn = juce::ImageCache::getFromMemory(BinaryData::ButtonOn_png, BinaryData::ButtonOn_pngSize);
    led = juce::ImageCache::getFromMemory(BinaryData::LEDOff_png, BinaryData::LEDOff_pngSize);
    ledOn = juce::ImageCache::getFromMemory(BinaryData::LEDon_png, BinaryData::LEDon_pngSize);
    
    oneButtonLED.setImage(led);
    twoButtonLED.setImage(led);
    
    addAndMakeVisible(oneButtonLED);
    addAndMakeVisible(twoButtonLED);
    testWidth = 12;
    
    activateLed(&oneButton);
    activateLed(&twoButton);
    
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
    
    
    //juce::Image led = juce::ImageCache::getFromMemory(BinaryData::LED_png, BinaryData::LED_pngSize);
    //juce::GlowEffect glow;
    
    //glow.setGlowProperties(100, juce::Colours::red);
    //g.setOrigin(50,50);
    //glow.applyEffect(led, g, 50.0f, 1.0f);
    
    //g.drawImageAt (led, getWidth()/2 - 50, 3* getHeight()/4 - 50);
    
   // glow.applyEffect(led, g, 1.0f, 1.0f);
    
    
   
    

    
    g.setColour (juce::Colours::red);
    //g.fillEllipse (getWidth()/2 - 50, 3* getHeight()/4 - 50 , 8, 8);
    

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
  
    
    
    //g.drawFittedText ("Kais Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void KaiKhorusAudioProcessorEditor::activateLed(juce::Button* button)
{
    
      int state;
      if(button == &oneButton)
      {
          state = oneButton.getToggleState();
          if(state == 1)
          {
              
              oneButtonLED.setImage(ledOn);
             
              oneButtonLED.setBounds(getWidth()/2 - 56, 3* getHeight()/4 - 20, testWidth, testWidth);
              oneButtonLED.resized();
          
          }
          else if(state == 0)
          {
                     
              oneButtonLED.setImage(led);
              oneButtonLED.setBounds(getWidth()/2 - 56, 3* getHeight()/4 - 20, testWidth, testWidth);
              oneButtonLED.resized();
              
              
          }
          
          
      }
  
      else if(button == &twoButton)
      {
          state = twoButton.getToggleState();
          if(state == 1)
          {
              
              twoButtonLED.setImage(ledOn);
             
              twoButtonLED.setBounds(getWidth()/2 + 44, 3* getHeight()/4 - 20, testWidth, testWidth);
              twoButtonLED.resized();
          
          }
          else if(state == 0)
          {
                     
              twoButtonLED.setImage(led);
              twoButtonLED.setBounds(getWidth()/2 + 44, 3* getHeight()/4 - 20, testWidth, testWidth);
              twoButtonLED.resized();
              
              
          }
          
          
      }
    
    
    
}

void KaiKhorusAudioProcessorEditor::buttonClicked(juce::Button* button)
{

    activateLed(button);
     
     
}

void KaiKhorusAudioProcessorEditor::resized()
{
    int sliderSize = 150;
    int labelWidth = 100;
    int labelHeight = 50;
    
    int buttonWidth = 100;
    int buttonHeight = 75;

    titleLabel.setBounds(0, 0, 400, 25);
    
    
    wetDry.setBounds(getWidth()/2 - sliderSize/2, getHeight()/2.75 - sliderSize/2, sliderSize, sliderSize);
    wetDryLabel.setBounds(getWidth()/2 - labelWidth/2, getHeight()/2 + 4, labelWidth, labelHeight);
    
    
    oneButton.setBounds(getWidth()/2 - 100, 3* getHeight()/4, buttonWidth, buttonHeight);
    twoButton.setBounds(getWidth()/2, 3* getHeight()/4, buttonWidth, buttonHeight);
    
    //myKnobSlider1.setBounds(0, 0, 200, 200);
    
    oneButtonLED.setBounds(getWidth()/2 - 56, 3* getHeight()/4 - 20, testWidth, testWidth);
    twoButtonLED.setBounds(getWidth()/2 + 44, 3* getHeight()/4 - 20, testWidth, testWidth);
    
    

}


