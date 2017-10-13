#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_FPS             5
#define ENEMY_MOVING_FRAMES   4
#define ENEMY_IDLE_FRAMES     1
#define ENEMY_R_MOVING_POS    28
#define ENEMY_L_MOVING_POS    24
#define ENEMY_UP_MOVING_POS   20
#define ENEMY_DOWN_MOVING_POS 16

#include "sprite.h"
#include "constants.h"
#include "event_handler.h"

//Header file for enemy class that holds enemy position and its rect
class Enemy : public Sprite {
  public:
    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY);

    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int pos_x, int pos_y, double velocityX, double velocityY);

    virtual void update(double seconds);
    virtual void notifyCollision(Image*, SDL_Rect*);


    void kill() { dead = true; } 
    bool isEnemy() { return true; }
    bool isDead() { return dead; }

    bool following = false;

    std::string dir="right";
  private:
    bool dead = false;

    void idleAnimation(double);
};

#endif
