/*
  ==============================================================================

	DeckGui.cpp
	Created: 30 Jan 2024 8:38:45am
	Author:  ruby

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse, Colour _colour) : player(_player),
waveformDisplay(formatManagerToUse, cacheToUse), themeColor(_colour)
{
	// Create an instance of CustomLookAndFeel
	auto customLookAndFeel = new CustomLookAndFeel();
	
	auto playImage = ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
	auto pauseImage = ImageCache::getFromMemory(BinaryData::pause_png, BinaryData::pause_pngSize);
	auto stopImage = ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::stop_pngSize);
	auto loopImage = ImageCache::getFromMemory(BinaryData::loop_png, BinaryData::loop_pngSize);
	auto rewindImage = ImageCache::getFromMemory(BinaryData::previous_png, BinaryData::previous_pngSize);
	auto forwardImage = ImageCache::getFromMemory(BinaryData::next_png, BinaryData::next_pngSize);

	playButton.setImages(true, true, true, playImage, 1.0f, Colours::white, Image(nullptr), 1.0f, Colours::transparentWhite, pauseImage, 1.0f, Colours::transparentWhite);
	stopButton.setImages(true, true, true, stopImage, 1, Colours::white, Image(nullptr), 1, Colours::transparentWhite, stopImage, 1, Colours::transparentWhite);
	loopButton.setImages(true, true, true, loopImage, 1, Colours::white, Image(nullptr), 1, Colours::transparentWhite, loopImage, 1, Colours::transparentWhite);
	rewindButton.setImages(true, true, true, rewindImage, 1.0f, Colours::white, Image(nullptr), 1.0f, Colours::transparentWhite, rewindImage, 1.0f, Colours::transparentWhite);
	forwardButton.setImages(true, true, true, forwardImage, 1.0f, Colours::white, Image(nullptr), 1.0f, Colours::transparentWhite, forwardImage, 1.0f, Colours::transparentWhite);

	playButton.setClickingTogglesState(true);

	//buttons
	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);
	addAndMakeVisible(loadButton);
	addAndMakeVisible(loopButton);
	addAndMakeVisible(rewindButton);
	addAndMakeVisible(forwardButton);

	//sliders
	addAndMakeVisible(volSlider);
	addAndMakeVisible(speedSlider);
	addAndMakeVisible(posSlider);

	//band sliders
	addAndMakeVisible(lowSlider);
	addAndMakeVisible(midSlider);
	addAndMakeVisible(highSlider);

	//waveform 
	addAndMakeVisible(waveformDisplay);

	//event listener
	playButton.addListener(this);
	stopButton.addListener(this);
	loadButton.addListener(this);
	loopButton.addListener(this);
	rewindButton.addListener(this);
	forwardButton.addListener(this);

	volSlider.addListener(this);
	speedSlider.addListener(this);
	posSlider.addListener(this);

	lowSlider.addListener(this);
	midSlider.addListener(this);
	highSlider.addListener(this);

	//slider style
	volSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 75);
	volSlider.setColour(Slider::textBoxOutlineColourId, Colours::pink);

	posSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	posSlider.setName("posSlider");

	speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	speedSlider.setName("speedSlider");

	speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	lowSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	lowSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	midSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	midSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	highSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	highSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

	//range
	volSlider.setRange(0.0, 1.0, 0.1);
	posSlider.setRange(0.0, 1.0);
	speedSlider.setRange(0.1, 5.0, 0.1);

	lowSlider.setRange(0.01, 2);
	midSlider.setRange(0.01, 2);
	highSlider.setRange(0.01, 2);

	lowSlider.setValue(1);
	midSlider.setValue(1);
	highSlider.setValue(1);

	volSlider.setValue(1.0);

	//setup vol slider label
	addAndMakeVisible(volLabel);
	volLabel.setText("VOL", juce::dontSendNotification);
	volLabel.setJustificationType(Justification::centredTop);
	volLabel.attachToComponent(&volSlider, false);

	//setup speed slider label
	addAndMakeVisible(speedLabel);
	speedLabel.setText("SPD", juce::dontSendNotification);
	speedLabel.setJustificationType(Justification::centredTop);
	speedLabel.attachToComponent(&speedSlider, false);

	//setup high slider label
	addAndMakeVisible(highLabel);
	highLabel.setText("HIGH", juce::dontSendNotification);
	highLabel.setJustificationType(Justification::centredTop);
	highLabel.attachToComponent(&highSlider, false);

	//setup mid slider label
	addAndMakeVisible(midLabel);
	midLabel.setText("MID", juce::dontSendNotification);
	midLabel.setJustificationType(Justification::centredTop);
	midLabel.attachToComponent(&midSlider, false);
	
	//setup low slider label
	addAndMakeVisible(lowLabel);
	lowLabel.setText("LOW", juce::dontSendNotification);
	lowLabel.setJustificationType(Justification::centredTop);
	lowLabel.attachToComponent(&lowSlider, false);

	//no text box for position slider
	posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

	posSlider.setLookAndFeel(customLookAndFeel);
	volSlider.setLookAndFeel(customLookAndFeel);
	lowSlider.setLookAndFeel(customLookAndFeel);
	midSlider.setLookAndFeel(customLookAndFeel);
	highSlider.setLookAndFeel(customLookAndFeel);
	speedSlider.setLookAndFeel(customLookAndFeel);

	startTimer(100);

}

DeckGUI::~DeckGUI()
{
	stopTimer();
}

void DeckGUI::paint(Graphics& g)
{
	g.fillAll(themeColor); //background color
	g.setColour(Colours::black);
	g.setFont(14.0f);

	g.drawText("POSITION", getWidth() / 2 - 30, getHeight() -80, 80, 20,
		Justification::centred, true);

	//if file is loaded, calls the DJAudioPlayer to get the title and length left of the track that is currently playing
	if (player->trackLoaded)
	{
		g.setColour(Colours::white);
		g.setFont(16.0f);
		//display track title
		g.drawText("Track Title: " + player->trackPlayingTitle, getWidth() / 5 + 10, getHeight() / 5 + 15, 220, 15, Justification::left, true);
		//display track audio left
		g.drawText("Track Left: " + player->trackLeft(), getWidth() / 5 + 10, getHeight() / 5 + 35, 220, 20, Justification::left, true);
	}
}

void DeckGUI::resized()
{
	double rowH = getHeight() / 10;
	double colW = getWidth() / 12;

	double loadOffset = (themeColor == Colours::darkred) ? colW * 0.25 : colW * 10.0;
	double volXOffset = (themeColor == Colours::darkred) ? colW * 1.0 : colW * 10.0;
	double speedXOffset = (themeColor == Colours::darkred)? colW * 10.0 :colW * 1.0;
	double lowXOffset = (themeColor == Colours::darkred) ? colW * 10.0 : colW * 1.0;
	double midXOffset = (themeColor == Colours::darkred) ? colW * 10.0 : colW * 1.0;
	double highXOffset = (themeColor == Colours::darkred) ? colW * 10.0 : colW * 1.0;

	waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
	rewindButton.setBounds(colW * 4.5, rowH * 8.7,colW * 1.5, rowH);
	playButton.setBounds(colW * 5.5, rowH * 8.7, colW * 1.5, rowH);
	forwardButton.setBounds(colW * 6.5, rowH * 8.7, colW * 1.5, rowH);
	stopButton.setBounds(colW * 3.5, rowH * 8.7, colW * 1.5, rowH);
	loopButton.setBounds(colW * 2.7, rowH * 8.7, colW * 0.6, rowH);
	loadButton.setBounds(loadOffset, rowH * 2, colW * 2.0, rowH);

	volSlider.setBounds(volXOffset, rowH * 4.0,  colW * 0.80, rowH * 4.0);
	speedSlider.setBounds(speedXOffset, rowH * 3.0,  colW * 1.0, rowH * 1.0);
	lowSlider.setBounds(lowXOffset, rowH * 4.7, colW * 1.0, rowH * 1.0);
	midSlider.setBounds(midXOffset, rowH * 6.4, colW * 1.0, rowH * 1.0);
	highSlider.setBounds(highXOffset, rowH * 8.1, colW * 1.0, rowH * 1.0);
	posSlider.setBounds(colW * 4.0, rowH * 3.5, colW * 5.0, rowH * 5.0);

}

void DeckGUI::buttonClicked(Button* button)
{
	if (button == &playButton)
	{
		std::cout << "Play button was clicked " << std::endl;
		if (button->getToggleState())
			player->start();
		else
			player->stop();
	}
	if (button == &stopButton)
	{
		std::cout << "Stop button was clicked " << std::endl;
		player->stop();
		player->setPosition(0);
		//reset the playbutton to playImage
		playButton.setToggleState(false, juce::dontSendNotification);

	}

	if (button == &loopButton)
	{
		repaint();
		player->toggleLoop();

		//set the loop button colours according to the loop state
		if (player->isLooping())
		{
			std::cout << "looping" << std::endl;
			loopButton.setToggleState(true, juce::dontSendNotification);
		}
		else
		{
			loopButton.setToggleState(false, juce::dontSendNotification);
		}
	}

	if (button == &loadButton)
	{
		auto fileChooserFlags =
			FileBrowserComponent::canSelectFiles;
		fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
			{
				auto chosenFile = chooser.getResult();
				player->loadURL(URL{ chosenFile });
				waveformDisplay.loadURL(URL{ chosenFile });

			});
	}

	if (button == &rewindButton)
	{
		player->rewind();
	}

	if (button == &forwardButton)
	{
		player->forward();
	}
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
	if (slider == &volSlider)
	{
		player->setGain(slider->getValue());
	}

	if (slider == &speedSlider)
	{
		player->setSpeed(slider->getValue());
	}

	if (slider == &posSlider)
	{
		player->setPositionRelative(slider->getValue());
	}
	if (slider == &lowSlider) {
		DBG("MainComponent::sliderValueChanged: low slider " << slider->getValue());
		player->setLBFilter(slider->getValue());
	}

	if (slider == &midSlider) {
		DBG("MainComponent::sliderValueChanged: Mid slider " << slider->getValue());
		player->setMBFilter(slider->getValue());
	}

	if (slider == &highSlider) {
		DBG("MainComponent::sliderValueChanged: High slider " << slider->getValue());
		player->setHBFilter(slider->getValue());
	}
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
	std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
	return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
	std::cout << "DeckGUI::filesDropped" << std::endl;
	if (files.size() == 1)
	{
		player->loadURL(URL{ File{files[0]} });
	}
}

void DeckGUI::timerCallback()
{

	waveformDisplay.setPositionRelative(player->getPositionRelative());
	double pos = player->getPositionRelative();
	if (pos >= 0.0) {
		posSlider.setValue(pos);
	}

	if (player->isPlaying())
	{
		repaint();
	}
	//if the pos reaches the end of the track amd the player isPlaying,
	if (pos >= 1.0 && player->isPlaying() && player->isLooping())
	{
		std::cout << "looping" << std::endl;
		player->setPosition(0);
		player->start();
	}
	//if the pos reaches the end of the track, the player loop state is false, and the playbutton state is true
	if (pos >= 1.0 && !player->isLooping() && playButton.getToggleState())
	{
		playButton.setToggleState(false, juce::dontSendNotification);
		
	}
}


