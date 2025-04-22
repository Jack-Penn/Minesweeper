#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

class Animatable : virtual public sf::Drawable {
public:
  Animatable();
  virtual void update();
  [[nodiscard]] unsigned int getDeltaTime() const;
  [[nodiscard]] unsigned int getAge() const;
  void resetAge();
  virtual bool isAnimationOver() const = 0;
private:
  std::function<void(int)> _update;
  int _created_at_time;
  int _last_update_time;
  static sf::Clock Clock;
};