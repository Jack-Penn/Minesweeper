#pragma once
#include "Animatable.h"

class AnimatedObject final : public Animatable {
  public:
    AnimatedObject(
        const std::function<void(unsigned int, unsigned int)>& updateFun, const std::function<void(sf::RenderTarget&, sf::RenderStates)>& drawFun): _update(updateFun), _draw(drawFun) {}
    void update() override {
      _update(getDeltaTime(), getAge());
      Animatable::update();
    }
    void draw(sf::RenderTarget& target, const sf::RenderStates states) const override {
      _draw(target, states);
    }
  private:
    std::function<void(unsigned int, unsigned int)> _update;
    std::function<void(sf::RenderTarget&, sf::RenderStates)> _draw;
};
