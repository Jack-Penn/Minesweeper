#pragma once

#include "../Animation/ScreenShaker.h"
#include "../Animation/AnimatedObject.h"
#include "../Animation/AnimatedCharBomb.h"
#include "../util.h"
#include "Window.h"
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

class WelcomeWindow final: public Window {
  public: WelcomeWindow(const int width,
    const int height,
      const sf::Font & font): Window(width, height, sf::Color::Blue, "Welcome", "mine.png", {
    {
      sf::Event::KeyPressed, [ & ](const sf::Event & event) {
        switch (event.key.code) {
        case sf::Keyboard::Enter:
          if (strlen(player_name) == 0) {
            _inputShaker.startShake(500, 20);
            break;
          }
          close();
          break;
        case sf::Keyboard::Backspace:
          if (_name_i > 0) {
            addCharBomb(_name_i - 1);
            if (player_name[_name_i] == '\0') {
              player_name[--_name_i] = '\0';
            } else {
              size_t i = --_name_i;
              do {
                player_name[i] = player_name[i + 1];
              } while (player_name[i++] != '\0');

              if (_name_i == 0)
                player_name[0] = toupper(player_name[0]);
            }
            updatePlayerNameLabel();
          }
          break;
        case sf::Keyboard::Delete:
          if (player_name[_name_i] != '\0') {
            addCharBomb(_name_i);
            if (player_name[_name_i + 1] == '\0') {
              player_name[_name_i] = '\0';
            } else {
              size_t i = _name_i;
              do {
                player_name[i] = player_name[i + 1];
              } while (player_name[i++] != '\0');

              if (_name_i == 0)
                player_name[0] = std::toupper(player_name[0]);
            }
            updatePlayerNameLabel();
          }
          break;
        case sf::Keyboard::Left:
          if (_name_i > 0) {
            --_name_i;
            updateCursorPosition();
          }
          break;
        case sf::Keyboard::Right:
          if (player_name[_name_i] != '\0') {
            ++_name_i;
            updateCursorPosition();
          }
          break;
        default: {}
        }
      }
    },
    {
      sf::Event::TextEntered,
      [ & ](const sf::Event & event) {
        const char c = std::tolower(event.text.unicode);
        if (c >= 97 && c <= 122 && _name_i < maxNameLength) {
          if (player_name[_name_i] == '\0')
            player_name[_name_i + 1] = '\0';
          else {
            if (_name_i == 0)
              player_name[0] = std::tolower(player_name[0]);

            for (size_t i = _name_i + 1; i < maxNameLength; i++) {
              // Swaps char at name_i with each char after
              const char temp = player_name[i];
              player_name[i] = player_name[_name_i];
              player_name[_name_i] = temp;
              if (temp == '\0') // stops at first end of string
                break;
            }
            if (player_name[_name_i] != '\0')
              addCharBomb(_name_i);
          }
          if (_name_i == 0) {
            player_name[_name_i] = std::toupper(c);
          } else {
            player_name[_name_i] = c;
          }
          ++_name_i;
          updatePlayerNameLabel();
        }
      }
    },
    {
      sf::Event::MouseMoved,
      [ & ](const sf::Event & event) {
        sf::Cursor cursor;
        if (isMouseHoveringInput(event.mouseMove.x, event.mouseMove.y)) {
          if (cursor.loadFromSystem(sf::Cursor::Text))
            setMouseCursor(cursor);
        } else if (cursor.loadFromSystem(sf::Cursor::Arrow))
          setMouseCursor(cursor);
      }
    },
    {
      sf::Event::MouseButtonPressed,
      [ & ](const sf::Event & event) {
        if (event.mouseButton.button == sf::Mouse::Left &&
          isMouseHoveringInput(event.mouseButton.x,
            event.mouseButton.y)) {
          size_t i = 0;
          constexpr float charTextWidth = 12;
          float charPosX;
          do {
            if (player_name[i] == '\0') {
              i++;
              break;
            }
            charPosX = _playerNameLabel.findCharacterPos(i++).x;
          } while (charPosX + charTextWidth / 2 < event.mouseButton.x);
          _name_i = i - 1;
          updateCursorPosition();
        }
      }
    }
  }),
  _inputShaker(
    this,
    [](const float intensity,
      const float progress) {
      const int offsetX =
        std::sin(progress * 5.0f * M_PI) * intensity;
      return sf::Vector2i(offsetX, 0);
    },
    20),
  _welcomeText("WELCOME TO MINESWEEPER!", font, 24),
  _inputLabel("Enter your name:", font, 20),
  _playerNameLabel(player_name, font, 20),
  _cursorText("|", font, 20),
  _blinkingCursor(
    [ & ](unsigned int,
      const unsigned int age) {
      constexpr int CURSOR_BLINK_TIME_WAIT = 500;
      constexpr int CURSOR_BLINK_TIME_HOLD = 500;
      _isCursorShowing =
        age % (CURSOR_BLINK_TIME_WAIT + CURSOR_BLINK_TIME_HOLD) <
        CURSOR_BLINK_TIME_HOLD;
    },
    [ & ](const RenderTarget & ,
      const sf::RenderStates & ) {
      if (_isCursorShowing)
        draw(_cursorText);
    }) {
    _welcomeText.setStyle(sf::Text::Underlined);
    setTextCenter(_welcomeText, width / 2.0f, height / 2.0f - 150);

    setTextCenter(_inputLabel, width / 2.0f, height / 2.0f - 75);

    _playerNameLabel.setFillColor(sf::Color::Yellow);
    _playerNameLabel.setPosition(width / 2.0f, height / 2.0f - 45);
    updatePlayerNameLabel();

    _cursorText.setOrigin(5, 0);
  }

  int maxNameLength = 10;
  char player_name[11] = "";

  void logPlayerName() const {
    for (int i = 0; i <= maxNameLength; i++) {
      if (player_name[i] == '\0') {
        std::cout << "\\0 ";
      } else {
        std::cout << player_name[i] << " ";
      }
    }
    std::cout << "\n_name_i: " << _name_i << std::endl;
  }

  void _update() override {
    if (_inputShaker.getIsShaking())
      _inputShaker.update();
    else
      AnimatedCharBomb::explosionShaker.update();

    _blinkingCursor.update();
    _charBombs.clean();
    _charBombs.update();
  }

  bool _shouldRedraw() override {
    return true;
  }

  void _draw() override {
    draw(_welcomeText);
    draw(_inputLabel);
    draw(_playerNameLabel);
    draw(_blinkingCursor);
    draw(_charBombs);
  }

  private: ScreenShaker _inputShaker;
  int _name_i = 0;
  sf::Text _welcomeText;
  sf::Text _inputLabel;
  sf::Text _playerNameLabel;
  sf::Text _cursorText;
  bool _isCursorShowing = true;
  AnimatedObject _blinkingCursor;
  AnimationList < AnimatedCharBomb > _charBombs;

  void updateCursorPosition() {
    const sf::Vector2f charPosition = _playerNameLabel.findCharacterPos(_name_i);
    _cursorText.setPosition(charPosition);
    _blinkingCursor.resetAge();
  }

  void updatePlayerNameLabel() {
    _playerNameLabel.setString(player_name);
    const sf::FloatRect textRect = _playerNameLabel.getGlobalBounds();
    _playerNameLabel.setOrigin(textRect.width / 2.0f, 5);
    updateCursorPosition();
  }

  bool isMouseHoveringInput(const int & mouseX,
    const int & mouseY) const {
    const sf::FloatRect tightTextRect = _playerNameLabel.getGlobalBounds();
    const sf::Vector2f PADDING(10, 10);
    const sf::Vector2f paddedTextRectPos = tightTextRect.getPosition() - PADDING;
    const sf::FloatRect paddedTextRect(
      paddedTextRectPos.x, paddedTextRectPos.y,
      tightTextRect.getSize().x + 2.0f * PADDING.x,
      16 + 2.0f * PADDING.y);
    return paddedTextRect.contains(mouseX, mouseY);
  }

  void addCharBomb(const size_t player_name_index) {
    const sf::Vector2f pos = _playerNameLabel.findCharacterPos(player_name_index);
    auto *
      const new_bomb = new AnimatedCharBomb(player_name[player_name_index], * _playerNameLabel.getFont(), 20, pos, this);
    _charBombs.push(new_bomb);
  }
};