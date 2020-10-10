#pragma once
#include "Game.h"

class Actor {
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	virtual void draw(float scrollX = 0);
	bool isOverlap(Actor* actor);
	bool isInRender(float scrollX = 0);
	bool containsPoint(int pointX, int pointY);
	bool clicked = false;
	SDL_Texture* texture;
	float x;
	float y;
	float vx; // velocidad eje x
	float vy; // velocidad eje y
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	bool outRight;
	bool outLeft;
	Game* game;

	bool collisionDown = false;
};
