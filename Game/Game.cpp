#include "Game.h"
#include "../Windows/GameWindow.h"
#include "../Animation/Delay.h"
#include "../Animation/ScreenShaker.h"
#include "Tile.h"
#include "../util.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <random>

Game::Game(GameWindow* window, const unsigned short cols, const unsigned short rows, const unsigned short totalMines)
    : sf::IntRect(0, 0, cols*32, rows*32),
window(window), _cols(cols), _rows(rows), _totalMines(totalMines), hiddenTiles(cols*rows-totalMines) {
  _grid = new Tile **[_rows];
  for (unsigned short i = 0; i < _rows; i++) {
    _grid[i] = new Tile *[_cols];
    for (unsigned short j = 0; j < _cols; j++) {
      _grid[i][j] = new Tile(this, i, j, width/_cols);
    }
  }

 placeRandomMines(_totalMines);

  _explosionShaker = new ScreenShaker(reinterpret_cast<sf::RenderWindow*>(window), [&](const float intensity, const float) -> sf::Vector2i {
      static std::uniform_real_distribution<float> offset_dist(-1.0f, 1.0f);
      const int offsetX = std::floor(offset_dist(number_generator) * intensity);
      const int offsetY = std::floor(offset_dist(number_generator) * intensity);
      return {offsetX,offsetY};
    }, 30);
}

Game::~Game() {
  for (unsigned short i = 0; i < _rows; ++i) {
    for (unsigned short j = 0; j < _cols; ++j) {
      delete _grid[i][j];
    }
    delete[] _grid[i];
  }
  delete[] _grid;
}

void Game::update() {
  _animations.clean();
  _animations.update();
  _explosionShaker->update();
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (unsigned short i = 0; i < _rows; ++i) {
    for (unsigned short j = 0; j < _cols; ++j) {
      target.draw(*_grid[i][j], states);
    }
  }

  target.draw(_animations, states);
}

void Game::handleTileClick(const sf::Vector2i& position,
                     const sf::Mouse::Button& button) {
  if (isOver) return;
  const auto relativeMousePosition = position - getPosition();
  const auto gameSize = static_cast<sf::Vector2f>(getSize());
  const auto tileRow = static_cast<int>(relativeMousePosition.y / (gameSize.y/_rows));
  const auto tileCols = static_cast<int>(relativeMousePosition.x / (gameSize.x/_cols));
  Tile& clickedTile = *_grid[tileRow][tileCols];
  switch (button) {
    case sf::Mouse::Left:

      if (!clickedTile.getIsFlagged() && !clickedTile.getIsRevealed()) {
        if (!_isFirstTileRevealed) {
          _isFirstTileRevealed = true;
          window->_timer.resume();
          //First Click - Prevent Player from loosing
          if (clickedTile.getIsMine()) {
            placeRandomMines(1);
            clickedTile.removeMine();
          }
        }
        clickedTile.reveal();
      }
      break;
    case sf::Mouse::Right:
      if (!clickedTile.getIsRevealed()) {
        clickedTile.toggleFlagged();
        if (clickedTile.getIsFlagged())
          window->decrementCounter();
        else
          window->incrementCounter();
      }
      break;
    default:;
  }
}

void Game::forEachAdjacentTile(const std::function<void(Tile&)>& callback, const int row, const int col) const {
  static const sf::IntRect gridRect(0, 0, _cols, _rows);
  for (short dy = -1; dy < 2; dy++) {
    for (short dx = -1; dx < 2; dx++) {
      if (dy == 0 && dx == 0) continue;
      sf::Vector2i pos(col + dx, row + dy);
      if (gridRect.contains(pos)) {
        callback(*_grid[pos.y][pos.x]);
      }
    }
  }
}

void Game::win() {
  window->gameWin();
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      Tile* tile = _grid[i][j];
      if (!tile->getIsRevealed() && !tile->getIsFlagged()) {
        tile->toggleFlagged();
      }
    }
  }
  isOver = true;
}

void Game::lose(const int row, const int col) {
  isOver = true;
  window->gameLose();
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      if (i == row && j == col) continue;
      Tile* tile = _grid[i][j];
      if (tile->getIsMine() && !tile->getIsRevealed()) {
        const int dist = std::sqrt(std::pow(row-i, 2) + std::pow(col-j, 2));
        pushAnimatable(new Delay(dist*100, [&, tile] {
          tile->reveal();
          // pushAnimatable(new PhysicsSprite(tile, randomVector(100, 300,{{-150,-30}}), sf::IntRect({0,0}, sf::Vector2i(window->getSize()))));
          _explosionShaker->startShake(1000, 10);
        }));
      }
    }
  }
}

void Game::pushAnimatable(Animatable* animatable) {
  _animations.push(animatable);
}


void Game::placeRandomMines(const int mineCount) const {
  if (mineCount <= 0) return;

  // Create list of all possible tile positions
  std::vector<std::pair<int, int>> availablePositions;
  availablePositions.reserve(_cols * _rows);
  for (int row = 0; row < _rows; ++row) {
    for (int col = 0; col < _cols; ++col) {
      if (!_grid[row][col]->getIsMine()) {
        availablePositions.push_back({row, col});
      }
    }
  }

  if (availablePositions.empty()) return;

  std::uniform_int_distribution<> dist(0, static_cast<int>(availablePositions.size()) - 1);
  const int minesToPlace = std::min(mineCount, static_cast<int>(availablePositions.size()));
  for (int i = 0; i < minesToPlace; ++i) {
    const int randomIndex = dist(number_generator);
    const auto [row, col] = availablePositions[randomIndex];
    _grid[row][col]->addMine();
    std::swap(availablePositions[randomIndex], availablePositions.back());
    availablePositions.pop_back();
    dist.param(std::uniform_int_distribution<>::param_type(0, static_cast<int>(availablePositions.size()) - 1));
  }
}

