#include "enemy.h"

Enemy::Enemy(SDL_Renderer *renderer, SDL_Surface *surf, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  double velocity_X, double velocity_Y)
    : Sprite(renderer, surf, filename, error_handler, width, height, pos_x,
    pos_y) {
      velocityX = velocity_X;
      velocityY = velocity_Y;
}

Enemy::Enemy(SDL_Renderer *renderer, SDL_Surface *surf, std::string filename,
  ErrorHandler *error_handler, int pos_x, int pos_y,
  double velocity_X, double velocity_Y)
    : Sprite(renderer, surf, filename, error_handler, pos_x,
    pos_y) {
      velocityX = velocity_X;
      velocityY = velocity_Y;
}

void Enemy::update(double seconds) {
  Sprite::update(seconds);

  if (pos_x < 0 || pos_x + rect.w > WIDTH) {
    velocityX *= -1;
  }
  if (pos_y < 0 || pos_y + rect.h > HEIGHT) {
    velocityY *= -1;
  }
}
