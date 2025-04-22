#pragma once

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "Animatable.h"
#include "../util.h"

class SpriteAnimation: public Animatable {
  public:
    sf::Sprite sprite;
    SpriteAnimation(const sf::Texture& spritesheet, const unsigned int cellWidth, const unsigned int animationDuration, const bool shouldLoop)
        : sprite(spritesheet, sf::IntRect(0,0,cellWidth, spritesheet.getSize().y)), _cellWidth(cellWidth), _cellHeight(spritesheet.getSize().y), _frameCount(spritesheet.getSize().x/cellWidth), _framePeriod(animationDuration / _frameCount), _shouldLoop(shouldLoop) {
    }
    void update() override {
      Animatable::update();
      if (_shouldLoop) {
        _index = getAge()/_framePeriod % _frameCount;
      } else {
        _index = std::min(_frameCount-1, getAge()/_framePeriod);
      }
      sprite.setTextureRect(sf::IntRect(_index * _cellWidth, 0, _cellWidth, _cellHeight));
    }
    void draw(sf::RenderTarget& target, const sf::RenderStates states) const override {
      target.draw(sprite, states);
    }
    bool isAnimationOver() const override {
      return !_shouldLoop && getAge() > _frameCount * _framePeriod;
    }
    void setFrameIndex(const size_t& index) {
      _index = index;
    }
    [[nodiscard]] size_t getFrameIndex() const {
      return _index;
    }
    void centerSprite(const sf::Vector2i& offset = {0,0}) {
      sprite.setOrigin(sprite.getLocalBounds().getSize()/2.0f + nonuniformScale(sf::Vector2f(offset), sprite.getScale()));
    }
   private:
     size_t _index = 0;
     unsigned int _cellWidth;
     unsigned int _cellHeight;
     unsigned int _frameCount;
     unsigned int _framePeriod;
     bool _shouldLoop;
};
