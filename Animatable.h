#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

class Animatable: virtual public sf::Drawable {
  public:
    Animatable() {
      _created_at_time = Clock.getElapsedTime().asMilliseconds();
      _last_update_time = _created_at_time;
    }
    virtual void update() {
      _last_update_time = Clock.getElapsedTime().asMilliseconds();
    }
    [[nodiscard]] unsigned int getDeltaTime() const {
      return Clock.getElapsedTime().asMilliseconds() - _last_update_time;
    }
    [[nodiscard]] unsigned int getAge() const {
      return _last_update_time - _created_at_time;
    }
    void resetAge() {
      _created_at_time = Clock.getElapsedTime().asMilliseconds();
      _last_update_time = _created_at_time;
    }
  private:
    std::function<void(int)> _update;
    int _created_at_time;
    int _last_update_time;
    static sf::Clock Clock;
};

sf::Clock Animatable::Clock;