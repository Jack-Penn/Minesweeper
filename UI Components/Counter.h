#pragma once
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

class Counter : public sf::Drawable {
public:
    Counter(const float x, const float y, const int& initialCount, const short& numDigits)
        : count(initialCount), numDigits(numDigits) {
        _digits = new sf::Sprite[numDigits];
        for (int i = 0; i < numDigits; ++i) {
            _digits[i] = sf::Sprite(*_digitsTexture);
            _digits[i].setPosition({ static_cast<float>(x + i * 21.0), y });
        }

        _sign = sf::Sprite(*_digitsTexture);
        _sign.setTextureRect({ 210, 0, 21, 32 });
        _sign.setPosition(x - 21, y);
        _updateSprites();
    }

    ~Counter() {
        delete[] _digits;
    }

    Counter(const Counter& other)
        : count(other.count), numDigits(other.numDigits) {
        _digits = new sf::Sprite[numDigits];
        for (int i = 0; i < numDigits; ++i) {
            _digits[i] = other._digits[i];
        }
        _sign = other._sign;
    }

    Counter& operator=(const Counter& other) {
        if (this != &other) {
            delete[] _digits;

            count = other.count;
            numDigits = other.numDigits;
            _digits = new sf::Sprite[numDigits];
            for (int i = 0; i < numDigits; ++i) {
                _digits[i] = other._digits[i];
            }
            _sign = other._sign;
        }
        return *this;
    }

    void setNumber(int num) {
        if (num != count) {
            count = num;
            _updateSprites();
        }
    }

    int getNumber() const {
        return count;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        if (count < 0) {
            target.draw(_sign, states);
        }
        for (int i = 0; i < numDigits; ++i) {
            target.draw(_digits[i], states);
        }
    }

private:
    static sf::Texture* _digitsTexture;
    int count;
    short numDigits;
    sf::Sprite* _digits;
    sf::Sprite _sign;

    void _updateSprites() {
        int positiveCount = std::abs(count);
        for (int i = 0; i < numDigits; ++i) {
            const int digit = (positiveCount / static_cast<int>(std::pow(10, numDigits - 1 - i))) % 10;
            _digits[i].setTextureRect({ 21 * digit, 0, 21, 32 });
        }
    }
};