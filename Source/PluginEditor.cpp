
#include "PluginProcessor.h"
#include "PluginEditor.h"


KaiKhorusAudioProcessorEditor::KaiKhorusAudioProcessorEditor (KaiKhorusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    mixValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mix", mix);
    buttonOne = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "buttonOne", oneButton);
    buttonTwo = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "buttonTwo", twoButton);

    juce::Font mixFont = juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::Micra_Bold_ttf, BinaryData::Micra_Bold_ttfSize));
    mixFont.setHeight(17.0f);
    juce::Font titleFont = juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::Micra_Bold_ttf, BinaryData::Micra_Bold_ttfSize));
    titleFont.setHeight(28.0f);
   
    
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Space Octopus Chorus", juce::dontSendNotification);
    titleLabel.setFont(titleFont);
    //titleLabel.setColour((juce::Label::ColourIds::textColourId), juce::Colour(118, 0, 182));
    
    addAndMakeVisible(&mix);
    mix.setLookAndFeel(&myLookAndFeelV1);
    mix.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mix.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    addAndMakeVisible(mixLabel);
    mixLabel.setFont(mixFont);
    mixLabel.setText("MIX", juce::dontSendNotification);
    mixLabel.setJustificationType (juce::Justification::centred);

    initializeButton(&oneButton);
    oneButton.setButtonText("I");
    
    initializeButton(&twoButton);
    twoButton.setButtonText("II");
     
    ledOff = juce::ImageCache::getFromMemory(BinaryData::LEDOff_png, BinaryData::LEDOff_pngSize);
    ledOn = juce::ImageCache::getFromMemory(BinaryData::LEDon_png, BinaryData::LEDon_pngSize);
    
    addAndMakeVisible(oneButtonLED);
    oneButtonLED.setImage(ledOff);
    activateLedState(&oneButton);
    
    addAndMakeVisible(twoButtonLED);
    twoButtonLED.setImage(ledOff);
    activateLedState(&twoButton);
    
    setSize (400, 400);
}

KaiKhorusAudioProcessorEditor::~KaiKhorusAudioProcessorEditor()
{
}

//==============================================================================
void KaiKhorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    //juce::Image background = juce::ImageCache::getFromMemory(BinaryData::SpaceBackground_jpeg, BinaryData::SpaceBackground_jpegSize);
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::octopus2_jpeg, BinaryData::octopus2_jpegSize);
    g.drawImageAt (background, 0, 0);
}

void KaiKhorusAudioProcessorEditor::activateLedState(juce::Button* button)
{
      int state;
      if(button == &oneButton)
      {
          state = oneButton.getToggleState();
          if(state == 1)
          {
              oneButtonLED.setImage(ledOn);
          }
          else if(state == 0)
          {
              oneButtonLED.setImage(ledOff);
          }
      }
  
      else if(button == &twoButton)
      {
          state = twoButton.getToggleState();
          if(state == 1)
          {
              twoButtonLED.setImage(ledOn);
          }
          else if(state == 0)
          {
              twoButtonLED.setImage(ledOff);
          }
      }
}

void KaiKhorusAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    activateLedState(button);
}

void KaiKhorusAudioProcessorEditor::resized()
{
    int sliderSize = 150;
    int labelWidth = 100;
    int labelHeight = 50;
    
    int buttonWidth = 100;
    int buttonHeight = 75;
    int ledWidth = 12;
    //Because there isnt many components these bounds are a bit specific, they could use some refactoring in the future to make them easier to understand
    titleLabel.setBounds(0, 5, 400, 25);
    
    mix.setBounds(getWidth()/2 - sliderSize/2, getHeight()/2.75 - sliderSize/2 + 10, sliderSize, sliderSize);
    mixLabel.setBounds(getWidth()/2 - labelWidth/2, getHeight()/2 + 14, labelWidth, labelHeight);
    
    oneButton.setBounds(getWidth()/2 - buttonWidth, 3 * getHeight()/4, buttonWidth, buttonHeight);
    twoButton.setBounds(getWidth()/2, 3 * getHeight()/4, buttonWidth, buttonHeight);
    
    oneButtonLED.setBounds(getWidth()/2 - 56, 3 * getHeight()/4 - 20, ledWidth, ledWidth);
    twoButtonLED.setBounds(getWidth()/2 + 44, 3 * getHeight()/4 - 20, ledWidth, ledWidth);
}

void KaiKhorusAudioProcessorEditor::initializeButton(juce::TextButton *button)
{
    buttonOffColour = juce::Colour::fromRGBA(255, 209, 59, 255);
    buttonOnColour = juce::Colour::fromRGBA(242, 177, 39, 255);
    
    addAndMakeVisible(button);
    button->setLookAndFeel (&myLookAndFeelV1);
    button->setClickingTogglesState(true);
    button->setColour(juce::TextButton::ColourIds::buttonColourId, buttonOffColour);
    button->setColour(juce::TextButton::ColourIds::buttonOnColourId, buttonOnColour);
    button->addListener (this);
}
