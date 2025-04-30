#pragma once
#include "../Game/Game.h"
#include "../Game/Timer.h"
#include "../UI Components/Button.h"
#include "../UI Components/Counter.h"
#include "LeaderboardWindow.h"
#include "Window.h"

#include <SFML/Graphics.hpp>
#include <array>
#include <thread>

class GameWindow final : public Window {
public:
    GameWindow(const int numCols, const int numRows, const int numMines, const sf::Font& font, const char playerName[])
        : Window(numCols * 32, numRows*32 + 100, sf::Color::White, "Minesweeper", "flag.png", {
             {
                sf::Event::MouseButtonPressed, [&](const sf::Event& event) {
                   const auto mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
                    if (_game->contains(mousePosition)) {
                        if (!isPauseActive) {
                            _game->handleTileClick(mousePosition, event.mouseButton.button);
                        }
                    } else if (_leaderboardButton.contains(mousePosition)) {
                        _leaderboardButton.handleClick(event.mouseButton.button);
                        sf::Event event_{};
                        while (pollEvent(event_)) {} //Clear Event Stack Queue
                    } else {
                        for (Button button : {_faceButton, _debugButton, static_cast<Button>(_pauseButton)}) {
                            if (button.contains(mousePosition))
                                button.handleClick(event.mouseButton.button);
                        }
                    }
                }
             },
             {sf::Event::MouseMoved, [&](const sf::Event& event) {
                 const auto mousePosition = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (_game->contains(mousePosition) && !isPauseActive) {
                        _game->handleTileClick(mousePosition, sf::Mouse::Left);
                    }
                }
             }}
        }),
        _playerName(playerName),
        _game(nullptr),
        _faceButton(numCols * 32/2.0 -32, (numRows + 0.5) * 32, "face_happy.png", [this, numCols, numRows, numMines] {
            if (isPauseActive) {
                isPauseActive = false;
                _pauseButton.setTexture(*TextureLoader::loadTexture("pause.png"));
            }
            delete _game;
            _game = new Game(this, numCols, numRows, numMines);
            _faceButton.setTexture(*TextureLoader::loadTexture("face_happy.png"));
            _mineCounter = Counter(33, 32 * (numRows +0.5) + 16, numMines, 3);
            _timer = Timer(numCols * 32 - 97,32* (numRows +0.5) + 16);
        }),
        _debugButton(numCols * 32 -304, (numRows + 0.5) * 32, "debug.png", [this] {
            if (!isPauseActive)
                isDebugActive = !isDebugActive;
        }),
        _pauseButton(numCols * 32 -240, (numRows + 0.5) * 32, "pause.png", [this]() {
            if (!_game->isOver) {
                isPauseActive = !isPauseActive;
                if (isPauseActive) {
                    _timer.pause();
                    _pauseButton.setTexture(*TextureLoader::loadTexture("play.png"));
                } else {
                    if (_timer.getElapsedTime() != 0)
                        _timer.resume();
                    _pauseButton.setTexture(*TextureLoader::loadTexture("pause.png"));
                }
            }

        }),
    _leaderboardButton(numCols * 32 - 176, (numRows + 0.5) * 32, "leaderboard.png", [&font, this, numCols, numRows] {
        // Save original pause state and timer status
    bool wasPaused = isPauseActive;

    if (!_game->isOver) {
        isPauseActive = true;
        _timer.pause();
        _update();
        _draw();
        display();
    }

    LeaderboardWindow leaderboard_window(numCols * 32 / 2, (numRows * 32) / 2 + 50, font, _leaderboardIndex);
    leaderboard_window.focus();

    if (!_game->isOver) {
        isPauseActive = wasPaused;
        if (wasPaused) {
            _timer.pause();
        } else {
            _timer.resume();
        }
        _update();
        _draw();
        display();
    }
    }),
        _mineCounter(33, 32* (numRows +0.5) + 16, numMines, 3),
        _timer(numCols * 32 - 97,32* (numRows +0.5) + 16)
        {
        _game = new Game(this, numCols, numRows, numMines);
    }
    

    void _update() override {
        _game->update();
        _timer.update();
    }

    bool _shouldRedraw() override {
        return true;
    }

    void _draw() override {
        draw(_faceButton);
        draw(_debugButton);
        draw(_pauseButton);
        draw(_leaderboardButton);
        draw(*_game);
        draw(_mineCounter);
        draw(_timer);
    }

    void gameWin() {
        _faceButton.setTexture(*TextureLoader::loadTexture("face_win.png"));
        _timer.pause();
        const int totalTime = _timer.getElapsedTime();
        std::cout << "Total Time: " << totalTime << std::endl;

        std::cout << "Updating Leaderboard" << std::endl;

        std::ifstream file("../files/leaderboard.txt");
        if (!file.is_open()) {
            std::cerr << "Couldn't open leaderboard file." << std::endl;
            return;
        }
        std::array<std::pair<int, std::string>, 5> leaderboard;
        std::string line;
        int i = 0;
        while (std::getline(file, line)) {
            const int breakIndex = line.find(",");
            std::string timeStr = line.substr(0, breakIndex);
            std::string name = line.substr(breakIndex + 2);
            int timeMs = timeToMilliseconds(timeStr);
            leaderboard[i++] = {timeMs, name};
            std::cout << name << " " << timeMs << std::endl;
        }
        file.close();

        i = 0;
        for (; i < leaderboard.size(); i++) {
            if (leaderboard[i].first > totalTime) {
                std::cout << "Updating Index: " << i << std::endl;
                for (int j = leaderboard.size() - 1; j > i; j--) {
                    leaderboard[j] = leaderboard[j-1];
                }
                leaderboard[i] = {totalTime, _playerName};
                break;
            }
        }

       if (i != leaderboard.size()) {
           _leaderboardIndex = i;
           std::cout << "Writing File" << std::endl;
           std::ofstream outFile("../files/leaderboard.txt");
           if (!outFile.is_open()) {
               std::cerr << "Couldn't open leaderboard file for writing." << std::endl;
               return;
           }

           for (const auto& entry : leaderboard) {
               int minutes = entry.first / 60000;
               int seconds = (entry.first / 1000) % 60;
               outFile << (minutes < 10 ? "0" : "") << minutes << ":"
                       << (seconds < 10 ? "0" : "") << seconds << ", " << entry.second << "\n";
           }

           outFile.close();
       } else {
           _leaderboardIndex == -1;
       }
    }

    void gameLose() {
        _faceButton.setTexture(*TextureLoader::loadTexture("face_lose.png"));
        _timer.pause();
    }

    void incrementCounter() {
        _mineCounter.setNumber(_mineCounter.getNumber() + 1 );
    }

    void decrementCounter() {
        _mineCounter.setNumber(_mineCounter.getNumber() - 1 );
    }

    bool isDebugActive = false;
    bool isPauseActive = false;
    Timer _timer;

private:
    const char *_playerName;
    int _leaderboardIndex = -1;

    Game* _game;

    Button _faceButton;
    Button _debugButton;
    Button _pauseButton;
    Button _leaderboardButton;

    Counter _mineCounter;
};