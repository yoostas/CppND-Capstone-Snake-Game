#include <iostream>
#include "manager.h"

int main() {
  auto manager = std::make_unique<Manager>();
  manager.get()->run();
  std::cout << "Game has terminated successfully!\n";
  manager->ending();
  return 0;
}