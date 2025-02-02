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
#include "Checkpoint.h"
#include "Recolectable.h"
#include "Checkpoint.h"
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
	Checkpoint* checkpoint;

	int points;

	Text* textItems;
	Actor* backgroundItems;
	int items;

	bool controlContinue = false;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	bool saved = false;

	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	list<Tile*> cajas;
	list<Tile*> trampolines;
	list<Tile*> bloquesDestruibles;
	list<Recolectable*> recolectables;
};