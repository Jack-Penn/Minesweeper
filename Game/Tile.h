#pragma once

#include <SFML/Graphics.hpp>
#include "../Animation/PhysicsSprite.h"

class Game;
class Tile final : public sf::Drawable {
public:
    Tile(Game* game, unsigned short row, unsigned short col, unsigned int width);
    int getRow() const;
    int getCol() const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(float x, float y);
    void setSize(float w);
    void addMine();
    void removeMine();
    bool getIsMine() const;
    bool toggleFlagged();
    bool getIsFlagged() const;
    void reveal();
    bool getIsRevealed() const;
private:
    Game* _game;
    unsigned short _row;
    unsigned short _col;
    unsigned short _size;

    sf::Sprite _tile;
    sf::Sprite _flag;
    sf::Sprite _overlay;
    void _setNumberOverlay();
    bool _isRevealed = false;
    bool _isMine = false;
    bool _isFlagged = false;
    unsigned short _adjacentMines = 0;

    static sf::Texture* tile_hidden;
    static sf::Texture* tile_revealed;
    static sf::Texture* mine;
    static sf::Texture* flag;
    static sf::Texture* numbers[8];
    static sf::Texture* explosionSpritesheet;
};