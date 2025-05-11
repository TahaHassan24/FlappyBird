#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
using namespace sf;

class Bird{
	Texture textures[2];
	SoundBuffer buffer;
	const int gravity;
	const int flap_speed;
	float velocity_y;
	int anim_counter, texture_switch;
	bool should_fly;
public:
	Sound flapSound;
	Sprite bird_sprite;
	Bird();
	void setShouldFly(bool);
	void flapBird(Time&);
	void resetBirdPosition();
	void update(Time&);
	float getRightBound();
};

