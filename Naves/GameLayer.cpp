#include "GameLayer.h"

GameLayer::GameLayer(Game *game)
	: Layer(game) {
	pause = true;
	message = new Actor("res/mensaje_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);
	init();
}

void GameLayer::init() {
	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);
	buttonJump = new Actor("res/boton_salto.png", WIDTH*0.9, HEIGHT*0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH*0.75, HEIGHT*0.83, 100, 100, game);

	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	space = new Space(1);
	scrollX = 0;
	
	items = 0;
	textItems = new Text("0", WIDTH * 0.81, HEIGHT * 0.05, game);
	textItems->content = to_string(items);
	backgroundItems = new Actor("res/item.png", WIDTH * 0.755, HEIGHT * 0.06, 24, 24, game);
	points = 0;
	textPoints = new Text("0", WIDTH * 0.92, HEIGHT * 0.05, game);
	textPoints->content = to_string(points);
	background = new Background("res/fondo_2.png", WIDTH*0.5, HEIGHT*0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png", WIDTH * 0.865, HEIGHT * 0.05, 24, 24, game);

	projectiles.clear(); // Vaciar por si reiniciamos el juego
	enemies.clear(); 
	tiles.clear();
	cajas.clear();
	trampolines.clear();
	recolectables.clear();
	bloquesDestruibles.clear();

	loadMap("res/" + to_string(game->currentLevel) + ".txt");

}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERAXISMOTION:
			game->input = GameInputType::GAMEPAD;
			break;
		case SDL_QUIT:
			game->loopActive = false;
			return;
			case SDL_MOUSEBUTTONDOWN:
				game->input = GameInputType::MOUSE;
				break;
			case SDL_KEYDOWN:
				game->input = GameInputType::KEYBOARD;
				break;
		}
		if (game->input == GameInputType::KEYBOARD) {
			keysToControls(event);
		}
		if (game->input == GameInputType::MOUSE) {
			mouseToControls(event);
		}
		
	}
	//procesar controles
	//Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
			space->addDynamicActor(newProjectile);
			controlShoot = false;
		}

	}

	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	/*if (controlMoveY > 0) {
		player->moveY(1);
	}*/
	if (controlMoveY < 0) {
		player->jump();
	}
	/*else {
		player->moveY(0);
	}*/

}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

			cout << character << endl;
		}
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case 'C': {
		cup = new Tile("res/copa.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		cup->y = cup->y - cup->height / 2;
		space->addDynamicActor(cup); // Realmente no hace falta
		break;
	}
	case 'E': {
		Enemy* enemy = new Enemy(x, y, game);
		// modificación para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}

	case '1': {
		if (saved) {
			player = new Player(checkpoint->x, checkpoint->y, game);
		}
		else {
			player = new Player(x, y, game);
		}
		// modificación para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/bloque_tierra.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'U': {
		Tile* tile = new Tile("res/box.png", x, y, game);
		tile->y = tile->y - tile->height / 2;
		cajas.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'Y': {
		Tile* tile = new Tile("res/trampolin.png", x, y, game);
		tile->y = tile->y - tile->height / 2;
		trampolines.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'R': {
		Recolectable* item = new Recolectable(x, y, game);
		item->y = item->y - item->height / 2;
		recolectables.push_back(item);
		space->addDynamicActor(item);
		break;
	}
	case 'W': {
		Tile* tile = new Tile("res/bloque_des.png", x, y, 30, game);
		tile->y = tile->y - tile->height / 2;
		bloquesDestruibles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'A': {
		checkpoint = new Checkpoint(x, y, game);
		checkpoint->y = checkpoint->y - checkpoint->height / 2;
		space->addDynamicActor(checkpoint);
		break;
	}
	}
}


void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_d: {// derecha
			controlMoveX = 1;
			break;
		}
		case SDLK_a: {// izquierda
			controlMoveX = -1;
			break;
		}
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}
	}
	

}

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = true;
			controlMoveX = pad->getOrientationX(motionX);
			if (controlMoveX > -20 && controlMoveX < 20) {
				controlMoveX = 0;
			}
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = -1;
		}
		
	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		if (pad->clicked && pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
		}
		else {
			pad->clicked = false;
			controlMoveX = 0;
		}
		if (buttonShoot->containsPoint(motionX, motionY) == false) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY) == false) {
			controlMoveY = 0;
		}

	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			controlMoveX = 0;
		}
	}
}



void GameLayer::update() {
	if (pause) {
		return;
	}

	if (player->isOverlap(checkpoint) && checkpoint->animation == nullptr) {
		checkpoint->animation = new Animation("res/checkpoint_flag.png", 
			checkpoint->width, checkpoint->height, 640, 64, 6, 10, true, game);
		saved = true;
	}

	// Nivel superado
	if (cup->isOverlap(player)) {
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5,
			WIDTH, HEIGHT, game);
		pause = true;
		saved = false;
		checkpoint->animation = nullptr;
		init();
	}

	if (player->y > HEIGHT + player->height) {
		init();
	}
	space->update();
	background->update();
	// Generar enemigos
	newEnemyTime--;
	/*if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		newEnemyTime = 110;
	}*/

	player->update();
	checkpoint->update();
	for (auto const& enemy : enemies) {
		enemy->update();
	}
	// Colisiones , Enemy - Player
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			player->loseLife();
			if (player->life <= 0){
				init();
			}
			return;
		}
	}
	// Colisiones , Enemy - Projectile

	list<Enemy*> deleteEnemies; // lista enlazada
	list<Projectile*> deleteProjectiles;
	list<Tile*> deleteCajas;

	for (auto const& caja : cajas) {
		for (auto const& projectile : projectiles) {
			if (caja->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				bool pInList2 = std::find(deleteCajas.begin(),
					deleteCajas.end(),
					caja) != deleteCajas.end();

				if (!pInList2) {
					deleteCajas.push_back(caja);
				}
				space->removeStaticActor(caja);
				points+=2;
				textPoints->content = to_string(points);
			}
		}
	}

	for (auto const& delCaja : deleteCajas) {
		cajas.remove(delCaja);
	}
	deleteCajas.clear();

	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				enemy->impacted();
				points++;
				textPoints->content = to_string(points);
			}
		}
	}

	for (auto const& trampolin : trampolines) {
		if (player->isOverlap(trampolin)) {
			player->jump(-20);
		}
	}

	list<Recolectable*> deleteItems;

	for (auto const& item : recolectables) {
		if (player->isOverlap(item)) {
			items++;
			textItems->content = to_string(items);
			bool rInList = std::find(deleteItems.begin(),
				deleteItems.end(),
				item) != deleteItems.end();

			if (!rInList) {
				deleteItems.push_back(item);
			}
		}
	}

	for (auto const& delItem : deleteItems) {
		recolectables.remove(delItem);
		space->removeDynamicActor(delItem);
	}
	deleteItems.clear();

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender(scrollX) == false || projectile->vx == 0) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	for (auto &const enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();
			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}

	list<Tile*> deleteBlocks;

	for (auto const& bloque : bloquesDestruibles) {
		if (player->isOverlap(bloque)) { // change to isOver
			bloque->reduceLife();
			if (bloque->getLifeSeconds() == 0) {
				bool bInList2 = std::find(deleteBlocks.begin(),
					deleteBlocks.end(),
					bloque) != deleteBlocks.end();

				if (!bInList2) {
					deleteBlocks.push_back(bloque);
				}
				space->removeStaticActor(bloque);
			}
		}
	}

	for (auto const& delBloque : deleteBlocks) {
		bloquesDestruibles.remove(delBloque);
	}
	deleteBlocks.clear();


	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();



	for (const auto& projectile: projectiles) {
		projectile->update();
	}

	for (const auto& item : recolectables) {
		item->update();
	}
}

void GameLayer::calculateScroll() {
	if (player->x > WIDTH * .3) {
		if (player->x - scrollX < WIDTH * .3) {
			scrollX = player->x - WIDTH * .3;
		}
	}
	if (player->x < mapWidth - WIDTH * .3) {
		if (player->x - scrollX > WIDTH * .7) {
			scrollX = player->x - WIDTH * .7;
		}
	}
}

void GameLayer::draw() {
	calculateScroll();
	background->draw(scrollX);

	for (const auto& tile : tiles) {
		tile->draw(scrollX);
	}

	for (const auto& caja : cajas) {
		caja->draw(scrollX);
	}

	for (const auto& trampolin : trampolines) {
		trampolin->draw(scrollX);
	}

	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX);
	}

	for (const auto& item : recolectables) {
		item->draw(scrollX);
	}

	for (const auto& bloque : bloquesDestruibles) {
		bloque->draw(scrollX);
	}

	cup->draw(scrollX);
	checkpoint->draw(scrollX);
	player->draw(scrollX);

	for (auto const& enemy : enemies) {
		enemy->draw(scrollX);
	}

	// HUD
	backgroundPoints->draw();
	textPoints->draw();
	backgroundItems->draw();
	textItems->draw();

	if (game->input == GameInputType::MOUSE) {
		buttonJump->draw(); // NO TIENEN SCROLL, POSICION FIJA
		buttonShoot->draw(); // NO TIENEN SCROLL, POSICION FIJA
		pad->draw();
	}

	if (pause) {
		message->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

