#pragma once
#include "Actor.h"
#include "Projectile.h"
#include "Animation.h"
#include "Audio.h"

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	void update();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0) override;
	int shootCadence = 20;
	int shootTime = 0;
	void jump();
	bool onAir;
	void loseLife();

	int life = 3;
	int invulnerableTime = 0;
	Projectile* shoot();
	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aShootingLeft;
	Animation* aShootingRight;
	Animation* aJumpingRight;
	Animation* aJumpingLeft;
	Animation* animation;
	Audio* audioShoot;

	int state;
	int orientation;
};

enum class State {
	IDLE,
	RUNNING
};


