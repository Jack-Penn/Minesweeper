#include "TextureLoader.h"
#include <iostream>

std::unordered_map<std::string, sf::Texture*>& TextureLoader::_getTextureCache() {
  static std::unordered_map<std::string, sf::Texture*> _textureCache;
  return _textureCache;
}

sf::Texture* TextureLoader::loadTexture(const std::string& filename) {
  auto _textureCache = _getTextureCache();
  if(_textureCache.find(filename) == _textureCache.end()) {
    // Texture Not Loaded Yet
    auto* newTexture = new sf::Texture();
    const std::string relativePath = "../files/images/" + filename;
    if(!newTexture->loadFromFile(relativePath)) {
      std::cout << "Error loading " << relativePath << std::endl;
      delete newTexture;
      return nullptr;
    }
    _textureCache.emplace(filename, newTexture);
    return newTexture;
  }
  return _textureCache[filename];
}

TextureLoader::~TextureLoader() {
  auto _textureCache = _getTextureCache();
  for (const auto& pair : _textureCache) {
    delete pair.second;
  }
  _textureCache.clear();
}