#include "ReplayAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <Windows.h>

using namespace BWAPI;
using namespace Filter;

/*
**Currently this is just a build order analyzer.
**It saves the build order in a file called <matchup><number>.log
**This can be used as a training set for the Neural Network
*/

ReplayAnalyzer::ReplayAnalyzer(std::string dir)
{
	replayList = getDirContents(dir);
	//create the string containing the file name

	std::string nextmap = replayList->front();

	int tempint = nextmap.find('\\');
	while (tempint > 0)
	{
		nextmap.replace(tempint , 1, "/");
		tempint = nextmap.find('\\');
	}
	
	setNextMap(nextmap);
	replayList->pop_front();
}

ReplayAnalyzer::~ReplayAnalyzer()
{
	Output->close();
	if (replayList != NULL)
		delete replayList;
}

void ReplayAnalyzer::onStart()
{
	std::string Filename("C:\\analyzedReplays\\");
	std::string extension;



	int i = 0;
	extension += std::to_string(i);
	extension += ".log";

	std::ifstream temp(Filename + extension);

	while (temp.is_open())
	{
		temp.close();
		i++;
		Playerset players = Broodwar->getPlayers();
		for (auto p : players)
		{
			// Only print the player if they are not an observer
			if (!p->isObserver())
			{
				extension += p->getRace();
				extension += "_";
			}
		}
		extension += std::to_string(i);
		extension += ".log";
		temp.open(Filename + extension);

	}
	if (temp.is_open())
		temp.close();

	Output = new std::fstream;
	Output->open(Filename + extension, std::fstream::out);

	// Print the map name.
	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	// Uncomment the following line and the bot will know about everything through the fog of war (cheat).
	//Broodwar->enableFlag(Flag::CompleteMapInformation);

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

	// Check if this is a replay
	if (Broodwar->isReplay())
	{
		Broodwar->setLocalSpeed(0);
		Broodwar->setFrameSkip(1);
		if (Broodwar->isDebug())
		{
			Broodwar->setGUI(false);
		}

		// Announce the players in the replay
		Broodwar << "The following players are in this replay:" << std::endl;

		// Iterate all the players in the game using a std:: iterator
		Playerset players = Broodwar->getPlayers();
		for (auto p : players)
		{
			// Only print the player if they are not an observer
			if (!p->isObserver())
			{
				Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
			}
		}

	}
	else // if this is not a replay
	{
		// Retrieve you and your enemy's races. enemy() will just return the first enemy.
		// If you wish to deal with multiple enemies then you must use enemies().
		Broodwar->sendText("I am a replay analyzer i shouldn't play games :(");
		Broodwar->sendText("I will be going now bye bye and gg");
		Broodwar->leaveGame();
	}
}

void ReplayAnalyzer::onEnd(bool isWinner)
{
	// Called when the game ends
	Playerset players = Broodwar->getPlayers();
	for (auto p : players)
	{
		// Only print the player if they are not an observer
		if (!p->isObserver())
		{
			if (p->isVictorious())
			{
				*Output << "Winner" << ';' << p->getRace().c_str() << std::endl;
			}
			else if (p->isDefeated())
			{
				*Output << "Loser" << ';' << p->getRace().c_str() << std::endl;
			}


		}
	}

	/*
	The program must now setup the next replay for analysis
	This can be done by changing the next map variable in the chaoslauncher bwapi config.ini
	map = maps\Replays\nrg__i_am_new.rep
	This variable needs to be changed to the new map
	This file is in te Starcraft/bwapi-data directory
	*/


	if (replayList->size() != 0)
	{
		setNextMap(replayList->front());
		replayList->pop_front();
	}
	else {
		setNextMap("maps/replays");
	}



	Output->close();
	Broodwar->leaveGame();
}

void ReplayAnalyzer::onFrame()
{
	// Called once every game frame
	Output->flush();
	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

}

void ReplayAnalyzer::onSendText(std::string text)
{

	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());


	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!

}

void ReplayAnalyzer::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ReplayAnalyzer::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ReplayAnalyzer::onNukeDetect(BWAPI::Position target)
{

	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}

	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ReplayAnalyzer::onUnitDiscover(BWAPI::Unit unit)
{
}

void ReplayAnalyzer::onUnitEvade(BWAPI::Unit unit)
{
}

void ReplayAnalyzer::onUnitShow(BWAPI::Unit unit)
{
}

void ReplayAnalyzer::onUnitHide(BWAPI::Unit unit)
{
}

void ReplayAnalyzer::onUnitCreate(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;

			Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
			*Output << std::setfill('0') << std::setw(2) << minutes << std::setw(2) << seconds << ';' << unit->getType().c_str() << std::endl;;
		}
	}
}

void ReplayAnalyzer::onUnitDestroy(BWAPI::Unit unit)
{
}

void ReplayAnalyzer::onUnitMorph(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (!unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
			*Output << std::setfill('0') << std::setw(2) << minutes << std::setw(2) << seconds << ';' << unit->getType().c_str() << std::endl;;
		}
	}
}

void ReplayAnalyzer::onUnitRenegade(BWAPI::Unit unit)
{
}

void ReplayAnalyzer::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ReplayAnalyzer::onUnitComplete(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (!unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s completes a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
			*Output << std::setfill('0') << std::setw(2) << minutes << std::setw(2) << seconds << ';' << unit->getType().c_str() << std::endl;;
		}
	}
}


std::wstring ReplayAnalyzer::toWCHAR(std::string dir)
{
	if (dir.empty())
		return NULL;
	int sz = MultiByteToWideChar(GetACP(), 0, &dir[0], (int)dir.size(), 0, 0);
	std::wstring res(sz, 0);
	MultiByteToWideChar(GetACP(), 0, &dir[0], (int)dir.size(), &res[0], sz);
	return res;
}

std::list<std::string> *ReplayAnalyzer::getDirContents(std::string dir)
{
	WIN32_FIND_DATA found;

	HANDLE hFind = FindFirstFile(toWCHAR(dir + "*").c_str(), &found);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return  new std::list<std::string>;
	}
	// List all the files in the directory with some info about them.
	std::list<std::string> *fileList;
	fileList = new std::list<std::string>;
	do
	{
		if (found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

			std::list<std::string> *temp;
			std::wstring tempWstr;
			tempWstr = found.cFileName;
			std::string tempstr(tempWstr.begin(), tempWstr.end());

			if ((tempstr.find("..") == std::string::npos) && (tempWstr.find(L".") == std::string::npos))
			{
				temp = getDirContents(dir + tempstr + "\\");
				fileList->splice(fileList->end(), *temp);
				delete temp;
			}
		}
		else
		{
			std::wstring temp(found.cFileName);
			std::string tempstr(temp.begin(), temp.end());
			tempstr.insert(0, dir);
			fileList->push_back(tempstr);
		}
	} while (FindNextFile(hFind, &found) != 0);
	return fileList;
}

void ReplayAnalyzer::setNextMap(std::string Filename)
{
	//First read all the contents to a string buffer
	//Modify where necessary
	//Write to file
	std::fstream bwapi_ini;
	bwapi_ini.open(("C:\\Starcraft\\bwapi-data\\bwapi.ini"), std::fstream::in);

	std::string temp("");
	std::string temp2("");
	while (!bwapi_ini.eof())
	{
		getline(bwapi_ini, temp2);
		temp += temp2 + '\n';
	}
		

	bwapi_ini.close();
	

	//first delete everything after map =
	size_t nFPos = temp.find("map = ");
	size_t secondNL = temp.find('\n', nFPos);
	size_t firstNL = temp.rfind('\n', nFPos);

	temp.erase(firstNL, secondNL - firstNL);
	temp2 = "\nmap = ";
	temp2 += Filename.substr(13, Filename.size());
	temp2 += '\n';
	//Now add a new line containing the map variable
	temp.insert(firstNL, temp2);
	

	DeleteFile(L"C:\\Starcraft\\bwapi-data\\bwapi.ini");
	bwapi_ini.open(("C:\\Starcraft\\bwapi-data\\bwapi.ini"), std::fstream::out);
	bwapi_ini << temp;
	bwapi_ini.close();
}