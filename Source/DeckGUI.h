/*
  ==============================================================================

    DeckGui.h
    Created: 30 Jan 2024 8:38:45am
    Author:  ruby

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "CustomLookAndFeel.h"
using namespace juce;
//==============================================================================
/*
*/
class DeckGUI : public Component,
    public Button::Listener,
    public Slider::Listener,
    public FileDragAndDropTarget,
    public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse, Colour _colour);
    ~DeckGUI();

    void paint(Graphics&) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override;
   
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    Slider volSlider;

private:

    ImageButton playButton;
    ImageButton stopButton;
    ImageButton loopButton;
    ImageButton rewindButton;
    ImageButton forwardButton;

    TextButton loadButton{ "LOAD" };

    Slider speedSlider;
    Slider posSlider;
    Slider midSlider;
    Slider lowSlider;
    Slider highSlider;

    Colour themeColor;

    Label volLabel;
    Label speedLabel;
    Label highLabel;
    Label midLabel;
    Label lowLabel;
 
    FileChooser fChooser{ "Select a file..." };

    CustomLookAndFeel* customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};

