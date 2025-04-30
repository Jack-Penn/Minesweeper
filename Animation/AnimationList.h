#pragma once

#include "Animatable.h"
#include "SFML/Graphics.hpp"
#include <list>
#include <algorithm>
#include <iostream>

template <typename T = Animatable>
class AnimationList final : public Animatable {
public:
  AnimationList() = default;

  ~AnimationList() override {
    for (auto const& animatable: _animatables) {
      delete animatable;
    }
  }

  void update() override {
    for (auto const& animatable: _animatables) {
      animatable->update();
    }
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    for (auto const& animatable: _animatables) {
      target.draw(*animatable, states);
    }
  }

  bool isAnimationOver() const override {
    if (_animatables.empty()) return true;
    return std::all_of(_animatables.begin(), _animatables.end(), [](const Animatable* animatable) {
      return animatable->isAnimationOver();
    });
  }

  void push(T* new_animatable) {
    _animatables.push_back(new_animatable);
  }

  void clean() {
    for (auto it = _animatables.begin(); it != _animatables.end();) {
      auto* animatable = *it;
      if (animatable->isAnimationOver()) {
        it = _animatables.erase(it);
        delete animatable;
      } else {
        ++it;
      }
    }
  }

private:
  std::list<T*> _animatables;

  void _print() {
    std::cout << _animatables.size() << " ";
    for (auto* const& animatable: _animatables) {
      if (animatable == nullptr) std::cout << "nullptr";
      else std::cout << animatable;
      std::cout << ", ";
    }
    std::cout << std::endl;
  }
};
