/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 14 Feb 2024 9:53:45pm
    Author:  ruby

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel()
{
 
}

CustomLookAndFeel::~CustomLookAndFeel()
{

}

void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const Slider::SliderStyle style, Slider& slider)
{
    if (style == Slider::SliderStyle::LinearBarVertical)
    {
            // Draw the track
            auto trackWidth = width;
            auto trackBounds = Rectangle<float>(x, y, trackWidth, height);

            // gradient colors for the track
            ColourGradient gradient(Colour(0xff00ff00), trackBounds.getTopLeft(),
                Colour(0xffff0000), trackBounds.getBottomLeft(), false);
            gradient.addColour(0.5, Colours::yellow); // Yellow in the middle

            // Fill the track with the gradient
            g.setGradientFill(gradient);
            g.fillRect(trackBounds);

            // Draw the thumb as a rounded rectangle
            auto thumbWidth = trackBounds.getWidth() * 1.5f;
            auto thumbHeight = 10;
            auto thumbBounds = Rectangle<float>(trackBounds.getX() + trackBounds.getWidth() * 0.5f - thumbWidth * 0.5f,
                sliderPos - thumbHeight * 0.5f, thumbWidth, thumbHeight);

            // Draw the thumb with a solid color
            g.setColour(Colours::grey);
            g.fillRoundedRectangle(thumbBounds, 4.0f);

            // Draw the outline of the thumb
            g.setColour(Colours::white);
            g.drawRoundedRectangle(thumbBounds, 10.0f, 5.0f); 

            // Fill the remaining space above the track with grey
            auto remainingBounds = Rectangle<float>(x, y, trackWidth, thumbBounds.getY());
            g.setColour(Colours::grey);
            g.fillRect(remainingBounds);
    }
    else
    {
        // Fallback to the default LookAndFeel
        LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

void CustomLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
    Slider& slider)
{
    auto radius = jmin(width / 2, height / 2) - 4.0f;
    auto centreX = x + width * 0.5f;
    auto centreY = y + height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Draw the background ellipse
    g.setColour(Colours::black);
    g.fillEllipse(rx, ry, rw, rw);

    if (slider.getName() == "posSlider")
    {
        // Draw the disc image for the "posSlider"
        auto originalImage = ImageCache::getFromMemory(BinaryData::disc_png, BinaryData::disc_pngSize);
        int newWidth = originalImage.getWidth();
        int newHeight = originalImage.getHeight();
        auto resizedImage = originalImage.rescaled(newWidth-100, newHeight-100, Graphics::highResamplingQuality);
        auto imageX = centreX - resizedImage.getWidth() / 2;
        auto imageY = centreY - resizedImage.getHeight() / 2;
        g.drawImageAt(resizedImage, imageX, imageY);
        g.setColour(Colours::white);
    }
    else {

        auto originalImage = ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
        int newWidth = originalImage.getWidth();
        int newHeight = originalImage.getHeight();
        auto resizedImage = originalImage.rescaled(newWidth - 80, newHeight - 80, Graphics::highResamplingQuality);
        auto imageX = centreX - resizedImage.getWidth() / 2;
        auto imageY = centreY - resizedImage.getHeight() / 2;
        g.drawImageAt(resizedImage, imageX, imageY);
        g.setColour(Colours::darkgoldenrod);
        
    }
    // Draw the pointer
    Path pointer;
    auto pointerLength = radius * 0.53f;
    auto pointerThickness = 2.5f;
    pointer.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    pointer.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
    g.fillPath(pointer);
}

