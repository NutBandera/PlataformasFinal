#include "Tile.h"

Tile::Tile(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 40, 32, game) {

}

Tile::Tile(string filename, float x, float y, int lifeSeconds, Game* game)
	: Actor(filename, x, y, 40, 32, game) {
	this->lifeSeconds = lifeSeconds;
}

int Tile::getLifeSeconds() {
	return lifeSeconds;
}

void Tile::reduceLife() {
	lifeSeconds--;
}
