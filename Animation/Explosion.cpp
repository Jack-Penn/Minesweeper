#include <SFML/Graphics.hpp>
#include "../TextureLoader.h"
#include "Explosion.h"

sf::Texture* Explosion::_explosionSpritesheet = TextureLoader::loadTexture("explosion.png");