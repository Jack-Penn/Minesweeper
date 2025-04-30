#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include "SpriteAnimation.h"
#include "ScreenShaker.h"
#include "PhysicsAnimatable.h"
#include "Explosion.h"
#include "../util.h"

class AnimatedCharBomb final : public PhysicsAnimatable {
public:
   AnimatedCharBomb(char character, const sf::Font& font, int charSize, sf::Vector2f position, sf::RenderWindow* window);

   sf::Vector2u getSize() const override;

   void update() override;
   void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

   bool isAnimationOver() const override;
  static ScreenShaker explosionShaker;
private:
   sf::Text _text;

    int _lifespan;
  
   Explosion *_explosion;
   bool _isExploding = false;

  sf::RenderWindow* _window;
};

ScreenShaker AnimatedCharBomb::explosionShaker([](const float intensity, const float progress) -> sf::Vector2i {
  static std::uniform_real_distribution<float> offset_dist(-1.0f, 1.0f);
  const int offsetX = std::floor(offset_dist(number_generator) * intensity);
  const int offsetY = std::floor(offset_dist(number_generator) * intensity);
  return {offsetX,offsetY};
}, 50);




AnimatedCharBomb::AnimatedCharBomb(const char character, const sf::Font& font, const int charSize, const sf::Vector2f position, sf::RenderWindow* window)
    : PhysicsAnimatable(position), _text(character, font, charSize), _window(window) {
    static std::uniform_int_distribution<> lifespan_distribution(2000, 5000);
    _lifespan = lifespan_distribution(number_generator);

    _velocity = randomVector(300, 600, -150, -30);

    _explosion = new Explosion();

    const sf::Vector2u windowSize = _window->getSize();
    _bounds = sf::IntRect(0,0,windowSize.x,windowSize.y);
    _boundsCollide = true;

    _text.setColor(sf::Color::Yellow);
}

sf::Vector2u AnimatedCharBomb::getSize() const{
      return sf::Vector2u(_text.getCharacterSize(), _text.getCharacterSize());
}

void AnimatedCharBomb::update() {
  if (_isExploding) {
    _explosion->update();
  } else {
    PhysicsAnimatable::update();
    _text.setPosition(_position);

    const unsigned int current_age = getAge() + 1;

    // Red beeping
    const int beepInterval = 1000 * 1000 / current_age;
    _text.setFillColor(current_age % beepInterval > beepInterval / 2
                           ? sf::Color::Red
                           : sf::Color::Yellow);

    if (current_age > _lifespan) {
      _isExploding = true;
      _explosion->setPosition(_position);
      _explosion->resetAge();
      explosionShaker.startShake(_window, 1000, 20);
    }
  }
}

inline void AnimatedCharBomb::draw(sf::RenderTarget &target, const sf::RenderStates states) const {
    if (isAnimationOver()) return;
    if (_isExploding) {
      target.draw(*_explosion, states);
    }
    else
      target.draw(_text, states);
}

bool AnimatedCharBomb::isAnimationOver() const {
  return _isExploding && _explosion->isAnimationOver();
}