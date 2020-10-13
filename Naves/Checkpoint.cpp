#include "Checkpoint.h"

Checkpoint::Checkpoint(float x, float y, Game* game)
	: Actor("res/checkpoint.png", x, y, 64, 64, game) {
}

void Checkpoint::update() {
	if (animation != nullptr) {
		animation->update();
	}
}


void Checkpoint::draw(float scrollX) {
	if (animation != nullptr) {
		animation->draw(x - scrollX, y);
	}
	else {
		Actor::draw(scrollX);
	}
}


