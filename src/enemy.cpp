/*
 * enemy.cpp
 */

#include "enemy.h"
#include "character.h"
//Definitions for the enemy class that inherits from Sprite

//constructor that takes in width and height for the rect
Enemy::Enemy(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  double velocity_X, double velocity_Y)
    : Sprite(renderer, filename, error_handler, width, height, pos_x,
    pos_y, true) {

    rect.w *= 2;
    rect.h *= 2;

    velocityX = velocity_X;
    velocityY = velocity_Y;
}

//constructor with no width and height arguments
Enemy::Enemy(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, double velocity_X,
  double velocity_Y)
    : Sprite(renderer, filename, error_handler, pos_x, pos_y, true) {

    velocityX = velocity_X;
    velocityY = velocity_Y;
}

void Enemy::freeze() {
  if (!frozen) {
    tempVX = velocityX;
    tempVY = velocityY;
    velocityX = 0;
    velocityY = 0;
    frozen = true;
  }
}

void Enemy::thaw() {
  frozen = false;
  velocityX = tempVX;
  velocityY = tempVY;
}

void Enemy::update(double seconds) {
  if (flipXVelocity) velocityX *= -1;
  if (flipYVelocity) velocityY *= -1;
  flipXVelocity = false;
  flipYVelocity = false;

  Sprite::update(seconds);

  if (this->dead) {
    velocityY = 0;
    velocityX = 0;
    collidable = false;
    SDL_SetTextureAlphaMod(this->getTexture(), 0);
    return;
  }

  if (velocityX > 0) {
    dir = "right";
    Sprite::animate(seconds, ENEMY_R_MOVING_POS, ENEMY_R_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityX < 0) {
    dir = "left";
    Sprite::animate(seconds, ENEMY_L_MOVING_POS, ENEMY_L_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityY > 0) {
    dir = "down";
    Sprite::animate(seconds, ENEMY_DOWN_MOVING_POS, ENEMY_DOWN_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityY < 0) {
    dir = "up";
    Sprite::animate(seconds, ENEMY_UP_MOVING_POS, ENEMY_UP_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  }
}

void Enemy::notifyCollision(Image* img, doubleRect* intersection, bool resolved) {
  if (!img->isSword() && !img->isEnemy()) Sprite::notifyCollision(img,
                                                  intersection, resolved);

  if(img->isEnemy() || img->isCharacter() || img->isSword()){
    return;
  }

  if (intersection->w > intersection->h) {
    flipYVelocity = true;
  } else {
    flipXVelocity = true;
  }
}

void Enemy::idleAnimation(double seconds) {
  int pos = -1;

  if (dir == "right") pos = ENEMY_R_MOVING_POS;
  else if (dir == "left") pos = ENEMY_L_MOVING_POS;
  else if (dir == "up") pos = ENEMY_UP_MOVING_POS;
  else if (dir == "down") pos = ENEMY_DOWN_MOVING_POS;
  else error_handler->quit(__func__, "direction not found");

  Sprite::animate(seconds, pos, pos + ENEMY_IDLE_FRAMES - 1);
}
