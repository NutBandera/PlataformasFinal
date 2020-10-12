#pragma once

#include "Actor.h"

class Tile : public Actor
{
public:
	Tile(string filename, float x, float y, Game* game);
	Tile(string filename, float x, float y, int lifeSeconds, Game* game);
	int lifeSeconds = 0;
	int getLifeSeconds();
	void reduceLife();
};
