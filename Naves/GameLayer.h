#pragma once
#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Tile.h"
#include "Space.h"
#include "Audio.h"
#include "Pad.h"
#include <list>
#include <fstream>
#include <sstream>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;

	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	int mapWidth;
	list<Tile*> tiles;
	void keysToControls(SDL_Event event);
	int newEnemyTime = 0;
	void calculateScroll();
	float scrollX;
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	Actor* message;
	bool pause;

	// Elementos de interfaz
	Actor* buttonJump;
	Actor* buttonShoot;
	Pad* pad;



	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Space* space;
	Text* textPoints;
	Audio* audioBackground;
	Tile* cup; // Elemento de final de nivel

	int points;

	bool controlContinue = false;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	list<Enemy*> enemies;
	list<Projectile*> projectiles;
};