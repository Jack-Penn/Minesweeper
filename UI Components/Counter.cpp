#include <SFML/Graphics.hpp>
#include "../TextureLoader.h"
#include "Counter.h"

sf::Texture* Counter::_digitsTexture = TextureLoader::loadTexture("digits.png");