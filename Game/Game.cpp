#include "Game.h"
#include "../Animation/Delay.h"
#include "Tile.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <random>

Game::Game(GameWindow* window, const unsigned short cols, const unsigned short rows, const unsigned short totalMines)
    : sf::IntRect(0, 0, cols*32, rows*32),
      window(window), _cols(cols), _rows(rows), _totalMines(totalMines) {
  _grid = new Tile **[_rows];
  for (unsigned short i = 0; i < _rows; i++) {
    _grid[i] = new Tile *[_cols];
    for (unsigned short j = 0; j < _cols; j++) {
      _grid[i][j] = new Tile(this, i, j, width/_cols);
    }
  }

 placeRandomMines(_totalMines);
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
          //First Click - Prevent Player from loosing
          while (clickedTile.getIsMine()) {
            clickedTile.removeMine();
            placeRandomMines(1);
          }
        }
        clickedTile.reveal();
      }
      break;
    case sf::Mouse::Right:
      if (!clickedTile.getIsRevealed())
        clickedTile.toggleFlagged();
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
void Game::lose(const int row, const int col) {
  isOver = true;
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      if (i == row && j == col) continue;
      Tile* tile = _grid[i][j];
      if (tile->getIsMine() && !tile->getIsRevealed()) {
        const int dist = std::sqrt(std::pow(row-i, 2) + std::pow(col-j, 2));
        pushAnimatable(new Delay(dist*100, [tile] {
          tile->reveal();
        }));
      }
    }
  }
}

// void Game::recurseAdjacentTiles(const std::function<void(Tile&, int)>& callback, const int row, const int col) {
//   std::unordered_set<Tile*> visited;
//   forEachAdjacentTile([&](Tile& adjTile) { _recurseFunction(visited, adjTile, callback); }, row, col);
// }
// void Game::_recurseFunction(std::unordered_set<Tile*>& visited, Tile& tile, const std::function<void(Tile&, int)>& callback) {
//   // static int count = 0;
//   if (visited.find(&tile) != visited.end()) return;
//   // std::cout << visited.size() << std::endl;
//   // std::cout << "(" << tile.getCol() << ", " << tile.getRow() << ")" << std::endl;
//   callback(tile, visited.size());
//   visited.insert(&tile);
//   forEachAdjacentTile([&](Tile& adjTile) { _recurseFunction(visited, adjTile, callback); }, tile.getRow(), tile.getCol());
// }

void Game::pushAnimatable(Animatable* animatable) {
  _animations.push(animatable);
}


void Game::placeRandomMines(const int mineCount) const {
  static std::mt19937 number_generator(3504);
  static std::uniform_int_distribution<> cols_dist(0, _cols-1);
  static std::uniform_int_distribution<> rows_dist(0, _rows-1);
  unsigned short current_mines = 0;
  while (current_mines < mineCount) {
    const int col = cols_dist(number_generator);
    const int row = rows_dist(number_generator);
    if (!_grid[row][col]->getIsMine()) {
      current_mines++;
      _grid[row][col]->addMine();
    }
  }
}