#include <iostream>
#include <vector>
#include <map>
#include<array>
#include<cstdlib>
#include <SFML/Graphics.hpp>
#include "GameScreen.h"
#include "Board.h"
using namespace std;

GameScreen::GameScreen()
{
	window.create(sf::VideoMode((b.getWidth() * 32), ((b.getHeight() * 32) + 100)), "Minesweeper");

	mine_image.loadFromFile("images/mine.png");
	texture_mine.loadFromImage(mine_image);
	mine.setTexture(texture_mine);

	tile_hiddenImage.loadFromFile("images/tile_hidden.png");
	texture_hidden.loadFromImage(tile_hiddenImage);
	tile_hidden.setTexture(texture_hidden);

	flag_image.loadFromFile("images/flag.png");
	texture_flag.loadFromImage(flag_image);
	flag.setTexture(texture_flag);

	tile_revealedImage.loadFromFile("images/tile_revealed.png");
	texture_revealed.loadFromImage(tile_revealedImage);
	tile_revealed.setTexture(texture_revealed);

	happy_image.loadFromFile("images/face_happy.png");
	texture_happy.loadFromImage(happy_image);
	happy.setTexture(texture_happy);

	sad_image.loadFromFile("images/face_lose.png");
	texture_sad.loadFromImage(sad_image);
	sad.setTexture(texture_sad);

	win_image.loadFromFile("images/face_win.png");
	texture_win.loadFromImage(win_image);
	win.setTexture(texture_win);
	win_x = ((float)((b.getWidth() * 32) / 2) - 32);
	win_y = (float)(b.getHeight() * 32);

	debug_image.loadFromFile("images/debug.png");
	texture_debug.loadFromImage(debug_image);
	debug.setTexture(texture_debug);
	debug_x = ((float)((b.getWidth() * 32) / 2) + 96); 
	debug_y = (float)(b.getHeight() * 32);

	texture_nums.resize(8);
	nums.resize(8);
	num_images.resize(8);

	for (unsigned int i = 0; i < 8; i++)
	{
		num_images[i].loadFromFile("images/number_" + to_string(i+1) + ".png");
		texture_nums[i].loadFromImage(num_images[i]);
		nums[i].setTexture(texture_nums[i]);
	}

	texture_tests.resize(3);
	tests.resize(3);
	test_images.resize(3);
	center_x = (((b.getWidth() * 32) / 2)) + 160;
	center_y = (float)(b.getHeight() * 32);

	for (unsigned int i = 0; i < 3; i++)
	{
		test_images[i].loadFromFile("images/test_" + to_string(i+1) + ".png");
		texture_tests[i].loadFromImage(test_images[i]);
		tests[i].setTexture(texture_tests[i]);
	}

	digit_image.loadFromFile("images/digits.png");
	texture_digits.loadFromImage(digit_image);
	digit.resize(11);

	for (unsigned int i = 0; i < 11; i++)
	{
		digit[i].setTexture(texture_digits);
		digit[i].setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
	}
}

void GameScreen::run()
{
	while (window.isOpen())
	{
		handleEvents();
		draw();
	}
}


void GameScreen::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}

		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				leftClick(event.mouseButton.x, event.mouseButton.y);
			}

			else
			{
				rightClick();
			}
		}
	}
}

void GameScreen::leftClick(int x, int y) // Reveals tiles
{
	if (!b.isGameOver())
	{
		int col = x / 32;
		int row = y / 32;
		if (row >= 0 && col >= 0 && row < b.getHeight() && col < b.getWidth() && !b.getTileVector()[row][col].flagged)
		{
			b.revealTile(row, col);
		}
	}

	if (x < (debug_x + 64) && x > debug_x && y < debug_y + 64 && y > debug_y)
	{
		debugMode = !debugMode;
	}

	if (b.isGameOver() or b.isGameWon())
	{
		if (x > win_x && x < (win_x + 64) && y > win_y && y < (win_y + 64))
		{
			b.build();
			gameWon = false;
		}
	}

	if (x > center_x && x < (center_x + 64) && y > center_y && y < (center_y + 64))
	{
		b.build("boards/testboard1.brd");
		gameWon = false;
		window.create(sf::VideoMode(b.getWidth() * 32, (b.getHeight() * 32) + 100), "Minesweeper");
	}

	if (x > (center_x + 64) && x < (center_x + 128) && y > center_y && y < (center_y + 64))
	{
		b.build("boards/testboard2.brd");
		gameWon = false;
		window.create(sf::VideoMode(b.getWidth() * 32, (b.getHeight() * 32) + 100), "Minesweeper");
	}

	if (x > (center_x + 128) && x < (center_x + 192) && y > center_y && y < (center_y + 64))
	{
		b.build("boards/testboard3.brd");
		gameWon = false;
		window.create(sf::VideoMode(b.getWidth() * 32, (b.getHeight() * 32) + 100), "Minesweeper");
	}
}

void GameScreen::rightClick() // Flags tiles
{
	if (sf::Mouse::isButtonPressed && ! b.isGameOver())
	{
		sf::Vector2i rightPosition = sf::Mouse::getPosition(window);
		int col = rightPosition.x / 32;
		int row = rightPosition.y / 32;
		if (row >= 0 && col >= 0 && row < b.getHeight() && col < b.getWidth())
		{
			b.flagTile(row, col);
		}
	}
}

void GameScreen::draw()
{
	const vector<vector<Tile>> &tileVector = b.getTileVector();
	window.clear(sf::Color::Black);								// Ensure window is empty when we initialize game
																// Draw things to the window with window.draw()
	for (unsigned int i = 0; i < b.getHeight(); i++)
	{
		for (unsigned int j = 0; j < b.getWidth(); j++)
		{
			tile_hidden.setPosition((j*32), (i*32)); // Set position of all sprites
			mine.setPosition((j*32), (i*32));
			flag.setPosition((j*32), (i*32));
			tile_revealed.setPosition((j*32), (i*32));
			happy.setPosition((float)((b.getWidth() * 32) / 2) - 32, (float)(b.getHeight() * 32));
			debug.setPosition((float)((b.getWidth() * 32) / 2) + 96, (float)(b.getHeight() * 32));

			if (tileVector[i][j].neighborMines > 0)
			{
				nums[tileVector[i][j].neighborMines - 1].setPosition((j * 32), (i * 32));
			}
			if (tileVector[i][j].neighborMines == 0)
			{
				nums[tileVector[i][j].neighborMines].setPosition((j*32), (i*32));
			}


			// Actual drawing here

			if (tileVector[i][j].revealed == true && tileVector[i][j].mine != true)
			{
				window.draw(tile_revealed);
				if (tileVector[i][j].neighborMines > 0)
				{ 
					window.draw(nums[tileVector[i][j].neighborMines - 1]);
				}
			}

			else if (tileVector[i][j].mine == true && tileVector[i][j].revealed == true)
			{
				window.draw(tile_revealed);
				window.draw(mine);
			}

			else if (tileVector[i][j].flagged == true)
			{
				window.draw(tile_hidden);
				window.draw(flag);
			}

			else if (tileVector[i][j].mine == true && debugMode == false)
			{
				window.draw(mine);
				window.draw(tile_hidden);
			}

			else if (tileVector[i][j].mine == true && debugMode == true)
			{
				window.draw(tile_hidden);
				window.draw(mine);
			}

			else
			{
				window.draw(tile_hidden);
			}
			
		}
	}

	window.draw(debug);

	for (unsigned int i = 0; i < 3; i++)
	{
		tests[i].setPosition(center_x + (i * 64), center_y);
		window.draw(tests[i]);
	}

	digits.resize(3);
	countValue = b.getNumMines() - b.getFlaggedTiles();

	if (countValue < 0)
	{
		digits[0] = 10;
		digits[1] = (abs((int)countValue) / 10) % 10;
		digits[2] = (abs((int)countValue)) % 10;
	}

	else
	{
		digits[0] = (countValue / 100) % 10;
		digits[1] = (countValue / 10) % 10;
		digits[2] = (countValue) % 10;
	}

	for (unsigned int k = 0; k < 3; k++)
	{
		digit[digits[k]].setPosition((float)(20) + (k * 21), (float)(b.getHeight() * 32));
		window.draw(digit[digits[k]]);
	}

	if (b.isGameOver() == true)
	{
		sad.setPosition((float)((b.getWidth() * 32) / 2) - 32, (float)(b.getHeight() * 32));
		window.draw(sad);
	}

	else if (b.isGameOver() == false && !b.isGameWon())
	{
		window.draw(happy);
	}

	else if (b.isGameOver() == false && b.isGameWon())
	{
		win.setPosition((float)((b.getWidth() * 32) / 2) - 32, (float)(b.getHeight() * 32));
		window.draw(win);

		if (!gameWon)
		{
			gameWon = true;
			for (unsigned int i = 0; i < b.getHeight(); i++)
			{
				for (unsigned int j = 0; j < b.getWidth(); j++)
				{
					if (tileVector[i][j].mine == true && tileVector[i][j].flagged == true)
					{
						continue;
					}

					else if (tileVector[i][j].mine == true && tileVector[i][j].flagged == false)
					{
						b.flagTile(i, j);
					}
				}
			}
		}
	}

	window.display(); // Display what we've printed to the window
}