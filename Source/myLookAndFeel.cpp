#include "myLookAndFeel.h"

//==============================================================================
myLookAndFeel::myLookAndFeel()
{
    sliderImage = juce::ImageCache::getFromMemory(BinaryData::bluey_png, BinaryData::bluey_pngSize);
}

//==============================================================================
void myLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height, float sliderPos,
                                       float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{

    if (sliderImage.isValid())
    {
        const double rotation = (slider.getValue()
            - slider.getMinimum())
            / (slider.getMaximum()
                - slider.getMinimum());

        const int frames = sliderImage.getHeight() / sliderImage.getWidth();
        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
        const float radius = fmin(width / 2.0f, height / 2.0f);
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius - 1.0f;
        const float ry = centerY - radius;

        g.drawImage(sliderImage,
            (int)rx,
            (int)ry,
            2 * (int)radius,
            2 * (int)radius,
            0,
            frameId*sliderImage.getWidth(),
                    sliderImage.getWidth(),
                    sliderImage.getWidth());
    }
    else
    {
        static const float textPpercent = 0.35f;
        juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f,
            0.5f * height, width * textPpercent, 0.5f * height);

        g.setColour(juce::Colours::white);

        g.drawFittedText(juce::String("No Image"), text_bounds.getSmallestIntegerContainer(),
            juce::Justification::horizontallyCentred | juce::Justification::centred, 1);
    }
}

void myLookAndFeel::drawButtonText (juce::Graphics& g, juce::TextButton& button,
                                     bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
{
    
    juce::Font font = (juce::Font (juce::Typeface::createSystemTypefaceFor (BinaryData::AstroSpaceeZ2Bg_ttf, BinaryData::AstroSpaceeZ2Bg_ttfSize)));
    font.setHeight(24.0f);
    g.setFont (font);
    g.setColour (button.findColour (button.getToggleState() ? juce::TextButton::textColourOnId
                                                            : juce::TextButton::textColourOffId)
                       .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

    const int yIndent = fmin (4, button.proportionOfHeight (0.3f));
    const int cornerSize = fmin (button.getHeight(), button.getWidth()) / 2;

    const int fontHeight = juce::roundToInt (font.getHeight() * 0.6f);
    const int leftIndent  = fmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = fmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    const int textWidth = button.getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText (button.getButtonText(),
                          leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                          juce::Justification::centred, 2);
}


