#pragma once
#include "Actor.h"
#include "Animation.h"

class Checkpoint :
	public Actor
{
public:
	Checkpoint(float x, float y, Game* game);
	void update();
	void draw(float scrollX = 0) override;
	Animation* animation = nullptr;
};

