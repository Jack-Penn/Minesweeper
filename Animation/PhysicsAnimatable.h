#pragma once

#include <SFML/Graphics.hpp>
#include "Animatable.h"

class PhysicsAnimatable : virtual public Animatable {
public:
  explicit PhysicsAnimatable(sf::Vector2f position, sf::Vector2u size = {0, 0}, sf::Vector2f velocity = sf::Vector2f(0, 0), sf::IntRect bounds = sf::IntRect(0, 0, 0, 0), bool boundsCollide = false);

  void update() override;
  virtual sf::Vector2u getSize() const;

private:
  static sf::Vector2f Acceleration;

protected:
  sf::Vector2f _position;
  sf::Vector2u _size;
  sf::Vector2f _velocity;
  sf::IntRect _bounds;
  bool _boundsCollide;
};