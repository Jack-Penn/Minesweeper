#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "Game/Game.h"
#include "Windows/WelcomeWindow.h"
#include "Windows/GameWindow.h"

int main() {
  //Load Config File Parameters
  int settings[3];
  {
    std::ifstream ConfigFile("../files/config.cfg");
    if (!ConfigFile.is_open()) {
      std::cerr << "Couldn't open Config file." << std::endl;
      return 1;
    }
    int i = 0;
    std::string line;
    while (std::getline(ConfigFile, line)) {
      settings[i++] = std::stoi(line);
    }
    ConfigFile.close();
  }
  const auto [numCols, numRows, numMines] = settings;

    sf::Font font;
    if (!font.loadFromFile("../files/font.ttf"))
      std::cout << "Error loading font" << std::endl;

    WelcomeWindow welcome_window(numCols * 32, numRows*32 + 100, font);
    welcome_window.focus();

    std::cout << "Submitted Name: " << welcome_window.player_name << std::endl;

    GameWindow gameWindow(numCols * 32, numRows*32 + 100, font);
    Game game(&gameWindow, numCols, numRows, numMines);
    gameWindow.setGame(&game);
    gameWindow.focus();
    return 0;
}