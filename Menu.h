#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace std;
using namespace sf;


class Menu{

public:

    Menu(RenderWindow& window);
    void draw();
    void Input(Event& event);
    int getDifficulty();

private:
    RenderWindow& win;
    int option;
    Font font;
    Text title;
    Text easyText;
    Text mediumText;
    Text hardText;
    void updateTextColors();
};

