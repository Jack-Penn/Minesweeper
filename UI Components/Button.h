#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button: public sf::Sprite {
public:
  Button(const int x, const int y, const char* texturePath,
         const std::function<void()> &onClick): Sprite(*TextureLoader::loadTexture(texturePath)), _onClick(onClick) {
    setPosition(x, y);
  }
  template <typename T>
  bool contains(sf::Vector2<T> point) {
    sf::IntRect rect(getGlobalBounds());
    return rect.contains(point);
  }
  void handleClick(const sf::Mouse::Button& button) {
    if (button == sf::Mouse::Left) _onClick();
  }
private:
  std::function<void()> _onClick;
};