#pragma once
#include "src/GameEngine/hpp/State.hpp"
#include "src/GameEngine/hpp/Game.hpp"
#include <vector>
#include <fstream>

namespace hgw
{
	class GameState : public State
	{
	public:
		enum moveDir { Left, Right, Up, Down};
		
		GameState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);
	private:
		bool isFading = false;
		bool waitingForEnd = false;

		void AddPartToSnake();

		bool IsSnakeGood();
		bool IsAppleEaten();
		bool DoesSnakeContainApple(sf::Vector2f posToCheck);

		sf::Vector2f RandomOnScreen();

		GameDataRef _data;

		std::vector<moveDir> moveDirections;
		std::vector<sf::RectangleShape> Snake;

		sf::RectangleShape Apple;
		sf::RectangleShape Border;
		sf::RectangleShape fadeOut;

		sf::Text Score;
		sf::Text BestScore;

		int currentScoreValue = 0;
		bool isNewHighScoreSet;

		std::fstream dataFile;
	};
}