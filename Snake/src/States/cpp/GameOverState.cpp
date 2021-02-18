#include "src/States/hpp/GameOverState.hpp"
#include "src/States/hpp/GameState.hpp"
#include "src/DEFINE.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

namespace hgw
{
	GameOverState::GameOverState(GameDataRef data, bool newBestSet) : _data(data), isNewHighScoreSet(newBestSet)
	{
	}

	void GameOverState::Init()
	{
		fadeOut.setFillColor(sf::Color(0, 0, 0, 0));
		fadeOut.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

		_data->graphics.LoadFont("FreePixel", FREE_PIXEL_FONT);
		
		GameOver.setFont(_data->graphics.GetFont("FreePixel"));
		Retry.setFont(_data->graphics.GetFont("FreePixel"));
		Quit.setFont(_data->graphics.GetFont("FreePixel"));
		NewHighScore.setFont(_data->graphics.GetFont("FreePixel"));

		GameOver.setString("GAME OVER");
		Retry.setString("RETRY");
		Quit.setString("QUIT");
		if (isNewHighScoreSet)
		{
			NewHighScore.setString("NEW HIGHSCORE:" + std::to_string(_data->variables.bestScore));
		}
		
		GameOver.setCharacterSize(80);
		Retry.setCharacterSize(80);
		Quit.setCharacterSize(80);
		NewHighScore.setCharacterSize(80);

		GameOver.setPosition((SCREEN_WIDTH - GameOver.getGlobalBounds().width) / 2, (SCREEN_HEIGHT - GameOver.getGlobalBounds().height) / 2 - 100);
		Retry.setPosition((SCREEN_WIDTH - Retry.getGlobalBounds().width) / 2, GameOver.getPosition().y + 100);
		Quit.setPosition((SCREEN_WIDTH - Quit.getGlobalBounds().width) / 2, Retry.getPosition().y + 100);
		NewHighScore.setPosition((SCREEN_WIDTH - NewHighScore.getGlobalBounds().width) / 2, (SCREEN_HEIGHT - NewHighScore.getGlobalBounds().height) / 10);
	}

	void GameOverState::HandleInput()
	{
		sf::Event event;

		while (_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				_data->window.close();
			}

			if (_data->input.IsTextClicked(Retry, sf::Mouse::Button::Left, event.type, _data->window))
			{
				_data->machine.AddState(StateRef(new GameState(_data)));
			}

			if (_data->input.IsTextClicked(Quit, sf::Mouse::Button::Left, event.type, _data->window))
			{
				_data->window.close();
			}
		}
	}

	void GameOverState::Update(float dt)
	{
		
	}

	void GameOverState::Draw(float dt)
	{
		_data->window.clear(sf::Color::Black);
		
		_data->window.draw(GameOver);
		_data->window.draw(Retry);
		_data->window.draw(Quit);
		_data->window.draw(NewHighScore);

		_data->window.display();
	}
}