#include "util.h"
#include <cmath>
#include <vector>
#include <utility>
#include <numeric>
#include <random>

std::mt19937 number_generator(3504);

sf::Vector2f randomVector(float minMag, float maxMag, float minAngle, float maxAngle) {
  std::uniform_real_distribution<> magnitude_distribution(minMag, maxMag);
  std::uniform_real_distribution<> angle_distribution(minAngle, maxAngle);
  const int magnitude = magnitude_distribution(number_generator);
  const double angle_rad = angle_distribution(number_generator) * (M_PI / 180.0);
  return sf::Vector2f(magnitude * std::cos(angle_rad), magnitude * std::sin(angle_rad));
}

sf::Vector2f randomVector(float minMag, float maxMag, const std::vector<std::pair<float, float>>& angleRanges) {
  std::uniform_real_distribution<> magnitude_distribution(minMag, maxMag);
  const float magnitude = magnitude_distribution(number_generator);

  const float totalAngleRange = std::accumulate(angleRanges.begin(), angleRanges.end(), 0.0f,
      [](float sum, const std::pair<float, float>& range) {
          return sum + std::abs(range.second - range.first);
      });

  std::uniform_real_distribution<> angle_distribution(0.0f, totalAngleRange);
  float angle = angle_distribution(number_generator);

  for(const auto& range : angleRanges) {
    float rangeSize = std::abs(range.second - range.first);
    if(angle <= rangeSize) {
      angle = range.first + angle;
      break;
    }
    angle -= rangeSize;
  }

  const double angle_rad = angle * (M_PI / 180.0);
  return sf::Vector2f(magnitude * std::cos(angle_rad), magnitude * std::sin(angle_rad));
}


void setTextCenter(sf::Text &text, const float x, const float y) {
  const sf::FloatRect textRect = text.getGlobalBounds();
  text.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
  text.setPosition(sf::Vector2f(x, y));
}