#include "Menu.h"

Menu::Menu(RenderWindow& window) :win(window), option(0) {
    font.loadFromFile("assets/flappy-font.ttf");

    title.setFont(font);
    title.setString("Select Difficulty");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(100, 180);
    title.setOutlineColor(Color::Black);
    title.setOutlineThickness(3);

    easyText.setFont(font);
    easyText.setString("Easy");
    easyText.setCharacterSize(40);
    easyText.setPosition(120, 280);
    easyText.setOutlineColor(Color::Black);
    easyText.setOutlineThickness(2);

    mediumText.setFont(font);
    mediumText.setString("Medium");
    mediumText.setCharacterSize(40);
    mediumText.setPosition(120, 340);
    mediumText.setOutlineColor(Color::Black);
    mediumText.setOutlineThickness(2);

    hardText.setFont(font);
    hardText.setString("Hard");
    hardText.setCharacterSize(40);
    hardText.setPosition(120, 400);
    hardText.setOutlineColor(Color::Black);
    hardText.setOutlineThickness(2);

    updateTextColors();
}

void Menu::draw() {
    win.draw(title);
    win.draw(easyText);
    win.draw(mediumText);
    win.draw(hardText);
}

void Menu::Input(Event& event) {
    if (event.type == Event::KeyPressed) {
         if (event.key.code == sf::Keyboard::Up) {
            if (option == 0) {
                option = 2;
            }
            else {
                option--;
            }
            updateTextColors();
        }
        else if (event.key.code == sf::Keyboard::Down) {
            if (option == 2) {
                option = 0;
            }
            else {
                option++;
            }
            updateTextColors();
        }
    }
    else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        
        if (easyText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            option = 0;
            updateTextColors();
        }
        else if (mediumText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            option = 1;
            updateTextColors();
        }
        else if (hardText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            option = 2;
            updateTextColors();
        }
    }
}

int Menu::getDifficulty(){
    return option;
}

void Menu::updateTextColors() {
    easyText.setFillColor(sf::Color::White);
    mediumText.setFillColor(sf::Color::White);
    hardText.setFillColor(sf::Color::White);
    switch (option) {
    case 0:
        easyText.setFillColor(Color::Yellow);
        break;

    case 1:
        mediumText.setFillColor(Color::Yellow);
        break;

    case 2:
        hardText.setFillColor(Color::Yellow);
        break;
    }
}
