#include "src/States/hpp/MenuState.hpp"
#include "src/States/hpp/GameState.hpp"
#include "src/DEFINE.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <filesystem>

namespace hgw
{
	namespace fs = std::experimental::filesystem;

	MenuState::MenuState(GameDataRef data) : _data(data)
	{

	}

	void MenuState::Init()
	{
		if (fs::exists("data/data.dat"))
		{
			if (_dataFile.good())
			{
				_dataFile.open("data/data.dat", std::fstream::binary | std::fstream::in);

				_dataFile >> _bestScore;
				_dataFile.close();
			}
		}
		else
		{
			_dataFile.open("data/data.dat", std::fstream::binary | std::fstream::out);
			_dataFile << _bestScore;
			_dataFile.close();
		}
		
		_data->graphics.LoadFont("FreePixel", FREE_PIXEL_FONT);

		_data->graphics.LoadTexture("Snake", MAIN_MENU_SNAKE);

		_mainMenuSnake.setTexture(_data->graphics.GetTexture("Snake"));
		_mainMenuStart.setFont(_data->graphics.GetFont("FreePixel"));
		_mainMenuOptions.setFont(_data->graphics.GetFont("FreePixel"));
		_mainMenuBestScore.setFont(_data->graphics.GetFont("FreePixel"));

		_mainMenuStart.setString("START");
		_mainMenuOptions.setString("OPTIONS");
		_mainMenuBestScore.setString("BEST SCORE: " + std::to_string(_bestScore));

		_mainMenuStart.setCharacterSize(80);
		_mainMenuOptions.setCharacterSize(80);
		_mainMenuBestScore.setCharacterSize(80);

		_mainMenuSnake.setPosition((SCREEN_WIDTH - _mainMenuSnake.getGlobalBounds().width) / 2, 30);
		_mainMenuStart.setPosition((SCREEN_WIDTH - _mainMenuStart.getLocalBounds().width) / 2, (SCREEN_HEIGHT - _mainMenuStart.getLocalBounds().height) / 2);
		_mainMenuOptions.setPosition((SCREEN_WIDTH - _mainMenuOptions.getGlobalBounds().width) / 2, _mainMenuStart.getPosition().y + 80);
		_mainMenuBestScore.setPosition((SCREEN_WIDTH - _mainMenuBestScore.getGlobalBounds().width) / 20 - 10, 800);

		_data->variables.bestScore = _bestScore;
		_data->variables.bestScorePos = _mainMenuBestScore.getPosition();
	}

	void MenuState::HandleInput()
	{
		sf::Event event;

		while (_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				_data->window.close();
			}

			if (_data->input.IsTextClicked(_mainMenuStart, sf::Mouse::Left, event.MouseButtonPressed, _data->window))
			{
				_data->machine.AddState(StateRef(new GameState(_data)));
			}
		}
	}

	void MenuState::Update(float dt)
	{

	}

	void MenuState::Draw(float dt)
	{
		_data->window.clear(sf::Color::Black);

		_data->window.draw(_mainMenuSnake);
		_data->window.draw(_mainMenuStart);
		_data->window.draw(_mainMenuOptions);
		_data->window.draw(_mainMenuBestScore);

		_data->window.display();
	}
}