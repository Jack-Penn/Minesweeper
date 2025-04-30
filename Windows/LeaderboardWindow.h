#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "../util.h"
#include "Window.h"

class LeaderboardWindow final : public Window {
  public:
  	LeaderboardWindow(const int width, const int height, const sf::Font& font, int leaderboardIndex)
  : Window(width, height, sf::Color::Blue, "Leaderboard", "crown.png", {}), _titleText("LEADERBOARD", font, 20) {
      _titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
      setTextCenter(_titleText, width/2, height/2 - 120);

      std::string contentString = "";
      {
        int rank = 0;
        std::ifstream LeaderboardFile("../files/leaderboard.txt");
        if (!LeaderboardFile.is_open()) {
          std::cerr << "Couldn't open leaderboard file." << std::endl;
        }
        std::string line;
        while (std::getline(LeaderboardFile, line)) {
          const int breakIndex = line.find(",");
          std::string name = line.substr(breakIndex+2);
          contentString += std::to_string(rank +1) + ".\t" + line.substr(0, breakIndex) + "\t" + line.substr(breakIndex+2) + (rank == leaderboardIndex ? "*" : "") + "\n";
          rank++;
        }
        LeaderboardFile.close();
      }
      _contentText = sf::Text(contentString, font, 18);
      _contentText.setStyle(sf::Text::Bold);
      _contentText.setLineSpacing(2.5);
      _contentText.setOrigin(_contentText.getGlobalBounds().width/2.0f, 0);
      _contentText.setPosition(width/2, height/2-70);
  	}

protected:
    void _update() override {}

    bool _shouldRedraw() override {
  	  return false;
  	}

    void _draw() override {
      sf::RenderWindow::draw(_titleText);
      sf::RenderWindow::draw(_contentText);
    }
    private:
      sf::Text _titleText;
      sf::Text _contentText;
};