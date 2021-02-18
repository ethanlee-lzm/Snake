#pragma once
#include "src/GameEngine/hpp/State.hpp"
#include "src/GameEngine/hpp/Game.hpp"
#include <fstream>

namespace hgw
{
	class MenuState : public State
	{
	public:
		MenuState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

		sf::Vector2f getBestScorePos();
	private:
		GameDataRef _data;
		int _bestScore = 0;

		sf::Sprite _mainMenuSnake;

		sf::Text _mainMenuStart;
		sf::Text _mainMenuOptions;
		sf::Text _mainMenuBestScore;

		std::fstream _dataFile;
	};
}