/*
 * sword.cpp
 */

#include "sword.h"
#include "state.h"
#include "character.h"

// constructor that takes in arguments for the width and height for the rect
Sword::Sword(SDL_Renderer *renderer, std::string filename, ErrorHandler
  *errorHandler, int w, int h, int pos_x, int pos_y, Image *king_p,
  EventHandler *eventHandler, Audio *audioHandler, State *state_p)
    : Sprite(renderer, filename, errorHandler, w, h, pos_x, pos_y, true),
    audioHandler(audioHandler) {

  rect.w *= 2;
  rect.h *= 2;

  king = king_p;
  state = state_p;
  createListeners(eventHandler);
}

void Sword::update(double seconds) {
  attackingTimer += seconds;

  pos_x = king->pos_x - 40;
  pos_y = king->pos_y - 31;
  dir = static_cast<Character*>(king)->dir;
  
  if (lastAttack
      && attackingTimer > ATTACK_FRAMES/(CHARACTER_FPS*speedMultiplier)) {
    attacking = false;
    lastAttack = false;
  }

  Sprite::update(seconds);

  if (attacking) {
    switch (dir[0]) {
      case 'u':
        Sprite::animate(seconds, U_SWORD_POS, U_SWORD_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
        break;
      case 'd':
        Sprite::animate(seconds, D_SWORD_POS, D_SWORD_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
        break;
      case 'r':
        Sprite::animate(seconds, R_SWORD_POS, R_SWORD_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
        break;
      case 'l':
        Sprite::animate(seconds, L_SWORD_POS, L_SWORD_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
        break;
    }
  }
}

void Sword::render(Camera* camera, double interpol_alpha) {
  if (attacking) {
    SDL_SetTextureAlphaMod(texture, 255);
  } else {
    SDL_SetTextureAlphaMod(texture, 0);
  }
  Sprite::render(camera, interpol_alpha);
}

void Sword::notifyCollision(Image* image, SDL_Rect* intersection) {
  if (intersection) {}

  //check y position
  if (image->pos_x+32 > pos_x+4 && image->pos_x < pos_x+108) {
    if (image->pos_y+50 > pos_y+3 && image->pos_y+50 <= king->pos_y)
      collisionDir = "up";
    if (image->pos_y >= king->pos_y+50 && image->pos_y < pos_y+109)
      collisionDir = "down";
  }
  // check x positoin
  if (image->pos_y+50 > pos_y+3 && image->pos_y < pos_y+109) {
    if (image->pos_x+32 > pos_x+4 && image->pos_x+32 <= king->pos_x)
      collisionDir = "left";
    if (image->pos_x >= king->pos_x+32 && image->pos_x < pos_x+108)
      collisionDir = "right";
  }
  // enemies die when attacked
  if (attacking && image->isEnemy() && collisionDir == dir) {
    static_cast<Enemy*>(image)->kill();
    audioHandler->play("kill", 1);
    if (this->pair != nullptr) {
      this->pair->setCollidable(false);
      SDL_SetTextureAlphaMod(this->pair->getTexture(), 0);
      this->pair = nullptr;
      state->deactivateInstructionText();
    }
    static_cast<Character*>(king)->updateExp();
  }
}

void Sword::createListeners(EventHandler *eventHandler) {
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    attacking = true;}, SDLK_SPACE);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    lastAttack = true; attackingTimer = 0;}, SDLK_SPACE);
}

void Sword::cleanup() {}

Sword::~Sword() {}
