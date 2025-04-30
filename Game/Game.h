#pragma once
#include "../Animation/Animatable.h"
#include "../Animation/AnimationList.h"
#include "../Animation/ScreenShaker.h"

#include <SFML/Graphics.hpp>
#include <functional>

class Tile;
class GameWindow;
class Game final : public sf::Drawable, public sf::IntRect {
public:
  Game(GameWindow* window, unsigned short cols, unsigned short rows, unsigned short totalMines);
  ~Game() override;
  void handleTileClick(const sf::Vector2i& position,
                     const sf::Mouse::Button& button);
  void forEachAdjacentTile(const std::function<void(Tile&)>& callback, int row, int col) const;

  void win();
  void lose( int row, int col);
  // void recurseAdjacentTiles(const std::function<void(Tile&, int)>& callback, int row, int col);

  void pushAnimatable(Animatable* animatable);

  void update();
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  GameWindow* window;

  bool isOver = false;

  unsigned short hiddenTiles;


private:
  Tile*** _grid;
  unsigned short _rows;
  unsigned short _cols;
  unsigned short _totalMines;


  bool _isFirstTileRevealed = false;

  AnimationList<> _animations;
  ScreenShaker* _explosionShaker;

  void placeRandomMines(int mineCount) const;
  // void _recurseFunction(std::unordered_set<Tile*>& visited, Tile& tile, const std::function<void(Tile&, int)>& callback);
};