#include <BWAPI.h>
#include <BWAPI/Client.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <Windows.h>

#include "ReplayAnalyzer.h"
#include "BroodWarAI.h"

using namespace BWAPI;

void drawStats();
void drawBullets();
void drawVisibilityData();
void showPlayers();
void showForces();
bool show_bullets;
bool show_visibility_data;

void reconnect()
{
	while (!BWAPIClient.connect())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
	}
}


int main(int argc, const char* argv[])
{
	std::cout << "Connecting..." << std::endl;;

	BWAPI::AIModule *Caller;
	//This link has a replay pack with 7708 replays in it of high level. No duplicates
	//http://emotion.inrialpes.fr/people/synnaeve/TLGGICCUP_gosu_reps.7z
	//Replay crawler https://github.com/syhw/Broodwar_replays_scrappers
	ReplayAnalyzer *p_Analyzer = new ReplayAnalyzer("G:\\Blizzard\\StarCraft\\");
	BroodWarAI *p_Bot = new BroodWarAI;

	reconnect();


	while (true)
	{
		std::cout << "waiting to enter match" << std::endl;

		

		while (!Broodwar->isInGame())
		{
			BWAPI::BWAPIClient.update();
			if (!BWAPI::BWAPIClient.isConnected())
			{
				std::cout << "Reconnecting..." << std::endl;;
				reconnect();
			}
		}
		std::cout << "starting match!" << std::endl;
		//If the game is a replay load the replay analyzer;
		if (Broodwar->isReplay())
		{
			//TODO: only execute if it was a replay
			if (p_Analyzer->getReplayAmount() > 0)
			{
				std::cout << p_Analyzer->getReplayAmount() << " Replays to go" << std::endl;

			}
			else
			{
				std::cout << "All replays done quitting program now" << std::endl;
				break;
			}
			Caller = p_Analyzer;
		}
		else
		{
			Caller = p_Bot;
		}
		Caller->onStart();
		// Enable some cheat flags
		Broodwar->enableFlag(Flag::UserInput);
		// Uncomment to enable complete map information
		//Broodwar->enableFlag(Flag::CompleteMapInformation);

		show_bullets = false;
		show_visibility_data = false;


		while (Broodwar->isInGame())
		{
			Caller->onFrame();
			for (auto &e : Broodwar->getEvents())
			{
				switch (e.getType())
				{
				case EventType::MatchEnd:
					Caller->onEnd(e.isWinner());
					break;
				case EventType::SendText:
					Caller->onSendText(e.getText());
					break;
				case EventType::ReceiveText:
					Caller->onReceiveText(e.getPlayer(), e.getText());
					break;
				case EventType::PlayerLeft:
					Caller->onPlayerLeft(e.getPlayer());
					break;
				case EventType::NukeDetect:
					Caller->onNukeDetect(e.getPosition());
					break;
				case EventType::UnitCreate:
					Caller->onUnitCreate(e.getUnit());
					break;
				case EventType::UnitDestroy:
					Caller->onUnitDestroy(e.getUnit());
					break;
				case EventType::UnitMorph:
					Caller->onUnitMorph(e.getUnit());
					break;
				case EventType::UnitShow:
					Caller->onUnitShow(e.getUnit());
					break;
				case EventType::UnitHide:
					Caller->onUnitHide(e.getUnit());
					break;
				case EventType::UnitRenegade:
					Caller->onUnitRenegade(e.getUnit());
					break;
				case EventType::SaveGame:
					Caller->onSaveGame(e.getText());
					break;
				case EventType::UnitComplete:
					Caller->onUnitComplete(e.getUnit());
					break;
				case EventType::UnitDiscover:
					Caller->onUnitDiscover(e.getUnit());
					break;
				case EventType::UnitEvade:
					Caller->onUnitEvade(e.getUnit());
					break;
				}
			}

			if (show_bullets)
				drawBullets();

			if (show_visibility_data)
				drawVisibilityData();

			//drawStats();
			Broodwar->drawTextScreen(300, 0, "FPS: %f", Broodwar->getAverageFPS());

			BWAPI::BWAPIClient.update();
			if (!BWAPI::BWAPIClient.isConnected())
			{
				std::cout << "Reconnecting..." << std::endl;
				reconnect();
			}
		}
		std::cout << "Game ended" << std::endl;
	}
	std::cout << "Press ENTER to continue..." << std::endl;
	std::cin.ignore();
	return 0;
}

void drawStats()
{
	int line = 0;
	Broodwar->drawTextScreen(5, 0, "I have %d units:", Broodwar->self()->allUnitCount());
	for (auto& unitType : UnitTypes::allUnitTypes())
	{
		int count = Broodwar->self()->allUnitCount(unitType);
		if (count)
		{
			Broodwar->drawTextScreen(5, 16 * line, "- %d %s%c", count, unitType.c_str(), count == 1 ? ' ' : 's');
			++line;
		}
	}
}

void drawBullets()
{
	for (auto &b : Broodwar->getBullets())
	{
		Position p = b->getPosition();
		double velocityX = b->getVelocityX();
		double velocityY = b->getVelocityY();
		Broodwar->drawLineMap(p, p + Position((int)velocityX, (int)velocityY), b->getPlayer() == Broodwar->self() ? Colors::Green : Colors::Red);
		Broodwar->drawTextMap(p, "%c%s", b->getPlayer() == Broodwar->self() ? Text::Green : Text::Red, b->getType().c_str());
	}
}

void drawVisibilityData()
{
	int wid = Broodwar->mapHeight(), hgt = Broodwar->mapWidth();
	for (int x = 0; x < wid; ++x)
	for (int y = 0; y < hgt; ++y)
	{
		if (Broodwar->isExplored(x, y))
			Broodwar->drawDotMap(x * 32 + 16, y * 32 + 16, Broodwar->isVisible(x, y) ? Colors::Green : Colors::Blue);
		else
			Broodwar->drawDotMap(x * 32 + 16, y * 32 + 16, Colors::Red);
	}
}

void showPlayers()
{
	Playerset players = Broodwar->getPlayers();
	for (auto p : players)
		std::cout << "Player [" << p->getID() << "]: " << p->getName() << " is in force: " << p->getForce()->getName() << std::endl;
}

void showForces()
{
	Forceset forces = Broodwar->getForces();
	for (auto f : forces)
	{
		Playerset players = f->getPlayers();
		std::cout << "Force " << f->getName() << " has the following players:" << std::endl;
		for (auto p : players)
			std::cout << "  - Player [" << p->getID() << "]: " << p->getName() << std::endl;
	}
}
