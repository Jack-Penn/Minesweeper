#pragma once

#include "SpriteAnimation.h"
#include "../util.h"

class Explosion final : public SpriteAnimation {
  public:
    explicit Explosion(sf::Vector2f pos = {}, unsigned int size = 96): SpriteAnimation(*_explosionSpritesheet, 192, 500, false) {
      const float scaleFactor = size/192.0;
      sprite.setScale(scaleFactor, scaleFactor);
      centerSprite({-30, -30});
      sprite.setPosition(pos);
      static std::uniform_int_distribution<> explosion_rotation_distribution(0, 360);
      sprite.setRotation(explosion_rotation_distribution(number_generator));
    }

    void setPosition(sf::Vector2f pos) {
      sprite.setPosition(pos);
    }

  private:
    static sf::Texture* _explosionSpritesheet;
};