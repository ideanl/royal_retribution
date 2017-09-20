#ifndef CHARACTER_H
#define CHARACTER_H

#include "sprite.h"
#include "event_handler.h"
#include "constants.h"
#include "audio.h"

/*class for the player character that inherits from sprite will be 
responsible for player movement events and holds certain sfx along with
its position and rect*/
class Character : public Sprite {
  public:
    Character(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, EventHandler*, Audio*,
      int sheetFrames);

    Character(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
     int pos_x, int pos_y, EventHandler*, Audio*);

    virtual void notifyCollision(Image*, SDL_Rect* intersection);

    virtual void load();
    virtual void update(double seconds);
  private:
    void createListeners(EventHandler*);

    Audio *audioHandler;
};

#endif
