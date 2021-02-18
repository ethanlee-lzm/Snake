#pragma once
#include "src/GameEngine/hpp/State.hpp"
#include "src/GameEngine/hpp/Game.hpp"
#include <fstream>

namespace hgw
{
	class GameOverState : public State
	{
	public:
		GameOverState(GameDataRef data, bool isNewBestSet);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		int highScore;
		bool isNewHighScoreSet;

		sf::Text GameOver;
		sf::Text Retry;
		sf::Text Quit;
		sf::Text NewHighScore;
		
		sf::Vector2f BestScorePos;

		sf::RectangleShape fadeOut;
	};
}