/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 30 Jan 2024 9:32:24am
    Author:  ruby

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse) : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll(Colour::fromRGB(25, 25, 26));

    g.setColour(Colour::fromRGB(0, 0, 0));
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colour::fromRGBA(5, 177, 255, 150));
    if (fileLoaded)
    {
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f
        );
        g.setColour(Colours::hotpink);
        g.drawRect(position * getWidth(), 0, 1, getHeight());

        g.setColour(Colour::fromRGBA(128, 128, 128, 100));
        g.fillRect(position * getWidth(), 0, getWidth() - position * getWidth(), getHeight());
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("WaveformDisplay", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
  
}

void WaveformDisplay::loadURL(URL audioURL)
{
    std::cout << "Load URL" << std::endl;
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::cout << "File loaded" << std::endl;
        repaint();
    }
    else
    {
        std::cout << "Not loaded" << std::endl;
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position && !isnan(pos)) {
        position = pos;
        repaint();
    }
}