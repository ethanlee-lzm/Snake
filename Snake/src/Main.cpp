#include <iostream>
#include "src/GameEngine/hpp/Game.hpp"
#include "DEFINE.hpp"

int main()
{
	hgw::Game(SCREEN_WIDTH, SCREEN_HEIGHT ,"Snake"); //crashes for state creation, we dont have states, crate state in game.cpp
	return 0;
}