#include "MainComponent.h"

using namespace std;
//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(1220, 850);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
        && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
            [&](bool granted) { if (granted)  setAudioChannels(2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

        addAndMakeVisible(deckGUI1);
        addAndMakeVisible(deckGUI2);
        addAndMakeVisible(playlistComponent);
        addAndMakeVisible(crossfadeSlider);
     
        crossfadeSlider.addListener(this);
        crossfadeSlider.setRange(0.0, 1.0);
        crossfadeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        crossfadeSlider.setValue(0.5);

        addAndMakeVisible(crossFade);
        crossFade.setText("crossFade", juce::dontSendNotification);
        crossFade.setJustificationType(Justification::centredTop);
        crossFade.attachToComponent(&crossfadeSlider, false);

        formatManager.registerBasicFormats();

}

MainComponent::~MainComponent()
{
   
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

}
void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
   
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
    g.fillAll(juce::Colour(254, 238, 144));
    g.setColour(juce::Colours::orange);
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() / 1.9);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 1.9);

    playlistComponent.setBounds(0, getHeight()/1.9, getWidth(), getHeight()/2.1);
    crossfadeSlider.setBounds(getWidth()/2 - 100, getHeight()/2.5, 200, 100);
}

void MainComponent::sliderValueChanged(Slider* slider) {
    if (slider == &crossfadeSlider) {
        // Get the current volume values from both DeckGUI instances
        float currentVol1 = deckGUI1.volSlider.getValue();
        float currentVol2 = deckGUI2.volSlider.getValue();

        // Get the crossfade value from the slider
        float crossfadeValue = slider->getValue();

        // Calculate the adjusted volume levels based on the crossfade value
        float adjustedVol1 = currentVol1 * (1 - crossfadeValue);
        float adjustedVol2 = currentVol2 * crossfadeValue;

        // Apply the adjusted volume levels to the players
        player1.setGain(adjustedVol1);
        player2.setGain(adjustedVol2);
    }
}

