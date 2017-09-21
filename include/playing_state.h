#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "state.h"

class PlayingState : public State {
  public:
    PlayingState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~PlayingState();
  private:
    virtual void setup();
};

#endif