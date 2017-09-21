#include "menu_state.h"

MenuState::MenuState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

	setup();
  load();
}

void MenuState::setup() {
  images.push_back(new Image(engine->renderer, BG_FILENAME, errorHandler));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 45, WINDOW_TITLE, ROYAL_GOLD));
  images.push_back(new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 24, "New Game"));
}

void MenuState::load() {
  State::load();

  auto center = getCenterForImage(images[1]);
  images[1]->setPosition(std::get<0>(center), std::get<1>(center) - 90);

  center = getCenterForImage(images[2]);
  images[2]->setPosition(std::get<0>(center), std::get<1>(center) - 40);

  images[2]->onHover(&eventHandler, [] () {
    //images[2]->
  });
}

void MenuState::update(double seconds) {
  State::update(seconds);


  //std::cout << "Switched States" << std::endl;;
  //engine->setState("playing");
}

MenuState::~MenuState() {}

std::tuple<int, int> MenuState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}
