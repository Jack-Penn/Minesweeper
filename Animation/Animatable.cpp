#include "Animatable.h"

sf::Clock Animatable::Clock;

Animatable::Animatable() {
  _created_at_time = Clock.getElapsedTime().asMilliseconds();
  _last_update_time = _created_at_time;
}

void Animatable::update() {
  _last_update_time = Clock.getElapsedTime().asMilliseconds();
}

unsigned int Animatable::getDeltaTime() const {
  return Clock.getElapsedTime().asMilliseconds() - _last_update_time;
}

unsigned int Animatable::getAge() const {
  return _last_update_time - _created_at_time;
}

void Animatable::resetAge() {
  _created_at_time = Clock.getElapsedTime().asMilliseconds();
  _last_update_time = _created_at_time;
}