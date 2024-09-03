/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 30 Jan 2024 8:25:17am
    Author:  ruby

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;

class DJAudioPlayer : public AudioSource {
public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    float getRMSLevel();
 
    void setLBFilter(double gain);
    void setMBFilter(double gain);
    void setHBFilter(double gain);

    void start();
    void stop();

    /*check if audio is playing*/
    bool isPlaying();
    /** rewind track by 5 seconds */
    void rewind();
    /** forward track by 5 seconds */
    void forward();
    /** toggles the loop button */
    void toggleLoop();
    /** check if loop is on */
    bool isLooping();

    //get the relative position of the play head
    double getPositionRelative();

    juce::String trackPlayingTitle;

    /** track length left */
    juce::String trackLeft();

    /** bool that signifies track is loaded into the player */
    bool trackLoaded;


private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

    /// IIRFilterAudioSource to manage low band filter controls
    IIRFilterAudioSource audioLowFilter{ &resampleSource , false };

    /// IIRFilterAudioSource to manage mid band filter controls
    IIRFilterAudioSource audioMidFilter{ &audioLowFilter , false };

    /// IIRFilterAudioSource to manage high band filter controls
    IIRFilterAudioSource audioHighFilter{ &audioMidFilter , false };

    //bool - if loop is on or off
    bool loop;

    double thisSampleRate;
    float level;
};

