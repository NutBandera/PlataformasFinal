#include "Projectile.h"

Projectile::Projectile(float x, float y, Game* game) :
	Actor("res/disparo_jugador2.png", x, y, 20, 20, game) {
	vx = 9;
	vy = -1;
}

void Projectile::update() {
	//x = x + vx;
	vy = vy - 1;
}
