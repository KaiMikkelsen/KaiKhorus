//
//  myLookAndFeel.h
//  KaiKhorus
//
//  Created by Kai Mikkelsen on 2021-12-20.
//

/*
  ==============================================================================

    Created: 14 Dec 2017 10:16:04am
    Author:  Stefan Remberg

  ==============================================================================
*/

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
    juce::Image img1;

};


