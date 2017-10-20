/*
 * playing_state.cpp
 */

#include "playing_state.h"

PlayingState::PlayingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_1, TILES_TXT,
    &collisionDetector);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_1_ADD);

  setup();
  load();
}

void PlayingState::setup() {
  // Stairs 
  images[ele+"stairs"] = new Sprite(engine->renderer, STAIRS_FILENAME,
    errorHandler, map->width/2 - 45, map->height - 150, false);
  // Player 
  images[ppl+"king"] = new Character(engine->renderer, E_C_FILENAME,
    errorHandler, 16, 25, 80, 80, &eventHandler, &audioHandler, this);
  camera.setCharacter(static_cast<Character*>(images[ppl+"king"]));
  // Enemies
  std::ifstream file(LEVEL_1_E);
  int x = -1;
  int y = -1;
  while ((file >> y) && y != -1 && (file >> x) && x != -1) {
    std::string s = ppl+"enemy_"+std::to_string(num_enemies);
    images[s] = new Enemy(engine->renderer, E_C_FILENAME, errorHandler,
      16, 25, (x-1) * TILE_DIM, (y-1) * TILE_DIM, 0, 150);
    num_enemies++;
  }
  file.close();
  // Lights
  num_lights = map->pushLights(images);
  // Black
  images[add+"black"] = new Sprite(engine->renderer, BLACK_PIXEL, errorHandler,
    0, 0, false);
  // Score
  images[top+"score"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH - 114, 2, 16, "SCORE = " + std::to_string(engine->score));  
  // Heart types
  images[top+"heart_1"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 120, 34, false, true);
  images[top+"heart_2"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 80, 34, false, true);
  images[top+"heart_3"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 40, 34, false, true);
  // Stamina
  images[top+"sta_box"] = new Sprite(engine->renderer, STA_BOX, errorHandler,
    0, 34, false, true);
  images[top+"sta_bar"] = new Sprite(engine->renderer, STA_BAR, errorHandler,
    2, 38, false, true);
  // set key and coin pos
  double keyPosX = 0.0;
  double keyPosY = 0.0;
  double coinPosX = 0.0;
  double coinPosY = 0.0;
  Sprite *C1 = nullptr;
  Sprite *C2 = nullptr;
  for(auto tile : map->additions){
    if(tile.image->isChest()){
      if(keyPosX == 0.0 && keyPosY == 0.0){
        keyPosX = tile.image->pos_x;
        keyPosY = tile.image->pos_y;
        C1 = tile.image;
      } else {
        coinPosX = tile.image->pos_x;
        coinPosY = tile.image->pos_y;
        C2 = tile.image;
      }
    }
  }
  // add key
  images[add+"key"] = new Pickup(engine->renderer, KEY, errorHandler,
    32, 32, keyPosX, keyPosY, false, false, keyNum);
  static_cast<Sprite*>(images[add+"key"])->setPair(C1);
  // add coin
  images[add+"coin"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPosX, coinPosY, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin"])->setPair(C2);
  // FPS Counter 
  images[add+"fps"] = new Text(engine->renderer, FONT_FILENAME,  errorHandler,
    2, 2, 16, "FPS: ");
}

void PlayingState::load() {
  State::load();

  // set size of stairs & stam
  images[ele+"stairs"]->getDestRect()->h = 90;
  images[ele+"stairs"]->getDestRect()->w = 90;
  images[top+"sta_box"]->getDestRect()->h = 32;
  images[top+"sta_box"]->getDestRect()->w = 100;
  images[top+"sta_bar"]->getDestRect()->h = 24;
  images[top+"sta_bar"]->getDestRect()->w = 92;

  for (int i = 0; i < num_lights; i++) {
    std::string s = add+"light_"+std::to_string(i);

    images[s]->getDestRect()->w = TILE_DIM * 5;
    images[s]->getDestRect()->h = TILE_DIM * 5;

    SDL_SetTextureBlendMode(images[s]->getTexture(),SDL_BLENDMODE_ADD);
    if (SDL_SetTextureAlphaMod(images[s]->getTexture(), 80) < 0)
      errorHandler->quit(__func__, SDL_GetError());
  }

  images[add+"black"]->getDestRect()->w = map->width;
  images[add+"black"]->getDestRect()->h = map->height;

  SDL_SetTextureBlendMode(images[add+"black"]
    ->getTexture(), SDL_BLENDMODE_BLEND);
  if (SDL_SetTextureAlphaMod(images[add+"black"]->getTexture(), 150) < 0) {
    errorHandler->quit(__func__, SDL_GetError());
  }
}

void PlayingState::update(double seconds) {
  timer += seconds;

  if(Mix_PausedMusic() == 1){audioHandler.play("theme");}
  // update FPS Display
  if (timer > 1) {
    delete images[top+"fps"];
    images[top+"fps"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
      2, 2, 16, "FPS: " + std::to_string((int)(1/seconds)));
    images[top+"fps"]->load();
    timer = 0;
  }
  // update Score
  if(currentScore != engine->score){
    delete images[top+"score"];
    images[top+"score"] = new Text(engine->renderer, FONT_FILENAME,
      errorHandler, WIDTH-114, 2, 16, "SCORE = "+std::to_string(engine->score));  
    images[top+"score"]->load();
    currentScore = engine->score;
  }

  // updates Health
  switch(static_cast<Character*>(images[ppl+"king"])->hearts) {
    case 6:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 5:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 4:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 3:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 2:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 1:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(40, 0, 32, 32);
      break;
  }
  // update stamina
  updateSta();
  
  // enemy follows king if conditions met
  checkFollow();
  enemyFollow();

  SDL_SetTextureAlphaMod(images[add+"key"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"coin"]->getTexture(), 0);

  int x = 0;  
// set up inventory display
  for(Pickup *pUp : static_cast<Character*>(images[ppl+"king"])->inventory){  
    pUp->setFixed(true);
    pUp->setPosition(x * 40, 66);
    if(!pUp->isActivated()){
      SDL_SetTextureAlphaMod(pUp->getTexture(), 255);
      x++;
    }
  }
// shows contents of chest when open
  if(static_cast<Sprite*>(images[add+"key"])->pair->pair
    == static_cast<Sprite*>(images[add+"key"])->pair
    && !static_cast<Pickup*>(images[add+"key"])->isPickedUp())
  {
    SDL_SetTextureAlphaMod(images[add+"key"]->getTexture(), 255);
    static_cast<Character*>(images[ppl+"king"])
      ->inventory.push_back(static_cast<Pickup*>(images[add+"key"]));
    static_cast<Sprite*>(images[add+"key"])->pair
      = static_cast<Character*>(images[ppl+"king"]);
    static_cast<Pickup*>(images[add+"key"])->pickUp();
  }
  if(static_cast<Sprite*>(images[add+"coin"])->pair->pair
    == static_cast<Sprite*>(images[add+"coin"])->pair
    && !static_cast<Pickup*>(images[add+"coin"])->isPickedUp())
  {
    SDL_SetTextureAlphaMod(images[add+"coin"]->getTexture(), 255);
    static_cast<Character*>(images[ppl+"king"])
      ->inventory.push_back(static_cast<Pickup*>(images[add+"coin"]));
    static_cast<Sprite*>(images[add+"coin"])->pair
      = static_cast<Character*>(images[ppl+"king"]);
    engine->score += 1000;
    static_cast<Pickup*>(images[add+"coin"])->pickUp();
  }
  State::update(seconds);

  // changes state to Lose
  if (static_cast<Character*>(images[ppl+"king"])->hearts <= 0) {
    engine->setState("lose");
  }
  // changes state to Win
  if (images[ppl+"king"]->pos_x > map->width/2 - 45
      && images[ppl+"king"]->pos_x < map->width/2 + 45
      && images[ppl+"king"]->pos_y > map->height - 150)
  {
    std::ofstream file;
    file.open(SCORE_FILENAME, std::ios_base::app);
    file << std::to_string(engine->score) << std::endl;
    file.close();
    engine->setState("win");
  }
  // automatically win w/ '3'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    std::ofstream file;
    file.open(SCORE_FILENAME, std::ios_base::app);
    file << std::to_string(engine->score) << std::endl;
    file.close();
    engine->setState("win");
   }, SDLK_3);
  // automatically lose w/ '4'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
   engine->setState("lose"); }, SDLK_4);
}

void PlayingState::checkFollow() {
  int borderX = images[ppl+"king"]->pos_x
    + images[ppl+"king"]->getDestRect()->w+100;
  int borderY = images[ppl+"king"]->pos_y
    + images[ppl+"king"]->getDestRect()->h+100;
  for (int i = 0; i < num_enemies; i++) {
    std::string s = ppl+"enemy_"+std::to_string(i);
    if (!static_cast<Character*>(images[s])->running
        && images[s]->pos_x
        >= images[ppl+"king"]->pos_x - 100 - images[s]->getDestRect()->w
        && images[s]->pos_y
        >= images[ppl+"king"]->pos_y - 100 - images[s]->getDestRect()->h
        && images[s]->pos_x <= borderX && images[s]->pos_y <= borderY)
    {
      static_cast<Enemy*>(images[s])->following = true;
    }
  }
}
      
void PlayingState::enemyFollow() {
  for (int i = 0; i < num_enemies; i++) {
    std::string s = ppl+"enemy_"+std::to_string(i);
    if (static_cast<Enemy*>(images[s])->following) {
      // edit x velocity
      if (images[s]->pos_x
          < images[ppl+"king"]->pos_x-images[s]->getDestRect()->w+1)
      {
        images[s]->velocityX = 100;
      } else
      if (images[s]->pos_x
          > images[ppl+"king"]->pos_x+images[ppl+"king"]->getDestRect()->w-1)
      {
        images[s]->velocityX = -100;
      } else {
        images[s]->velocityX = 0;
      }
      // edit y velocity
      if (images[s]->pos_y + images[s]->getDestRect()->h / 2
          < images[ppl+"king"]->pos_y + images[ppl+"king"]->getDestRect()->h/2)
      {
        images[s]->velocityY = 100;
      } else
      if (images[s]->pos_y + images[s]->getDestRect()->h / 2 
          > images[ppl+"king"]->pos_y + images[ppl+"king"]->getDestRect()->h/2)
      {
        images[s]->velocityY = -100;
      } else {
        images[s]->velocityY = 0;
      } 
    }
  }
}

// inc/dec sta
void PlayingState::updateSta() {
  int w = int(static_cast<Character*>(images[ppl+"king"])->sta * 96);  
  images[top+"sta_bar"]->getDestRect()->w = w;
}

PlayingState::~PlayingState() {}
