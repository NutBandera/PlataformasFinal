#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 36, 40, game) {
	vxIntelligence = -1;
	vx = vxIntelligence;
	aMoving = new Animation("res/enemigo_movimiento.png", width, height, 108, 40, 6, 3, true, game);
	aDying = new Animation("res/enemigo_morir.png", width, height, 280, 40, 6, 8, false, game);
	animation = aMoving;
	state = game->stateMoving;
}

void Enemy::update() {
	bool endaAnimation = animation->update();
	if (endaAnimation) {
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}
	
	/*vx = -1;
	x = x + vx;*/

	if (state == game->stateMoving) {
		animation = aMoving;
	}

	if (state == game->stateDying) {
		animation = aDying;
	}

	if (state != game->stateDying) {
		//x = x + vx;
		/*if (x - width / 2 <= 0 || x + width / 2 >= WIDTH) {
			vx = vx * -1;
		}*/
		if (vx == 0) {
			vxIntelligence *= -1;
			vx = vxIntelligence;
		}
		if (outRight) {
			// mover hacia la izquierda vx tiene que ser negativa
			if (vxIntelligence > 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}
		if (outLeft) {
			// mover hacia la derecha vx tiene que ser positiva
			if (vxIntelligence < 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}

	}
	else {
		vx = 0;
	}

}

void Enemy::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}

void Enemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}
