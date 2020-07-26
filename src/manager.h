#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include "renderer.h"
#include "snake.h"
#include "controller.h"
#include "game.h"
#include <memory>
#include "SDL.h"

class Manager {
    public:
    Manager();
    void run();
    void ending();

    private:
    std::unique_ptr<Game> _game;
	std::unique_ptr<Controller> _controller;
    std::unique_ptr<Renderer> _renderer;
    //private methods
    int getGameScore() { return _game->GetScore(); }
    int getSnakeSize() { return _game->GetSize(); }

    std::size_t kFramesPerSecond{60};
	std::size_t kMsPerFrame{1000 / kFramesPerSecond};
	std::size_t kScreenWidth{640};
	std::size_t kScreenHeight{640};
	std::size_t kGridWidth{32};
	std::size_t kGridHeight{32};

};
#endif