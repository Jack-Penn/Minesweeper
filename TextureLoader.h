#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>
#include <string>

class TextureLoader {
public:
  static sf::Texture* loadTexture(const std::string& filename);
  ~TextureLoader();
private:
  static std::unordered_map<std::string, sf::Texture*>& _getTextureCache();
};