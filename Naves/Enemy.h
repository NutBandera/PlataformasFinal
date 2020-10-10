#pragma once
#include "Actor.h"
#include "Animation.h"

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void draw(float scrollX = 0) override;

	void impacted();
	int state;

	float vxIntelligence;

	Animation* aMoving;
	Animation* aDying;
	Animation* animation;
};

