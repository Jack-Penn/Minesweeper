#include <iostream>
#include <SFML/Graphics.hpp>
#include <cctype>
#include <bits/stdc++.h>
#include "AnimatedCharBomb.h"
#include "SpriteAnimation.h"
#include "AnimatedObject.h"
#include "ScreenShaker.h"

void setTextCenter(sf::Text &text, float x, float y);

int main() {
    sf::RenderWindow welcomeWindow(sf::VideoMode(800, 600), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    {
        sf::Image icon;
        if (!icon.loadFromFile("../files/images/mine.png"))
            std::cout << "Error loading welcomeWindow icon" << std::endl;
        const sf::Vector2u iconSize = icon.getSize();
        welcomeWindow.setIcon(iconSize.x, iconSize.y, icon.getPixelsPtr());
    }
    ScreenShaker inputShaker(&welcomeWindow, [](const float intensity, const float progress) {
        const int offsetX = std::sin(progress * 5.0f * M_PI) * intensity;
        return sf::Vector2i(offsetX, 0);
    }, 20);

    sf::Font font;
    if (!font.loadFromFile("../files/font.ttf"))
        std::cout << "Error loading font" << std::endl;
    const sf::Vector2u windowSize = welcomeWindow.getSize();

    sf::Text welcomeText("WELCOME TO MINESWEEPER!", font, 24);
    welcomeText.setStyle(sf::Text::Underlined);
    setTextCenter(welcomeText, windowSize.x/2.0f, windowSize.y/2.0f - 150);

    sf::Text inputLabel("Enter your name:", font, 20);
    setTextCenter(inputLabel, windowSize.x/2.0f, windowSize.y/2.0f - 75);

    constexpr short MAX_NAME_CHAR = 10;
    char player_name[MAX_NAME_CHAR+1] = "";
    size_t name_i = 0;
    sf::Text playerNameLabel(player_name, font, 20);

    sf::Text cursorText("|", font, 20);
    cursorText.setOrigin(5, 0);
    bool isCursorShowing = true;
    auto blinkingCursor = AnimatedObject([&isCursorShowing](unsigned int, const unsigned int age) {
        constexpr int CURSOR_BLINK_TIME_WAIT = 500;
        constexpr int CURSOR_BLINK_TIME_HOLD = 500;
        isCursorShowing = age % (CURSOR_BLINK_TIME_WAIT + CURSOR_BLINK_TIME_HOLD) < CURSOR_BLINK_TIME_HOLD;
    }, [&welcomeWindow, &cursorText, &isCursorShowing](const sf::RenderTarget&, const sf::RenderStates&) {
        if (isCursorShowing)
            welcomeWindow.draw(cursorText);
    });
    auto updateCursorPosition = [&playerNameLabel, &name_i, &cursorText, &blinkingCursor] {
        const sf::Vector2f charPosition = playerNameLabel.findCharacterPos(name_i);
        cursorText.setPosition(charPosition);
        blinkingCursor.resetAge();
    };

    const auto updatePlayerNameLabel = [&playerNameLabel, &player_name, &updateCursorPosition] {
        playerNameLabel.setString(player_name);
        const sf::FloatRect textRect = playerNameLabel.getGlobalBounds();
        playerNameLabel.setOrigin(textRect.width / 2.0f, 5);
        updateCursorPosition();
    };
    playerNameLabel.setPosition(windowSize.x/2.0f, windowSize.y/2.0f - 45);
    updatePlayerNameLabel();

    const auto isMouseHoveringInput = [&playerNameLabel](const int& mouseX, const int& mouseY) -> bool {
        const sf::FloatRect tightTextRect = playerNameLabel.getGlobalBounds();
        const sf::Vector2f PADDING(10, 10);
        const sf::Vector2f paddedTextRectPos = tightTextRect.getPosition() - PADDING;
        const sf::FloatRect paddedTextRect(paddedTextRectPos.x,paddedTextRectPos.y, tightTextRect.getSize().x + 2.0f * PADDING.x, 16 + 2.0f * PADDING.y);
        return paddedTextRect.contains(mouseX, mouseY);
    };

    std::vector<AnimatedCharBomb*> charBombs;
    auto addCharBomb = [&playerNameLabel, &font, &player_name, &charBombs, &welcomeWindow](const size_t player_name_index) {
        const sf::Vector2f pos = playerNameLabel.findCharacterPos(player_name_index);
        auto* const new_bomb = new AnimatedCharBomb(player_name[player_name_index], font, 20, pos, &welcomeWindow);
        for (auto& charBomb : charBombs) {
            if (!charBomb) {
                charBomb = new_bomb;
                return;
            }
        }
        charBombs.push_back(new_bomb);
    };

    auto coutPlayerName = [&player_name, &name_i] {
        for (int i = 0; i <= MAX_NAME_CHAR; i++) {
            if (player_name[i] == '\0') {
                std::cout << "\\0 ";
            } else {
                std::cout << player_name[i] << " ";
            }
        }
        std::cout << "\nname_i: " << name_i << std::endl;
    };
    // coutPlayerName();

    while(welcomeWindow.isOpen()) {
        sf::Event event{};
        while(welcomeWindow.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    welcomeWindow.close();
                    return 0;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Enter:
                            if (strlen(player_name) == 0) {
                                inputShaker.startShake(500, 20);
                                break;
                            }
                            welcomeWindow.close();
                            std::cout << "Submitted Name: " << player_name << std::endl;
                            break;
                        case sf::Keyboard::Backspace:
                            if (name_i > 0 ) {
                                addCharBomb(name_i - 1);
                                if (player_name[name_i] == '\0') {
                                    player_name[--name_i] = '\0';
                                } else {
                                    size_t i = --name_i;
                                    do {
                                        player_name[i] = player_name[i+1];
                                    } while (player_name[i++] != '\0');

                                    if (name_i == 0)
                                        player_name[0] = toupper(player_name[0]);
                                }
                                updatePlayerNameLabel();
                            }
                            break;
                        case sf::Keyboard::Delete:
                            if (player_name[name_i] != '\0' ) {
                                addCharBomb(name_i);
                                if (player_name[name_i+1] == '\0') {
                                    player_name[name_i] = '\0';
                                } else {
                                    size_t i = name_i;
                                    do {
                                        player_name[i] = player_name[i+1];
                                    } while (player_name[i++] != '\0');

                                    if (name_i == 0)
                                        player_name[0] = std::toupper(player_name[0]);
                                }
                                updatePlayerNameLabel();
                            }
                            break;
                        case sf::Keyboard::Left:
                            if (name_i > 0) {
                                --name_i;
                                updateCursorPosition();
                            }
                            break;
                        case sf::Keyboard::Right:
                            if (player_name[name_i] != '\0') {
                                ++name_i;
                                updateCursorPosition();
                            }
                        break;
                        default:{}
                        }
                    // coutPlayerName();
                    break;
                case sf::Event::TextEntered: {
                    char c = std::tolower(event.text.unicode);
                    if (c >= 97 && c <= 122 && name_i < MAX_NAME_CHAR) {
                        if (player_name[name_i] == '\0')
                            player_name[name_i + 1] = '\0';
                        else {
                            if (name_i == 0)
                                player_name[0] = std::tolower(player_name[0]);

                            for (size_t i = name_i+1; i < MAX_NAME_CHAR; i++) {
                                //Swaps char at name_i with each char after
                                const char temp = player_name[i];
                                player_name[i] = player_name[name_i];
                                player_name[name_i] = temp;
                                if (temp == '\0') //stops at first end of string
                                    break;
                            }
                            if (player_name[name_i] != '\0')
                                addCharBomb(name_i);
                        }
                        if (name_i == 0) {
                            player_name[name_i] = std::toupper(c);
                        } else {
                            player_name[name_i] = c;
                        }
                        ++name_i;
                        updatePlayerNameLabel();
                    }
                    break;
                }
                case sf::Event::MouseMoved: {
                    sf::Cursor cursor;
                    if (isMouseHoveringInput(event.mouseMove.x, event.mouseMove.y)) {
                        if (cursor.loadFromSystem(sf::Cursor::Text))
                            welcomeWindow.setMouseCursor(cursor);
                    } else if (cursor.loadFromSystem(sf::Cursor::Arrow))
                            welcomeWindow.setMouseCursor(cursor);
                    break;
                }
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left && isMouseHoveringInput(event.mouseButton.x, event.mouseButton.y)) {
                        size_t i = 0;
                        float charTextWidth = 12;
                        float charPosX;
                        do {
                            if (player_name[i] == '\0') {
                                i++;
                                break;
                            }
                            charPosX = playerNameLabel.findCharacterPos(i++).x;
                        } while (charPosX + charTextWidth/2 < event.mouseButton.x);
                        // std::cout << "new i: " << i-1 << std::endl;
                        name_i = i-1;
                        updateCursorPosition();
                    }
                    break;
                default: {}
                }
        }

        if (inputShaker.getIsShaking())
            inputShaker.update();
        else
            AnimatedCharBomb::explosionShaker.update();

        blinkingCursor.update();

        //Draw Element
        welcomeWindow.clear(sf::Color::Blue);

        for (auto& bomb : charBombs) {
            if (bomb == nullptr) continue;
            if (bomb->isAnimationOver()) {
                delete bomb;
                bomb = nullptr;
                continue;
            }
            bomb->update();
            welcomeWindow.draw(*bomb);
        }

        welcomeWindow.draw(welcomeText);
        welcomeWindow.draw(inputLabel);
        welcomeWindow.draw(blinkingCursor);
        welcomeWindow.draw(playerNameLabel);
        welcomeWindow.display();
    }
    return 0;
}

void setTextCenter(sf::Text &text, const float x, const float y) {
    const sf::FloatRect textRect = text.getGlobalBounds();
    text.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}