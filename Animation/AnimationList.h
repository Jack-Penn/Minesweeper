#pragma once

#include "Animatable.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T = Animatable>
class AnimationList final : public Animatable {
public:
  AnimationList() {}

  ~AnimationList() {
    _forEachAnimatable([](Animatable* animatable) {
      delete animatable;
    });
  }

  void update() override {
    // _print();
    _forEachAnimatable([](T* animatable) {
        animatable->update();
    });
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    _forEachAnimatable([&target, &states](const T* animatable) {
      target.draw(*animatable, states);
    });
  }

  bool isAnimationOver() const override {
    if (_animatables.empty()) return true;
    for (const auto& animatable : _animatables) {
      if (animatable != nullptr && !animatable->isAnimationOver()) {
        return false;
      }
    }
    return true;
  }

  void push(T* new_animatable) {
    _animatables.push_back(new_animatable);
  }

  void clean() {
    _forEachAnimatable([](T*& animatable) {
      if (animatable != nullptr && animatable->isAnimationOver()) {
        delete animatable;
        animatable = nullptr;
      }
    });

    _animatables.erase(
      std::remove(_animatables.begin(), _animatables.end(), nullptr),
      _animatables.end()
    );
  }

private:
  std::vector<T*> _animatables;

  template <typename Func>
  void _forEachAnimatable(const Func& func) {
    for (int i = 0; i < _animatables.size(); i++) {
      func(_animatables[i]);
    }
  }

  template <typename Func>
  void _forEachAnimatable(const Func& func) const {
    for (int i = 0; i < _animatables.size(); i++) {
      func(_animatables[i]);
    }
  }

  void _print() {
    std::cout << _animatables.size() << " ";
    for (const auto animatable : _animatables) {
      if (animatable == nullptr) std::cout << "nullptr";
      else std::cout << animatable;
      std::cout << ", ";
    }
    std::cout << std::endl;
  }
};
