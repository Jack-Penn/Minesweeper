#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include "SpriteAnimation.h"
#include "ScreenShaker.h"
#include "../TextureLoader.h"
#include "PhysicsAnimatable.h"
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

   static sf::Texture* _explosionSpritesheet;
   SpriteAnimation *_explosionSprite;
   bool _isExploding = false;

  sf::RenderWindow* _window;
   static std::mt19937 number_generator;
};

ScreenShaker AnimatedCharBomb::explosionShaker([](const float intensity, const float progress) {
  static std::uniform_real_distribution<float> offset_dist(-1.0f, 1.0f);
  const float offsetX = offset_dist(AnimatedCharBomb::number_generator) * intensity;
  const float offsetY = offset_dist(AnimatedCharBomb::number_generator) * intensity;
  return sf::Vector2i(offsetX,offsetY);
}, 50);
sf::Texture* AnimatedCharBomb::_explosionSpritesheet = TextureLoader::loadTexture("explosion.png");
std::mt19937 AnimatedCharBomb::number_generator(3504);


AnimatedCharBomb::AnimatedCharBomb(const char character, const sf::Font& font, const int charSize, const sf::Vector2f position, sf::RenderWindow* window)
    : PhysicsAnimatable(position), _text(character, font, charSize), _window(window) {
    static std::uniform_int_distribution<> lifespan_distribution(2000, 5000);
    _lifespan = lifespan_distribution(number_generator);

    _velocity = randomVector(300, 600, -150, -30);

    _explosionSprite = new SpriteAnimation(*_explosionSpritesheet, 192, 500, false);
    _explosionSprite->sprite.setScale(0.5f, 0.5f);
    _explosionSprite->centerSprite({-30, -30});
    static std::uniform_int_distribution<> explosion_rotation_distribution(0, 360);
    _explosionSprite->sprite.setRotation(explosion_rotation_distribution(number_generator));

    const sf::Vector2u windowSize = _window->getSize();
    _bounds = sf::IntRect(0,0,windowSize.x,windowSize.y);
    _boundsCollide = true;
}

sf::Vector2u AnimatedCharBomb::getSize() const{
      return sf::Vector2u(_text.getCharacterSize(), _text.getCharacterSize());
}

void AnimatedCharBomb::update() {
  if (_isExploding) {
    _explosionSprite->update();
  } else {
    PhysicsAnimatable::update();
    _text.setPosition(_position);

    const unsigned int current_age = getAge() + 1;

    // Red beeping
    const int beepInterval = 1000 * 1000 / current_age;
    _text.setFillColor(current_age % beepInterval > beepInterval / 2
                           ? sf::Color::Red
                           : sf::Color::White);

    if (current_age > _lifespan) {
      _isExploding = true;
      _explosionSprite->sprite.setPosition(_position);
      _explosionSprite->resetAge();
      explosionShaker.startShake(_window, 1000, 20);
    }
  }
}

inline void AnimatedCharBomb::draw(sf::RenderTarget &target, const sf::RenderStates states) const {
    if (isAnimationOver()) return;
    if (_isExploding) {
      target.draw(*_explosionSprite, states);
    }
    else
      target.draw(_text, states);
}

bool AnimatedCharBomb::isAnimationOver() const {
  return _isExploding && _explosionSprite->isAnimationOver();
}