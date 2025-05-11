#pragma once
#include<SFML/Graphics.hpp>
using namespace sf;

class Pipe{
public:
	Pipe(int);
	Sprite sprite_up, sprite_down;
	static void loadTextures();
	void update(Time&);
	float getRightBound();

private:
	static Texture texture_down;
	static Texture texture_up;
	static int pipe_distance;
	static int move_speed;

	static bool texturesLoaded;
};

