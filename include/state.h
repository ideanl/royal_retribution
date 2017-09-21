#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <vector>
#include "engine.h"
#include "collision_detector.h"
#include "audio.h"
#include "text.h"

class State {
  public:
    State(Engine* engine, ErrorHandler* errorHandler);

    virtual void load();

    virtual void setup() = 0;

    virtual void run(double seconds);

    virtual ~State();
  protected:
    virtual void cleanup();
    std::vector<Image*> images;
    Engine* engine;
    Audio audioHandler;
    EventHandler eventHandler;
    ErrorHandler* errorHandler;
  private:

    virtual void update(double seconds);
    virtual void render();

    CollisionDetector* collisionDetector;
};

#endif