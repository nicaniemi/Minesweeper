#pragma once
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Board.h"

class GameScreen
{
public:
	GameScreen();
	void run();
private:
	Board b;
	sf::RenderWindow window;

	sf::Texture texture_hidden;
	sf::Sprite tile_hidden;
	sf::Image tile_hiddenImage;

	sf::Texture texture_mine;
	sf::Sprite mine;
	sf::Image mine_image;

	sf::Texture texture_flag;
	sf::Sprite flag;
	sf::Image flag_image;

	sf::Texture texture_revealed;
	sf::Sprite tile_revealed;
	sf::Image tile_revealedImage;

	sf::Texture texture_happy;
	sf::Sprite happy;
	sf::Image happy_image;

	sf::Texture texture_sad;
	sf::Sprite sad;
	sf::Image sad_image;

	sf::Texture texture_win;
	sf::Sprite win;
	sf::Image win_image;
	float win_x;
	float win_y;

	sf::Texture texture_debug;
	sf::Sprite debug;
	sf::Image debug_image;
	float debug_x;
	float debug_y;
	bool debugMode;

	vector<sf::Texture> texture_tests;
	vector<sf::Sprite> tests;
	vector<sf::Image> test_images;
	float center_x;
	float center_y;

	vector<sf::Texture> texture_nums;
	vector<sf::Sprite> nums;
	vector<sf::Image> num_images;

	sf::Texture texture_digits;
	vector<sf::Sprite> digit;
	sf::Image digit_image;
	vector<int> digits;
	int countValue;

	void handleEvents();
	void draw();
	void leftClick(int x, int y);
	void rightClick();
	bool gameWon;
};