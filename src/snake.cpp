#include "snake.h"
#include <cmath>
#include <iostream>

//Rule of Five implementation

Snake::Snake(const Snake &sourceSnake) {
    std::unique_lock<std::mutex> mLock(_mtx);
    std::unique_lock<std::mutex> mLockSource(sourceSnake._mtx);
    speed = sourceSnake.speed;
    grid_height = sourceSnake.grid_height;
    grid_width = sourceSnake.grid_width;
    size = sourceSnake.size;
    alive = sourceSnake.alive;
    head_x = sourceSnake.head_x;
    head_y = sourceSnake.head_y;
    growing = sourceSnake.growing;
    body = sourceSnake.body;
}


Snake::Snake(Snake &&sourceSnake) {
    std::unique_lock<std::mutex> mLock(_mtx);
    std::unique_lock<std::mutex> mLockSource(sourceSnake._mtx);
    grid_height = sourceSnake.grid_height;
    grid_width = sourceSnake.grid_width;
    speed = sourceSnake.speed;
    size = sourceSnake.size;
    alive = sourceSnake.alive;
    head_x = sourceSnake.head_x;
    head_y = sourceSnake.head_y;
    growing = sourceSnake.growing;
    body = std::move(sourceSnake.body);
}

Snake &Snake::operator=(const Snake &sourceSnake) {
  	if (this == &sourceSnake)
        return *this;
    std::unique_lock<std::mutex> mLock(_mtx);
    std::unique_lock<std::mutex> mLockSource(sourceSnake._mtx);
    grid_height = sourceSnake.grid_height;
    grid_width = sourceSnake.grid_width;
    speed = sourceSnake.speed;
    size = sourceSnake.size;
    alive = sourceSnake.alive;
    head_x = sourceSnake.head_x;
    head_y = sourceSnake.head_y;
    growing = sourceSnake.growing;
    body = sourceSnake.body;
    return *this;
}


Snake &Snake::operator=(Snake &&sourceSnake) { 
    if (this == &sourceSnake) return *this;
    std::unique_lock<std::mutex> mLock(_mtx);
    std::unique_lock<std::mutex> mLockSource(sourceSnake._mtx);
    grid_height = sourceSnake.grid_height;
    grid_width = sourceSnake.grid_width;
    speed = sourceSnake.speed;
    size = sourceSnake.size;
    alive = sourceSnake.alive;
    head_x = sourceSnake.head_x;
    head_y = sourceSnake.head_y;
    growing = sourceSnake.growing;
    body = std::move(sourceSnake.body);
    return *this;
}

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::setDirection(Direction newDirection) {
  Snake::Direction opposite;
  switch(newDirection) {
    case Snake::Direction::kDown: 
      opposite = Snake::Direction::kUp; 
      break;
    case Snake::Direction::kUp: 
      opposite = Snake::Direction::kDown;
      break;
    case Snake::Direction::kLeft: 
      opposite = Snake::Direction::kRight; 
      break;
    case Snake::Direction::kRight: 
      opposite = Snake::Direction::kLeft;
      break;
  }
  std::unique_lock<std::mutex> lock(_mtx);
  if (direction != opposite || size == 1) 
    direction = newDirection;
  return;
}

void Snake::UpdateHead() {
  std::unique_lock<std::mutex> lock(_mtx);
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  //head_x = fmod(head_x + grid_width, grid_width);
 // head_y = fmod(head_y + grid_height, grid_height);
  if( (head_x  + grid_width <= grid_width) || (grid_width - head_x  < 1) || (head_y  + grid_height <= grid_height) || (grid_height - head_y < 1) ) {
     std::cout << "BOOM!!... " << std::endl;
     alive = false;
  }
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  std::unique_lock<std::mutex> lock(_mtx);
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { 
  std::unique_lock<std::mutex> lock(_mtx);
  growing = true; 
}

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  std::unique_lock<std::mutex> lock(_mtx);
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}