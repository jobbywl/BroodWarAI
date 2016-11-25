#include "ReplayAnalyzer.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

/*
**Currently this is just a build order analyzer.
**It saves the build order in a file called <matchup><number>.log
**This can be used as a training set for the Neural Network
*/

void ReplayAnalyzer::onStart()
{
	// Hello World!
	Broodwar->sendText("Hello world!");

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
	
	//create the string containing the file name
	std::string Filename;
	Filename = "./analyzedReplays/";

	// Check if this is a replay
	if (Broodwar->isReplay())
	{
		Broodwar->setLocalSpeed(0);
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
				Filename += (p->getRace().c_str());
				

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
	Broodwar->sendText(Filename.c_str());
	Filename += ".log";

	Output = new std::fstream;
	Output->open(Filename, std::fstream::out);
}

void ReplayAnalyzer::onEnd()
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
	*/

	Output->close();
	Broodwar->leaveGame();
}

void ReplayAnalyzer::onFrame()
{
	// Called once every game frame

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
			*Output << minutes << seconds << ';' << unit->getType().c_str() << std::endl;;
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
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
			*Output << minutes << seconds << ';' << unit->getType().c_str() << std::endl;;
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
}
