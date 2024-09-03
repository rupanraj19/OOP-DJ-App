/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 30 Jan 2024 8:25:17am
    Author:  ruby

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager): formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
   
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    audioLowFilter.prepareToPlay(samplesPerBlockExpected, sampleRate);
    audioMidFilter.prepareToPlay(samplesPerBlockExpected, sampleRate);
    audioHighFilter.prepareToPlay(samplesPerBlockExpected, sampleRate);

    thisSampleRate = sampleRate;

}
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
   
    audioHighFilter.getNextAudioBlock(bufferToFill);

    float rmsLevelLeft = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.buffer->getNumSamples()));
    float rmsLevelRight = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.buffer->getNumSamples()));
    level = (rmsLevelLeft + rmsLevelRight) / 2;

}

float DJAudioPlayer::getRMSLevel() {
    return level;
};


void DJAudioPlayer::releaseResources()
{
    audioHighFilter.releaseResources();  
}

void DJAudioPlayer::loadURL(URL audioURL)
{

    //get the name of the song that is current playing
    trackPlayingTitle = audioURL.getFileName();

    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());

        trackLoaded = true;
    }

  
}
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }

}
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::setLBFilter(double gain) {
    audioLowFilter.setCoefficients(juce::IIRCoefficients::makeLowShelf(thisSampleRate, 500, 1.0 / juce::MathConstants<double>::sqrt2, gain));
};

void DJAudioPlayer::setMBFilter(double gain) {
    audioMidFilter.setCoefficients(juce::IIRCoefficients::makePeakFilter(thisSampleRate, 3250, 1.0 / juce::MathConstants<double>::sqrt2, gain));
};

void DJAudioPlayer::setHBFilter(double gain) {
    audioHighFilter.setCoefficients(juce::IIRCoefficients::makeHighShelf(thisSampleRate, 5000, 1.0 / juce::MathConstants<double>::sqrt2, gain));
};


/** check if audio is playing */
bool DJAudioPlayer::isPlaying()
{
    return true;
}

/** toggles the loop button */
void DJAudioPlayer::toggleLoop()
{
    if (loop) {
        loop = false;
    }
    else {
        loop = true;
    }
}

/** check if loop is on */
bool DJAudioPlayer::isLooping()
{
    return loop;
}

void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();

}

/** rewind track by 5 seconds */
void DJAudioPlayer::rewind()
{
    //check if position is more than 0 after rewind 5secs
   
    if ((transportSource.getCurrentPosition() - 5.0) > 0)
    {
        transportSource.setPosition(transportSource.getCurrentPosition() - 5.0);
    }
    else  //else set position to beginning of the track
    {
        transportSource.setPosition(0);
    }
}

/** forward track by 5 seconds */
void DJAudioPlayer::forward()
{
    
    if ((transportSource.getCurrentPosition() + 5.0) < transportSource.getLengthInSeconds())
    {
        transportSource.setPosition(transportSource.getCurrentPosition() + 5.0);
    }
    else
    {
        transportSource.setPosition(transportSource.getLengthInSeconds());
    }
}

juce::String DJAudioPlayer::trackLeft()
{
    int trackLengthLeft = transportSource.getLengthInSeconds() - transportSource.getCurrentPosition();

    // Calculate minutes and seconds
    int mins = trackLengthLeft / 60;
    int secs = trackLengthLeft % 60;

    // Format the time string
    std::stringstream timeString;
    timeString << std::setfill('0') << std::setw(2) << mins << ":" << std::setw(2) << secs;

    // Return the formatted time string
    return timeString.str();
}