#include "PhysicsAnimatable.h"

sf::Vector2f PhysicsAnimatable::Acceleration(0, 1000);

PhysicsAnimatable::PhysicsAnimatable(const sf::Vector2f position, const sf::Vector2u size, const sf::Vector2f velocity, const sf::IntRect bounds, const bool boundsCollide)
    : _position(position), _size(size), _velocity(velocity), _bounds(bounds), _boundsCollide(boundsCollide) {}

void PhysicsAnimatable::update() {
  const float dt = getDeltaTime() / 1000.0;
  Animatable::update();

  _velocity += Acceleration * dt;
  _position += _velocity * dt;

  const sf::Vector2i boundsSize = _bounds.getSize();
  if (boundsSize.x != 0 && boundsSize.y != 0 && _boundsCollide) {
    const sf::Vector2i boundsOffset = _bounds.getPosition();
    const sf::Vector2u size = getSize();
    if (boundsOffset.y + _position.y + size.y > boundsSize.y) {
      _position.y = boundsSize.y - size.y;
      _velocity.y *= -0.75;
    }
    if (boundsOffset.x + _position.x + size.x > boundsSize.x) {
      _velocity.x *= -1;
      _position.x = boundsSize.x - size.x;
    } else if (_position.x < 0) {
      _velocity.x *= -1;
      _position.x = 0;
    }
  }
}

sf::Vector2u PhysicsAnimatable::getSize() const {
  return _size;
}