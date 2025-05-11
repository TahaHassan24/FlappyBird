#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<random>
#include<vector>
#include"Bird.h"
#include"Pipe.h"
#include"Menu.h"
using namespace sf;
using namespace std;

class Game {
public:
	Game(RenderWindow&);
	RenderWindow& win;
	void startGameLoop();
private:
	SoundBuffer deadBuffer;
	Sound deadSound;
	Texture bg_texture, ground_texture;
	Sprite bg_sprite, ground_sprite1, ground_sprite2;
	Bird bird;
	bool is_enter_pressed, is_space_pressed;
	bool run_game;
	int move_speed = 270;
	void draw();
	void moveGround(Time&);
	void doProcessing(Time& dt);
	void checkCollisions();
	int pipe_counter, pipe_spawn_time;
	vector<Pipe> pipes;
	random_device rd;
	uniform_int_distribution<int> dist{ 250,550 };
	Font font;
	Text restart;
	void restartBird();
	bool check;
	void Scoring();
	int score;
	Text scoreText;
	bool inMenu;
	Menu menu;
	Text play;
	int currentDifficulty;
	int highscore;
	Text highscoreText;
	string getFileName();
	void loadHighscore();
	bool showSpace;
	Text pressSpace;
	Text menuButton;
	Text musicText;
	Music music;
	bool isMuted;
};

