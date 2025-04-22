#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../TextureLoader.h"

using EventCallbackType = std::function<void(sf::Event)>;
class Window: protected sf::RenderWindow {
  public:
    Window(const int width, const int height, const sf::Color backgroundColor, const char name[], const char iconFilename[], const std::map<sf::Event::EventType, EventCallbackType>& eventCallbacks = {})
        : RenderWindow(sf::VideoMode(width, height), name,sf::Style::Titlebar | sf::Style::Close), _name(name), _bgColor(backgroundColor), eventCallbacks(eventCallbacks) {

      sf::Image icon = TextureLoader::loadTexture(iconFilename)->copyToImage();
      const sf::Vector2u iconSize = icon.getSize();
      setIcon(iconSize.x, iconSize.y, icon.getPixelsPtr());
    }

  void focus() {
    if(!isOpen()) {
      const sf::Vector2u windowSize = getSize();
      create(sf::VideoMode(windowSize.x, windowSize.y), _name,sf::Style::Titlebar | sf::Style::Close);
    }

      clear(_bgColor);
      _draw();
      display();

    while (isOpen()) {
      sf::Event event;
      while (pollEvent(event)) {
        if(event.type == sf::Event::Closed) close();
        for(auto [eventType, eventCallback] : eventCallbacks) {
          if(event.type == eventType) {
            eventCallback(event);
            break;
          }
        }
      }
      _update();
      if(_shouldRedraw()) {
        clear(_bgColor);
        _draw();
        display();
      }
    }
  }

  sf::Vector2u getSize() const override {
   return RenderWindow::getSize();
  }
  protected:
    virtual void _update() = 0;
    virtual bool _shouldRedraw() = 0;
    virtual void _draw() = 0;
    std::map<sf::Event::EventType, EventCallbackType> eventCallbacks;
  private:
    const char *_name;
    sf::Color _bgColor;
};