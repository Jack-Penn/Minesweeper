#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "LeaderboardWindow.h"
#include "../UI Components/Button.h"
#include "../Game/Game.h"

class GameWindow final : public Window {
public:
    GameWindow(const int width, const int height, const sf::Font& font)
        : Window(width, height, sf::Color::White, "Minesweeper", "flag.png", {
             {
                sf::Event::MouseButtonPressed, [&](const sf::Event& event) {
                   const auto mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
                    if (_game->contains(mousePosition)) {
                        _game->handleTileClick(mousePosition, event.mouseButton.button);
                    } else if (_leaderboardButton.contains(mousePosition)) {
                        _leaderboardButton.handleClick(event.mouseButton.button);
                        sf::Event event_{};
                        while (pollEvent(event_)) {} //Clear Event Stack Queue
                    }
                }
             },
             {sf::Event::MouseMoved, [&](const sf::Event& event) {
                 const auto mousePosition = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (_game->contains(mousePosition)) {
                        _game->handleTileClick(mousePosition, sf::Mouse::Left);
                    }
                }
             }}
        }),
        _game(nullptr),
        _leaderboardButton(width - 176, height - 84, "leaderboard.png", [&, width, height] {
            static LeaderboardWindow leaderboard_window(width / 2, (height - 100) / 2 + 50, font);
            leaderboard_window.focus();
        }) {}

    void setGame(Game* game) {
        _game = game;
    }

    void _update() override {
        _game->update();
    }

    bool _shouldRedraw() override {
        return true;
    }

    void _draw() override {
        draw(_leaderboardButton);
        draw(*_game);
    }

private:
    Game* _game;
    Button _leaderboardButton;
};