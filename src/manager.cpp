#include "manager.h"
#include "game.h"
#include "controller.h"

Manager::Manager() {
	_renderer = std::make_unique<Renderer>(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
	_controller = std::make_unique<Controller>();
    _game = std::make_unique<Game>(kGridWidth, kGridHeight);
};

void Manager::run() {
    _game->Run(*_controller, *_renderer, kMsPerFrame);
}

void Manager::ending() {
    std::cout << "Score: " << getGameScore() << "\n";
    std::cout << "Size: " << getSnakeSize() << "\n";
}

