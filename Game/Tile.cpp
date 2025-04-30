#include "Tile.h"

#include "../Animation/Delay.h"
#include "../Animation/Explosion.h"
#include "../Animation/PhysicsSprite.h"
#include "../Animation/SpriteAnimation.h"
#include "../TextureLoader.h"
#include "../Windows/GameWindow.h"
#include "../util.h"
#include "Game.h"

Tile::Tile(Game* game, const unsigned short row, const unsigned short col, const unsigned int width): _game(game), _row(row), _col(col), _size(width) {
  _tile.setTexture(*tile_hidden);
  _flag.setTexture(*flag);
  setPosition(col*width, row*width);
  setSize(width);
}

int Tile::getRow() const {
  return _row;
}
int Tile::getCol() const {
  return _col;
}

void Tile::setPosition(float x, float y) {
  _tile.setPosition(x, y);
  _flag.setPosition(x, y);
  _overlay.setPosition(x, y);
}
void Tile::setSize(float w) {
  const auto textureWidth = static_cast<float>(tile_hidden->getSize().x);
  _tile.setScale(w/textureWidth, w/textureWidth);
  _flag.setScale(w/textureWidth, w/textureWidth);
  _overlay.setScale(w/textureWidth, w/textureWidth);
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const{
  if (_game->window->isPauseActive) {
    sf::Sprite temp(_tile);
    temp.setTexture(*tile_revealed);
    target.draw(temp, states);
    return;
  }

  target.draw(_tile, states);
  if (_isFlagged)
    target.draw(_flag, states);
  if ((_isRevealed && _adjacentMines != 0) || (_isMine && _game->window->isDebugActive))
    target.draw(_overlay, states);

}

void Tile::addMine() {
  _isMine = true;
  _game->forEachAdjacentTile([](Tile& adjTile) {
    adjTile._adjacentMines++;
    if (!adjTile._isMine)
      adjTile._setNumberOverlay();
  }, _row, _col);
  _overlay.setTexture(*mine);
}

void Tile::removeMine() {
  _isMine = false;
  _game->forEachAdjacentTile([](Tile& adjTile) {
    adjTile._adjacentMines--;
    if (!adjTile._isMine)
      adjTile._setNumberOverlay();
  }, _row, _col);
  _setNumberOverlay();
}

bool Tile::getIsMine() const {
  return _isMine;
}

bool Tile::toggleFlagged() {
  _isFlagged = !_isFlagged;
  return _isFlagged;
}

bool Tile::getIsFlagged() const {
  return _isFlagged;
}

void Tile::reveal() {
  _isRevealed = true;
  _tile.setTexture(*tile_revealed);

  if (_isMine) {
    _game->pushAnimatable(new Explosion(_tile.getPosition()));

    if (!_game->isOver)
      _game->lose(_row, _col);
  } else {
    if (!_game->isOver) {
      _game->hiddenTiles--;
      if (_game->hiddenTiles == 0) {
        _game->win();
      }
    }

    if (_adjacentMines == 0 && !_game->window->isPauseActive) {
      _game->pushAnimatable(new PhysicsSprite(_tile, randomVector(100, 300,{{-150,-30}}), sf::IntRect({0,0}, sf::Vector2i(_game->window->getSize()))));
      _game->forEachAdjacentTile([this](Tile& adjTile) {
       if (!adjTile._isMine && !adjTile._isFlagged && !adjTile._isRevealed) {
         adjTile._isRevealed = true;
         _game->pushAnimatable(new Delay(200, [&adjTile] {
           adjTile.reveal();
         }));
       }
     }, _row, _col);
    }
  }
  }

bool Tile::getIsRevealed() const {
  return  _isRevealed;
}

void Tile::_setNumberOverlay() {
  if (_adjacentMines)
    _overlay.setTexture(*numbers[_adjacentMines-1]);
}

sf::Texture* Tile::tile_hidden = TextureLoader::loadTexture("tile_hidden.png");
sf::Texture* Tile::tile_revealed = TextureLoader::loadTexture("tile_revealed.png");
sf::Texture* Tile::mine = TextureLoader::loadTexture("mine.png");
sf::Texture* Tile::flag = TextureLoader::loadTexture("flag.png");
sf::Texture* Tile::explosionSpritesheet = TextureLoader::loadTexture("explosion.png");



sf::Texture* Tile::numbers[8] = {
  TextureLoader::loadTexture("number_1.png"),
  TextureLoader::loadTexture("number_2.png"),
  TextureLoader::loadTexture("number_3.png"),
  TextureLoader::loadTexture("number_4.png"),
  TextureLoader::loadTexture("number_5.png"),
  TextureLoader::loadTexture("number_6.png"),
  TextureLoader::loadTexture("number_7.png"),
  TextureLoader::loadTexture("number_8.png")
};