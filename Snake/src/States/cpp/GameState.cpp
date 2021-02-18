#include "src/States/hpp/GameState.hpp"
#include "src/States/hpp/GameOverState.hpp"
#include "src/DEFINE.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>

namespace hgw
{
	namespace fs = std::experimental::filesystem;

	GameState::GameState(GameDataRef data) : _data(data)
	{

	}

	void GameState::Init()
	{
		fadeOut.setFillColor(sf::Color(0, 0, 0, 0));
		fadeOut.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

		Snake.push_back(sf::RectangleShape(sf::Vector2f(10, 10)));
		Snake.push_back(sf::RectangleShape(sf::Vector2f(10, 10)));
		Snake.push_back(sf::RectangleShape(sf::Vector2f(10, 10)));
		Snake.push_back(sf::RectangleShape(sf::Vector2f(10, 10)));

		Snake[0].setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		Snake[1].setPosition(Snake[0].getPosition().x + 10, Snake[0].getPosition().y);
		Snake[2].setPosition(Snake[1].getPosition().x + 10, Snake[1].getPosition().y);
		Snake[3].setPosition(Snake[2].getPosition().x + 10, Snake[2].getPosition().y);

		moveDirections.push_back(moveDir::Left);
		moveDirections.push_back(moveDir::Left);
		moveDirections.push_back(moveDir::Left);
		moveDirections.push_back(moveDir::Left);

		Apple.setSize(sf::Vector2f(10, 10));
		Apple.setFillColor(sf::Color::Red);
		Apple.setPosition(RandomOnScreen());

		Border.setSize(sf::Vector2f(SCREEN_WIDTH, 10));
		Border.setPosition(sf::Vector2f(0, 800));

		_data->graphics.LoadFont("FreePixel", FREE_PIXEL_FONT);

		Score.setFont(_data->graphics.GetFont("FreePixel"));
		BestScore.setFont(_data->graphics.GetFont("FreePixel"));

		Score.setCharacterSize(70);
		BestScore.setCharacterSize(70);

		BestScore.setString("BEST SCORE:" + std::to_string(_data->variables.bestScore));
		Score.setString("SCORE:0");

		BestScore.setPosition(_data->variables.bestScorePos);
		Score.setPosition(_data->variables.bestScorePos + sf::Vector2f(540, 0));
	}

	void GameState::HandleInput()
	{
		sf::Event event;

		while (_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				_data->window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && moveDirections[0] != moveDir::Down && moveDirections[1] != moveDir::Down)
			{
				moveDirections[0] = moveDir::Up;
				goto exitIFS;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && moveDirections[0] != moveDir::Up && moveDirections[1] != moveDir::Up)
			{
				moveDirections[0] = moveDir::Down;
				goto exitIFS;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && moveDirections[0] != moveDir::Right && moveDirections[1] != moveDir::Right)
			{
				moveDirections[0] = moveDir::Left;
				goto exitIFS;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && moveDirections[0] != moveDir::Left && moveDirections[1] != moveDir::Left)
			{
				moveDirections[0] = moveDir::Right;
				goto exitIFS;
			}
		exitIFS:;
		}
	}

	void GameState::Update(float dt)
	{
		if (waitingForEnd)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			_data->machine.AddState(StateRef(new GameOverState(_data, isNewHighScoreSet)));
		}

		if (isFading)
		{
			Apple.setFillColor(sf::Color(255, 0, 0, Apple.getFillColor().a - 5));
			Border.setFillColor(sf::Color(255, 255, 255, Border.getFillColor().a - 5));
			Score.setFillColor(sf::Color(255, 255, 255, Score.getFillColor().a - 5));
			BestScore.setFillColor(sf::Color(255, 255, 255, BestScore.getFillColor().a - 5));
			
			if (Apple.getFillColor().a == 0)
			{
				waitingForEnd = true;
				isFading = false;
			}
		}
		else
		{
			if (!IsSnakeGood())
			{
				isFading = true;

				if (currentScoreValue > _data->variables.bestScore)
				{
					_data->variables.bestScore = currentScoreValue;
					isNewHighScoreSet = true;
				}
				else
				{
					isNewHighScoreSet = false;
				}

				if (dataFile.good())
				{
					dataFile.open("data/data.dat", std::fstream::binary | std::fstream::out);

					dataFile << _data->variables.bestScore;
					dataFile.close();
				}
			}
			else
			{
				if (IsAppleEaten())
				{
					AddPartToSnake();
					sf::Vector2f randApplePos = RandomOnScreen();

					while (DoesSnakeContainApple(randApplePos))
					{
						randApplePos = RandomOnScreen();
					}
					Apple.setPosition(randApplePos);
				}

				std::vector<moveDir> tempDir(Snake.size());

				for (int i = Snake.size() - 1; i >= 0; i--)
				{
					switch (moveDirections[i])
					{
					case moveDir::Left:
					{
						if (Snake[i].getPosition().x == 0)
						{
							Snake[i].setPosition(sf::Vector2f(890, Snake[i].getPosition().y));
						}
						else
						{
							Snake[i].move(sf::Vector2f(-10, 0));
						}
						
						if (i + 1 != Snake.size())
						{
							tempDir[i + 1] = moveDir::Left;
							tempDir[i] = moveDir::Left;
						}
						break;
					}
					case moveDir::Right:
					{
						if (Snake[i].getPosition().x == 890)
						{
							Snake[i].setPosition(sf::Vector2f(0, Snake[i].getPosition().y));
						}
						else
						{
							Snake[i].move(sf::Vector2f(10, 0));
						}
						
						if (i + 1 != Snake.size())
						{
							tempDir[i + 1] = moveDir::Right;
							tempDir[i] = moveDir::Right;
						}
						break;
					}
					case moveDir::Up:
					{
						if (Snake[i].getPosition().y == 0)
						{
							Snake[i].setPosition(sf::Vector2f(Snake[i].getPosition().x, 790));
						}
						else
						{
							Snake[i].move(sf::Vector2f(0, -10));
						}

						if (i + 1 != Snake.size())
						{
							tempDir[i + 1] = moveDir::Up;
							tempDir[i] = moveDir::Up;
						}
						break;
					}
					case moveDir::Down:
					{
						if (Snake[i].getPosition().y == 790)
						{
							Snake[i].setPosition(sf::Vector2f(Snake[i].getPosition().x, 0));
						}
						else
						{
							Snake[i].move(sf::Vector2f(0, 10));
						}

						if (i + 1 != Snake.size())
						{
							tempDir[i + 1] = moveDir::Down;
							tempDir[i] = moveDir::Down;
						}
						break;
					}
					}
				}

				moveDirections = tempDir;

				Score.setString("SCORE:" + std::to_string(currentScoreValue));
				if (currentScoreValue > _data->variables.bestScore)
				{
					BestScore.setString("BEST SCORE:" + std::to_string(currentScoreValue));
				}
			}
		}	
	}

	void GameState::Draw(float dt)
	{
		_data->window.clear(sf::Color::Black);

		_data->window.draw(Apple);
		_data->window.draw(Border);
		
		_data->window.draw(Score);
		_data->window.draw(BestScore);
		
		for (auto &n : Snake)
		{
			_data->window.draw(n);
		}	

		_data->window.display();
	}

	bool GameState::IsSnakeGood()
	{
		for (int i = 0; i < Snake.size() - 1; i++)
		{
			for (int j = i + 1; j < Snake.size(); j++)
			{
				if (Snake[i].getPosition() == Snake[j].getPosition())
				{
					return false;
				}
			}
		}
		return true;
	}

	bool GameState::DoesSnakeContainApple(sf::Vector2f posToCheck)
	{
		for (auto &n : Snake)
		{
			if (n.getPosition() == posToCheck)
			{
				return true;
			}
		}
		return false;
	}

	sf::Vector2f GameState::RandomOnScreen()
	{
		int seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 generator(seed);
		std::uniform_int_distribution<int> distX(0, SCREEN_HEIGHT);
		std::uniform_int_distribution<int> distY(0, 800);
		int x = distX(generator);
		int y = distY(generator);

		x -= x % 10;
		y -= y % 10;

		return sf::Vector2f(x, y);
	}

	void GameState::AddPartToSnake()
	{
		sf::Vector2f cords;
		sf::Vector2f lastPartPos = Snake[Snake.size() - 1].getPosition();
		moveDir lastPartDir = moveDirections[Snake.size() - 1];

		switch (lastPartDir)
		{
			case moveDir::Left:
			{
				cords.x = 10;
				cords.y = 0;
				break;
			}
			case moveDir::Right:
			{
				cords.x = -10;
				cords.y = 0;
				break;
			}
			case moveDir::Up:
			{
				cords.x = 0;
				cords.y = 10;
				break;
			}
			case moveDir::Down:
			{
				cords.x = 0;
				cords.y = -10;
				break;
			}
		}
		Snake.push_back(sf::RectangleShape(sf::Vector2f(10, 10)));
		Snake[Snake.size() - 1].setPosition(lastPartPos + cords);
		moveDirections.push_back(lastPartDir);

		currentScoreValue++;
	}

	bool GameState::IsAppleEaten()
	{
		for (auto &n : Snake)
		{
			if (n.getPosition() == Apple.getPosition())
			{
				return true;
			}
		}
		return false;
	}
}