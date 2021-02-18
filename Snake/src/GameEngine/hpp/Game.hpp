#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "StateMachine.hpp"
#include "GraphicsManager.hpp"
#include "InputManager.hpp"

namespace hgw
{
	struct GameDataVaraibles
	{
		int bestScore;
		sf::Vector2f bestScorePos;
	};

	struct GameData
	{
		StateMachine machine;
		sf::RenderWindow window;
		GraphicsManager graphics;
		InputManager input;

		GameDataVaraibles variables;
	};

	typedef std::shared_ptr<GameData> GameDataRef;

	class Game
	{
	public:
		Game(int width, int height, std::string title);

	private:
		const float dt = 1.0f / 15.0f;
		sf::Clock _clock;

		GameDataRef _data = std::make_shared<GameData>();

		void Run();
	};





}