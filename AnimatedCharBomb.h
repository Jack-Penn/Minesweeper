#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include "Animatable.h"
#include "SpriteAnimation.h"
#include "ScreenShaker.h"

class AnimatedCharBomb final : public Animatable {
public:
   static sf::Texture explosionSpritesheet;

   AnimatedCharBomb(char character, const sf::Font& font, int charSize, sf::Vector2f position, sf::RenderWindow* window);

   void update() override;
   void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

   bool isAnimationOver() const;
  static ScreenShaker explosionShaker;
private:
   sf::Text _text;
   sf::Vector2f _position;
   sf::Vector2f _velocity;

    int _lifespan;

   SpriteAnimation *_explosionSprite;
   bool _isExploding = false;

  sf::RenderWindow* _window;

   static sf::Vector2f Acceleration;

   static std::mt19937 number_generator;
   static std::uniform_int_distribution<> magnitude_distribution;
   static std::uniform_int_distribution<> angle_distribution;
   static std::uniform_int_distribution<> lifespan_distribution;
   static std::uniform_int_distribution<> explosion_rotation_distribution;
};

ScreenShaker AnimatedCharBomb::explosionShaker([](const float intensity, const float progress) {
  static std::uniform_real_distribution<float> offset_dist(-1.0f, 1.0f);
  const float offsetX = offset_dist(AnimatedCharBomb::number_generator) * intensity;
  const float offsetY = offset_dist(AnimatedCharBomb::number_generator) * intensity;
  return sf::Vector2i(offsetX,offsetY);
}, 50);
sf::Vector2f AnimatedCharBomb::Acceleration(0, 1000);
std::mt19937 AnimatedCharBomb::number_generator(3504);
std::uniform_int_distribution<> AnimatedCharBomb::magnitude_distribution(300, 600);
std::uniform_int_distribution<> AnimatedCharBomb::angle_distribution(30, 150);
std::uniform_int_distribution<> AnimatedCharBomb::lifespan_distribution(2000, 5000);
std::uniform_int_distribution<> AnimatedCharBomb::explosion_rotation_distribution(0, 360);

sf::Texture AnimatedCharBomb::explosionSpritesheet;

AnimatedCharBomb::AnimatedCharBomb(const char character, const sf::Font& font, const int charSize, const sf::Vector2f position, sf::RenderWindow* window)
    : _text(character, font, charSize), _position(position), _window(window) {
    _lifespan = lifespan_distribution(number_generator);
    // Random Velocity from angle and magnitude
    const int magnitude = magnitude_distribution(number_generator);
    const double angle_rad = angle_distribution(number_generator) * (M_PI / 180.0);
    _velocity = sf::Vector2f(magnitude * std::cos(angle_rad), -magnitude * std::sin(angle_rad));

    if (explosionSpritesheet.getSize().x == 0)
      explosionSpritesheet.loadFromFile("../files/images/explosion.png");

    _explosionSprite = new SpriteAnimation(explosionSpritesheet, 192, 500, false);
    _explosionSprite->sprite.setScale(0.5f, 0.5f);
    _explosionSprite->sprite.setOrigin(81, 81);
    _explosionSprite->sprite.setRotation(explosion_rotation_distribution(number_generator));
}

void AnimatedCharBomb::update() {
  if (_isExploding) {
    _explosionSprite->update();
  } else {
    const float dt = getDeltaTime() / 1000.0;
    Animatable::update();
    const unsigned int textSize = _text.getCharacterSize();

    _velocity += Acceleration * dt;
    _position += _velocity * dt;

    const auto windowSize = _window->getSize();
    if (_position.y + textSize > windowSize.y) {
      _position.y = windowSize.y - textSize;
      _velocity.y *= -0.75;
    }
    if (_position.x + textSize > windowSize.x) {
      _velocity.x *= -1;
      _position.x = windowSize.x - textSize;
    } else if (_position.x < 0) {
      _velocity.x *= -1;
      _position.x = 0;
    }
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