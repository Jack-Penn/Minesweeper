#pragma once
#include "PhysicsAnimatable.h"
#include <cmath>
#include <iostream>

class PhysicsSprite final : virtual public PhysicsAnimatable {
public:
  explicit PhysicsSprite(const sf::Sprite sprite, sf::Vector2f velocity = sf::Vector2f(0,0), sf::IntRect bounds = sf::IntRect(0,0,0,0), bool boundsCollide = false)
      : PhysicsAnimatable(sprite.getPosition(), sf::Vector2u(sprite.getLocalBounds().getSize()), velocity, bounds, boundsCollide), _sprite(sprite) {}
  void update() override {
    PhysicsAnimatable::update();
    _sprite.setPosition(_position);
  }
  void draw(sf::RenderTarget& target, const sf::RenderStates states) const override {
    target.draw(_sprite, states);
  }
  bool isAnimationOver() const override {
    const sf::Vector2i boundsSize = _bounds.getSize();
    if (boundsSize.x == 0 && boundsSize.y == 0) return false;
    if (_boundsCollide) return std::pow(_velocity.x, 2) + std::pow(_velocity.y,2) == 0;
    return !_bounds.intersects(sf::IntRect(_sprite.getGlobalBounds()));
  }
private:
  std::function<void(sf::Vector2f&, unsigned int, unsigned int)> _update;
  std::function<void(sf::RenderTarget&, sf::RenderStates)> _draw;
  sf::Sprite _sprite;
};