#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 50, 57, game) {
	aIdleLeft = new Animation("res/jugador_idle_izquierda.png", width, height,
		320, 40, 6, 8, true, game);
	aIdleRight = new Animation("res/jugador_idle_derecha.png", width, height,
		320, 40, 6, 8, true, game);
	animation = aIdleRight;
	aRunningRight = new Animation("res/jugador_corriendo_derecha.png", width, height,
		320, 40, 6, 8, true, game);
	aRunningLeft = new Animation("res/jugador_corriendo_izquierda.png", width, height,
		320, 40, 6, 8, true, game);
	aShootingRight = new Animation("res/jugador_disparando_derecha.png", width, height,
		160, 40, 6, 4, false, game);
	aShootingLeft = new Animation("res/jugador_disparando_izquierda.png", width, height,
		160, 40, 6, 4, false, game);
	aJumpingLeft = new Animation("res/jugador_saltando_izquierda.png", width, height,
		160, 40, 6, 4, true, game);
	aJumpingRight = new Animation("res/jugador_saltando_derecha.png", width, height,
		160, 40, 6, 4, true, game);

	audioShoot = new Audio("res/efecto_disparo.wav", false);

	state = game->stateMoving;
	orientation = game->orientationRight;

}

void Player::update() {
	if (onAir && state == game->stateMoving) {
		state = game->stateJumping;
	}
	if (!onAir && state == game->stateJumping) {
		state = game->stateMoving;
	}
	if (invulnerableTime > 0) {
		invulnerableTime--;
	}

	bool endAnimation = animation->update();


	if (collisionDown == true) {
		onAir = false;
	}
	else {
		onAir = true;
	}


	if (endAnimation) {
		if (state == game->stateShooting) {
			state = game->stateMoving;
		}
	}

	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}

	if (state == game->stateShooting) {
		if (orientation == game->orientationRight) {
			animation = aShootingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aShootingLeft;
		}
	}

	if (state == game->stateMoving) {
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aRunningLeft;
			}
		}
		if (vx == 0) {
			if (orientation == game->orientationRight) {
				animation = aIdleRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aIdleLeft;
			}
		}
	}

	if (state == game->stateJumping) {
		if (orientation == game->orientationRight) {
			animation = aJumpingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aJumpingLeft;
		}
	}
	
	if (shootTime > 0) {
		shootTime--;
	}

	//x = x + vx;
	//y = y + vy;
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Projectile* Player::shoot() {
	if (shootTime == 0) {
		audioShoot->play();
		shootTime = shootCadence;
		state = game->stateShooting;
		auto projectile = new Projectile(x, y, game);
		if (orientation == game->orientationLeft) {
			projectile->vx = projectile->vx * -1;
		}

		return projectile;
	}
	else {
		return NULL;
	}
	
}

void Player::draw(float scrollX) {
	if (invulnerableTime == 0) {
		animation->draw(x - scrollX, y);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5){
			animation->draw(x - scrollX, y);
		}
	}
}

void Player::jump(int v) {
	if (!onAir) {
		vy = v;
		onAir = true;
	}
}

void Player::loseLife() {
	if (invulnerableTime <= 0 && life > 0) {
		life--;
		invulnerableTime = 100;
	} 
}



