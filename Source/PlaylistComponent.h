/*
  ==============================================================================

    PlaylistComponent.h
    Created: 1 Feb 2024 1:59:40pm
    Author:  ruby

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iomanip>


using namespace juce;
//==============================================================================
/*
*/
class PlaylistComponent  : public Component, 
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener,
                           public FileDragAndDropTarget
                   
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,DeckGUI* _deckGUI2,AudioFormatManager& _formatManager);
    ~PlaylistComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics& , 
                            int rowNumber, 
                            int width, 
                            int height,     
                            bool rowIsSelected) override;

    void paintCell(Graphics&, 
                   int rowNumber,
                   int columnId,    
                   int width, 
                   int height,
                   bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
                                       int columnId, 
                                       bool isRowSelected,
                                       Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

    /** implement FileDragAndDropTarget */
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;


private:
    TableListBox tableComponent;

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    std::vector<juce::String>  trackTitles;
    std::vector<URL> trackURL;
    std::vector<juce::String> trackLength;
   
    AudioFormatManager& formatManager;


    /** calculates the length of the track with the audio file parsed in */
    std::string getTrackLength(File track);

    //load playlist 
    TextButton loadPlaylistButton{ "Load Playlist" };
    TextButton savePlaylistButton{ "Save Playlist" };
    TextEditor searchBox;

    Label middleLabel;

    /** load the tracks into playlist */
    void loadPlaylist();

    /** saves the tracks from playlist */
    void savePlaylist();

    /** returns the row where the search track title is at */
    int searchText(const juce::String& inputText);

    /** calls searchText, and rselect rhe row the text */
    void returnSearch(juce::String searchText);

    FileChooser fChooser{ "Select a file..." };

    juce::File filepath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userMusicDirectory).getChildFile("Playlist.csv");

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
