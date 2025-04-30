#pragma once
#include "Animatable.h"

class Delay: public Animatable {
public:
  Delay(unsigned int duration, std::function<void()> callback): _duration(duration), _callback(callback) {}
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {}
  void update() override {
    Animatable::update();
    if(!_isOver && getAge() > _duration) {
      _callback();
      _isOver = true;
    }
  }

  bool isAnimationOver() const override {
    return _isOver;
  }

  private:
    unsigned int _duration;
    std::function<void()> _callback;
    bool _isOver = false;
};