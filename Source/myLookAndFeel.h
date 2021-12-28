#pragma once
#include <JuceHeader.h>

//==============================================================================
class myLookAndFeel : public juce::LookAndFeel_V4
{
public:
    myLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawButtonText (juce::Graphics&, juce::TextButton&,
                         bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
private:
    juce::Image sliderImage;

};


