#pragma once
#include "Animatable.h"
#include <random>

class ScreenShaker;
using shakeFunType = std::function<sf::Vector2i(float, float)>;

class ScreenShaker final : public Animatable {
  public:
    ScreenShaker(shakeFunType shakeFun, unsigned int maxIntensity): _shakeFun(shakeFun), _maxIntensity(maxIntensity){}
    ScreenShaker(sf::RenderWindow* window, shakeFunType shakeFun, unsigned int maxIntensity): _window(window), _shakeFun(shakeFun), _maxIntensity(maxIntensity) {}
    void startShake(const unsigned int& duration, const unsigned int& intensity) {
      if (_window)
        startShake(_window, duration, intensity);
      else
        throw std::runtime_error("Window Shaker doesn't have a window specified");
    }
    bool isAnimationOver() const override {
      return getAge() > _duration;
    }
    void startShake(sf::RenderWindow* window, const unsigned int& duration, const unsigned int& intensity) {
      _window = window;
      resetAge();
      if(_isShaking) {
        _duration = std::max(duration, _duration);
        _intensity = std::min(_maxIntensity, _intensity + intensity/2);
      } else {
        _isShaking = true;
        _duration = duration;
        _originalPosition = _window->getPosition();
        _intensity = intensity;
      }

    }
    void update() override {
      Animatable::update();
      if(!_isShaking) {
        return;
      }
      const unsigned int current_age = getAge();
      if(current_age < _duration) {
        const float shakeProgress = static_cast<float>(current_age) / _duration;
        const float currentIntensity = _intensity * std::max(0.0f, 1.0f - shakeProgress);
        const sf::Vector2i offset = _shakeFun(currentIntensity, shakeProgress);
        _window->setPosition(_originalPosition + offset);
      } else {
        _isShaking = false;
        _window->setPosition(_originalPosition);
      }
    }

    void draw(sf::RenderTarget&, const sf::RenderStates) const override {};

    [[nodiscard]] bool getIsShaking() const {
      return _isShaking;
    }
   private:
     sf::RenderWindow* _window = nullptr;
     bool _isShaking = false;
     unsigned int _duration = 0;
     unsigned int _intensity = 0;
     unsigned int _maxIntensity = 0;
     sf::Vector2i _originalPosition;

    shakeFunType _shakeFun;
};