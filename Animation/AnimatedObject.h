#pragma once
#include "Animatable.h"

class AnimatedObject final : public Animatable {
  public:
    AnimatedObject(
        const std::function<void(unsigned int, unsigned int)>& updateFun, const std::function<void(sf::RenderTarget&, sf::RenderStates)>& drawFun, const std::function<bool(unsigned int)>& isAnimationOverFun = nullptr)
  : _update(updateFun), _draw(drawFun), _isAnimationOver(isAnimationOverFun) {}
    void update() override {
      _update(getDeltaTime(), getAge());
      Animatable::update();
    }
    void draw(sf::RenderTarget& target, const sf::RenderStates states) const override {
      _draw(target, states);
    }
    bool isAnimationOver() const override {
      if (_isAnimationOver) {
        return _isAnimationOver(getAge());
      }
      return false;
    }
  private:
    std::function<void(unsigned int, unsigned int)> _update;
    std::function<void(sf::RenderTarget &, sf::RenderStates)> _draw;
    std::function<bool(unsigned int)> _isAnimationOver;
};
