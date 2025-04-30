#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "Button.h"
#include "../TextureLoader.h"
#include <vector>

class ToggleButton : public Button {
public:
  ToggleButton(const int x, const int y, const std::vector<const char*>& textures,
               const std::function<void(int)>& onClick)
      : Button(x, y, textures[0], [this, onClick]() {
          onClick(_state);
          toggleState();
      }),
        _state(0),
        _textures(textures) {
    setPosition(x, y);
  }

  void toggleState() {
    _state = (_state + 1) % _textures.size();
    setTexture(*TextureLoader::loadTexture(_textures[_state]));
  }

  int getState() const {
    return _state;
  }

private:
  std::vector<const char*> _textures;
  int _state;
};