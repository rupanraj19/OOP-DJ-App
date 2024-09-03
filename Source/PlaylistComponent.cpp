/*
  ==============================================================================

	PlaylistComponent.cpp
	Created: 1 Feb 2024 1:59:40pm
	Author:  ruby

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, AudioFormatManager& _formatManager) :deckGUI1(_deckGUI1), deckGUI2(_deckGUI2), formatManager(_formatManager)
{
	tableComponent.getHeader().addColumn("Track title", 1, 400);
	tableComponent.getHeader().addColumn("Track Length", 2, 100);
	tableComponent.getHeader().addColumn("Deck1", 3, 100);
	tableComponent.getHeader().addColumn("Deck2", 4, 100);
	tableComponent.getHeader().addColumn("Remove", 5, 100);
	tableComponent.setModel(this);

	addAndMakeVisible(tableComponent);
	addAndMakeVisible(loadPlaylistButton);
	addAndMakeVisible(savePlaylistButton);
	addAndMakeVisible(searchBox);
	addAndMakeVisible(middleLabel);

	middleLabel.setText("Drag and Drop Files Here", juce::dontSendNotification);
	middleLabel.setJustificationType(juce::Justification::centred);


	loadPlaylistButton.addListener(this);
	savePlaylistButton.addListener(this);
	searchBox.addListener(this);


	//setup search box, call function to return searched track
	searchBox.setTextToShowWhenEmpty("Search for track...", juce::Colours::white);
	//When text input occurs in the text editor, the returnSearch function is invoked, passing the input parameter as the text contained in the searchBox.
	searchBox.onTextChange = [this] {
		returnSearch(searchBox.getText());
		};

	loadPlaylist();


}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	g.setColour(juce::Colours::white);
	g.setFont(14.0f);
	g.drawText("PlaylistComponent", getLocalBounds(),juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
	loadPlaylistButton.setBounds(10, getHeight() - 40, 100, 30);
	savePlaylistButton.setBounds(120, getHeight() - 40, 100, 30);
	searchBox.setBounds(getWidth() - 410, 0, 400, 28);
	middleLabel.setBounds(getWidth() / 2 - 100, getHeight() / 2, 200, 20);
	tableComponent.setBounds(0, 0, getWidth(), getHeight());
}



void PlaylistComponent::paintRowBackground(juce::Graphics& g,
	int rowNumber,
	int width,
	int height,
	bool rowIsSelected)
{
	if (rowIsSelected)
	{
		g.fillAll(juce::Colours::purple);
	}
	else
	{
		g.fillAll(juce::Colours::pink);
	}
}

void PlaylistComponent::paintCell(juce::Graphics& g,
	int rowNumber,
	int columnId,
	int width,
	int height,
	bool rowIsSelected)
{
	// track title column
	if (columnId == 1)
	{
		g.drawText(trackTitles[rowNumber], 2, 1, width - 4, height, Justification::centredLeft, true);
	}
	// track length column
	if (columnId == 2)
	{
		if (rowNumber < trackLength.size())
		{
			g.drawText(trackLength[rowNumber], 2, 2, width - 4, height, Justification::centredLeft, true);
		}
	}

}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{

	if (columnId == 3 || columnId == 4)
	{
		//create component yet
		if (existingComponentToUpdate == nullptr)
		{
			TextButton* btn = new TextButton{ "LOAD" };
			std::string rowid{ std::to_string(rowNumber) };
			std::string colid{ std::to_string(columnId) };
			//to get a unique id made of 2 strings added together
			btn->setComponentID((rowid + colid));
			btn->addListener(this);
			existingComponentToUpdate = btn;
		}
	}
	//remove column
	if (columnId == 5)
	{
		if (existingComponentToUpdate == nullptr)
		{
			TextButton* btn = new TextButton{ "REMOVE" };
			std::string rowid{ std::to_string(rowNumber) };
			std::string colid{ std::to_string(columnId) };
			btn->setComponentID((rowid + colid));
			btn->addListener(this);
			existingComponentToUpdate = btn;
		}
	}

	return existingComponentToUpdate;

}

int PlaylistComponent::getNumRows()
{
	return trackTitles.size();
}

void PlaylistComponent::buttonClicked(Button* button)
{

	if (button == &loadPlaylistButton)
	{
		if (getNumRows() >= 5)
		{
			// Hide the middle label
			middleLabel.setVisible(false);
		}

		auto fileChooserFlags =
			FileBrowserComponent::canSelectMultipleItems;
		fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
			{

				auto selectedFiles = chooser.getResults();
				for (const auto& file : selectedFiles)
				{
					trackTitles.push_back(file.getFileNameWithoutExtension());
					trackURL.push_back(URL{ File{file.getFullPathName()} });
					std::cout << "URL: " << file.getFullPathName() << std::endl;
					trackLength.push_back(getTrackLength(file));
				}
				tableComponent.updateContent();

			});
		tableComponent.updateContent();

	}
	else if (button == &savePlaylistButton)
	{
	
		savePlaylist();
	}
	else
	{
		//get first index of button id which is row
		int row = std::stoi(button->getComponentID().toStdString().substr(0, 1));
		//get second index of button id which is column
		int col = std::stoi(button->getComponentID().toStdString().substr(1, 2));

		std::cout << "Clicked button in row: " << row << ", column: " << col << std::endl;

		// Now you can safely use row and col indices
		if (col == 3)
		{
			// Your existing implementation for loading to deck1
			//load the url into deck1 player and waveform, and sets the waveform position
			std::cout << "URL for deckGUI1: " << URL(trackURL[row]).toString(false) << std::endl;

			deckGUI1->player->loadURL(URL{ trackURL[row] });
			deckGUI1->waveformDisplay.loadURL(URL{ trackURL[row] });
			deckGUI1->waveformDisplay.setPositionRelative(deckGUI1->player->getPositionRelative());

			//calls paint so that the track title and length is displayed immediately as the track is loaded in
			deckGUI1->repaint();
		}
		if (col == 4)
		{
			// Your existing implementation for loading to deck1
			//load the url into deck1 player and waveform, and sets the waveform position
			deckGUI2->player->loadURL(URL{ trackURL[row] });
			deckGUI2->waveformDisplay.loadURL(URL{ trackURL[row] });
			deckGUI2->waveformDisplay.setPositionRelative(deckGUI2->player->getPositionRelative());

			//calls paint so that the track title and length is displayed immediately as the track is loaded in
			deckGUI2->repaint();
		}
		if (col == 5)
		{
			//remove the title, url, length from the vectors
			trackTitles.erase(trackTitles.begin() + row);
			trackURL.erase(trackURL.begin() + row);
			trackLength.erase(trackLength.begin() + row);
			tableComponent.updateContent();
		}
	}
}

/** implement FileDragAndDropTarget */
bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{
	std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
	return true;
}

/** implement FileDragAndDropTarget */
void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
	std::cout << "DeckGUI::filesDropped" << std::endl;
	//load all files into playlist
	if (files.size() >= 1)
	{
		for (int i = 0; i < files.size(); ++i)
		{
			//push the track title into vector
			trackTitles.push_back(File{ files[i] }.getFileNameWithoutExtension());
			//push the track URL into vector
			trackURL.push_back(URL{ File{files[i]} }.toString(false));
			//push the track length into vector
			trackLength.push_back(getTrackLength(File{ files[i] }));

			std::cout << "URL for deckGUI1: " << URL(trackURL[i]).toString(true) << std::endl;
		}
		//update the table
		tableComponent.updateContent();
	}
}

std::string PlaylistComponent::getTrackLength(File track)
{
	std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(track));
	if (reader == nullptr) {
		return "N/A";
	}

	int audioLength = reader->lengthInSamples / reader->sampleRate;

	// Calculate minutes and seconds
	int minutes = audioLength / 60;
	int seconds = audioLength % 60;

	// Format minutes and seconds with leading zeros
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << minutes; // Minutes
	oss << ":";
	oss << std::setw(2) << std::setfill('0') << seconds; // Seconds

	return oss.str();
}


void PlaylistComponent::loadPlaylist()
{
	// Open playlist file for reading
	juce::File playlist(filepath);
	if (playlist.existsAsFile())
	{
		juce::FileInputStream input(playlist);
		input.setPosition(0);

		// Read file line by line
		while (!input.isExhausted())
		{
			// Read line
			std::string line = input.readNextLine().toStdString();
			std::cout << line << std::endl; // Print the line to console

			// Use stringstream to parse the line
			std::stringstream ss(line);

			// Temporary variables for metadata of track
			std::string _trackTitles;
			std::string _trackURL;
			std::string _trackLength;

			// Save track titles (first part of the line) into vector
			getline(ss, _trackTitles, ',');
			trackTitles.push_back(_trackTitles);

			// Save track URL (second part of the line) into vector
			getline(ss, _trackURL, ',');
			trackURL.push_back(URL(_trackURL));

			// Save track length (last part of the line) into vector
			getline(ss, _trackLength, ',');
			trackLength.push_back(_trackLength);
		}
	}
	else
	{
		// Handle the case where the playlist file doesn't exist
		std::cout << "Playlist file not found!" << std::endl;
	}
}


/** save the playlist */
void PlaylistComponent::savePlaylist()
{
	// Open playlist for writing
	std::fstream playlist;

	// Get the file path as a String
	juce::String filePathString = filepath.getFullPathName();

	// Convert the String to std::string
	std::string filePathStdString = filePathString.toStdString();

	// Open the file with the converted file path
	playlist.open(filePathStdString, std::fstream::out);

	// Check if the file is successfully opened
	if (playlist.is_open())
	{
		// Save the strings in the vector to the csv file
		for (int i = 0; i < trackURL.size(); ++i)
		{
			playlist << trackTitles[i] << "," << trackURL[i].toString(false) << "," << trackLength[i] << "\n";
		}

		// Close the file
		playlist.close();
	}
	else
	{
		// Handle error opening the file
		std::cerr << "Error: Unable to open file for writing: " << filePathStdString << std::endl;
	}
}
int PlaylistComponent::searchText(const juce::String& inputText)
{
	auto iter = std::find_if(trackTitles.begin(), trackTitles.end(),
		[&inputText](const juce::String& tracks) {
			return tracks.contains(inputText);
		});

	return (iter != trackTitles.end()) ? std::distance(trackTitles.begin(), iter) : 0;
}


void PlaylistComponent::returnSearch(juce::String inputText)
{
	if (inputText != "")
	{
		int rowNumber = searchText(inputText);
		tableComponent.selectRow(rowNumber);
	}
	else
	{
		tableComponent.deselectAllRows();
	}
}
