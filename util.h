#pragma once

#include <random>
#include <SFML/Graphics.hpp>

extern std::mt19937 number_generator;

sf::Vector2f randomVector(float minMag, float maxMag, float minAngle, float maxAngle);
sf::Vector2f randomVector(float minMag, float maxMag, const std::vector<std::pair<float, float>>& angleRanges);

template <typename T>
T nonuniformScale(T vec1, T vec2) {
  return {vec1.x * vec2.x, vec1.y * vec2.y};
}

void setTextCenter(sf::Text &text, float x, float y);
