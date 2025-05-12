#include"Game.h"
#include"Global.h"
#include<string>
#include<fstream>
using namespace sf;

Game::Game(RenderWindow& window) : win(window),is_enter_pressed(false),is_space_pressed(false), run_game(true), pipe_counter(71), pipe_spawn_time(70), menu(window) {
	win.setFramerateLimit(60);
	bg_texture.loadFromFile("assets/bg.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
	bg_sprite.setPosition(0.f, -250.f);

	ground_texture.loadFromFile("assets/ground.png");
	ground_sprite1.setTexture(ground_texture);
	ground_sprite2.setTexture(ground_texture);

	ground_sprite1.setScale(SCALE_FACTOR, SCALE_FACTOR);
	ground_sprite2.setScale(SCALE_FACTOR, SCALE_FACTOR);

	ground_sprite1.setPosition(0, 578);
	ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().width, 578);

	
	Pipe::loadTextures();



	deadBuffer.loadFromFile("assets/sfx/dead.wav");
	deadSound.setBuffer(deadBuffer);

	font.loadFromFile("assets/flappy-font.ttf");
	restart.setFont(font);
	restart.setCharacterSize(40);
	restart.setFillColor(Color::Yellow);
	restart.setPosition(210, 440);
	restart.setString("Restart");
	restart.setOutlineColor(Color::Black);
	restart.setOutlineThickness(5);
	
	check = false;
	score = 0;
	
	scoreText.setFont(font);
	scoreText.setCharacterSize(40);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(280, 30);
	scoreText.setString("0");
	scoreText.setOutlineColor(Color::Black);
	scoreText.setOutlineThickness(5);

	inMenu = true;

	font.loadFromFile("assets/flappy-font.ttf");
	play.setFont(font);
	play.setCharacterSize(30);
	play.setFillColor(Color::White);
	play.setPosition(130, 630);
	play.setString("Press 'ENTER' to start");
	play.setOutlineColor(Color::Black);
	play.setOutlineThickness(5);

	font.loadFromFile("assets/flappy-font.ttf");
	pressSpace.setFont(font);
	pressSpace.setCharacterSize(30);
	pressSpace.setFillColor(Color::White);
	pressSpace.setPosition(130, 430);
	pressSpace.setString("Press 'SPACE' to Flap");
	pressSpace.setOutlineColor(Color::Black);
	pressSpace.setOutlineThickness(5);

	font.loadFromFile("assets/flappy-font.ttf");
	highscoreText.setFont(font);
	highscoreText.setCharacterSize(50);
	highscoreText.setFillColor(Color::White);
	highscoreText.setPosition(150, 210);
	highscoreText.setOutlineColor(Color::Black);
	highscoreText.setOutlineThickness(5);

	currentDifficulty = 0;
	showSpace = true;

	font.loadFromFile("assets/flappy-font.ttf");
	menuButton.setFont(font);
	menuButton.setCharacterSize(40);
	menuButton.setFillColor(Color::Yellow);
	menuButton.setPosition(210, 500);
	menuButton.setString("Menu");
	menuButton.setOutlineColor(Color::Black);
	menuButton.setOutlineThickness(5);
	
	music.openFromFile("assets/sfx/bgm.ogg");
	music.setLoop(true);
	music.play();
	music.setVolume(25);
	isMuted = false;

	musicText.setFont(font);
	musicText.setCharacterSize(20);
	musicText.setFillColor(Color::White);
	musicText.setPosition(300, 20);
	musicText.setString("Music ( 'M' to toggle) : On");
	musicText.setOutlineColor(Color::Black);
	musicText.setOutlineThickness(2);

}

void Game::doProcessing(Time& dt)
{
	if (is_enter_pressed){
		moveGround(dt);

		if (pipe_counter > pipe_spawn_time){
			pipes.push_back(Pipe(dist(rd)));
			pipe_counter = 0;
		}
		pipe_counter++;

		for (int i = 0; i < pipes.size(); i++) {
			pipes[i].update(dt);
			if (pipes[i].getRightBound() < 0) {
				pipes.erase(pipes.begin() + i);
			}
		}

		checkCollisions();
		Scoring();
	}
	bird.update(dt);
}
void Game::startGameLoop(){
	Clock clock;
	
	while (win.isOpen()){
		Time dt = clock.restart();
		Event event;
		
		while (win.pollEvent(event)){
			if (event.type == Event::Closed){
				win.close();
			}

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::M) {
					if (isMuted) {
						music.play();
						isMuted = false;
						musicText.setString("Music ( 'M' to toggle ) : On");
					}
					else {
						music.pause();
						isMuted = true;
						musicText.setString("Music ( 'M' to toggle ) : Off");
					}
				}
			}

			if (inMenu) {

				menu.Input(event);

				if ((event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) || (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left))
				{
					int difficulty = menu.getDifficulty();
					currentDifficulty = difficulty;

					switch (difficulty) {
					case 0:

						pipe_spawn_time = 100;
						move_speed = 150.f;
						break;

					case 1:
						pipe_spawn_time = 70;
						move_speed = 200.f;
						break;

					case 2:
						pipe_spawn_time = 50;
						move_speed = 250.f;
						break;

					default:
						pipe_spawn_time = 70;
						move_speed = 200.f;

					}

					inMenu = false;
					is_enter_pressed = true;
					bird.setShouldFly(true);
					loadHighscore();
					restartBird();
				}


			}
			else {
				if (event.type == Event::KeyPressed && run_game) {

					if (event.key.code == Keyboard::Enter && !is_enter_pressed) {
						is_enter_pressed = true;
						showSpace = false;
						bird.setShouldFly(true);
					}
					if (event.key.code == Keyboard::Space && is_enter_pressed) {
						bird.flapBird(dt);
					}
				}
				if ((event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left || event.type == Event::KeyPressed) && !run_game) {
					if (restart.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) || event.key.code == Keyboard::Enter || event.key.code == Keyboard::Space) {
						restartBird();
					}
					if (menuButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						inMenu = true;
						run_game = true;
					}
				}
			}
			
			
		}

		
		if (inMenu) {
			win.draw(bg_sprite);
			menu.draw();
			win.draw(musicText);
		}
		else {
			doProcessing(dt);
			draw();
			
		}
		
		win.display();
	}
}

void Game::checkCollisions(){
	if (pipes.size() > 0){
		if (pipes[0].sprite_down.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) || pipes[0].sprite_up.getGlobalBounds().intersects(bird.bird_sprite.getGlobalBounds()) || bird.bird_sprite.getGlobalBounds().top >= 540) {
			is_enter_pressed = false;
			run_game = false;
			deadSound.play();
			highscoreText.setString("Score: " + to_string(score) + "\nHigh Score: " + to_string(highscore));
		}
	}
}

void Game::draw(){
	win.draw(bg_sprite);
	for (Pipe& pipe : pipes){
		win.draw(pipe.sprite_down);
		win.draw(pipe.sprite_up);
	}

	win.draw(ground_sprite1);
	win.draw(ground_sprite2);
	win.draw(bird.bird_sprite);

	if (!run_game) {
		win.draw(restart);
		win.draw(highscoreText);
		win.draw(menuButton);
		win.draw(musicText);
	}

	if (!is_enter_pressed && !inMenu) {
		win.draw(play);
	}
	if (showSpace) {
		win.draw(pressSpace);
		
	}

	if (run_game) {
		win.draw(scoreText);
	}
	if (inMenu) {
		win.draw(musicText);
	}
}

void Game::moveGround(sf::Time& dt){
	ground_sprite1.move(-move_speed * dt.asSeconds(), 0.f);
	ground_sprite2.move(-move_speed * dt.asSeconds(), 0.f);

	if (ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width < 0){
		ground_sprite1.setPosition(ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width, 578);
	}
	if (ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width < 0){
		ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width, 578);
	}

}


void Game::restartBird() {
	bird.resetBirdPosition();
	bird.setShouldFly(false);
	run_game = true;
	
	pipe_counter = 71;
	pipes.clear();
	is_enter_pressed = false;
	score = 0;
	scoreText.setString("0");
	loadHighscore();
}

void Game::Scoring() {
	if (pipes.size() > 0) {
		if (!check) {
			if (bird.bird_sprite.getGlobalBounds().left > pipes[0].sprite_up.getGlobalBounds().left && bird.getRightBound() < pipes[0].getRightBound()) {
				check = true;
			}
		}
		else {
			if (bird.bird_sprite.getGlobalBounds().left > pipes[0].getRightBound()) {
				score++;
				string b = to_string(score);
				scoreText.setString(b);
				check = false;

				if (score > highscore) {
					highscore = score;
					
					string temp2 = to_string(highscore);
					highscoreText.setString("Score: " + b + "\nHigh Score : " + temp2);

					ofstream file(getFileName());
					if (file.is_open()) {
						file << highscore;
						file.close();
					}
				}
			}
		}
	}
}

string Game::getFileName() {
	if (currentDifficulty == 0) {
		return "EasyHighscore.txt";
	}
	else if (currentDifficulty == 2) {
		return "HardHighscore.txt";
	}
	else {
		return "MediumHighscore.txt";
	}
}

void Game::loadHighscore() {
	highscore = 0;
	ifstream file(getFileName());
	if (file.is_open()) {
		file >> highscore;
		file.close();
	}
	string temp1 = to_string(score);
	string temp2 = to_string(highscore);
	highscoreText.setString("Score: "+ temp1+"\n\nHigh Score : " + temp2);
}
