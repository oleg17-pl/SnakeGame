#pragma once

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <random>

#include "console.h"
#include "vec.h"

enum class MoveDirection { UP, LEFT, DOWN, RIGHT };

class Game
{
private:
	int fieldSizeX, fieldSizeY;
	std::vector<vec2<int>> snake;
	vec2<int> applePos, prevTailPos, drawOffset;
	MoveDirection direction;
	bool ateApple, losed, consoleShouldClose, won;
	int score;

	bool checkIsOnSnake(vec2<int> pos)
	{
		for (vec2<int> segment : snake)
		{
			if (segment == pos)
				return true;
		}
		return false;
	}

	void generateRandomApplePos()
	{
		if (checkIsWon())
			return;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> xdistribution(0, fieldSizeX - 1);
		std::uniform_int_distribution<> ydistribution(0, fieldSizeY - 1);

		while (true)
		{
			int X = xdistribution(gen);
			int Y = ydistribution(gen);
			vec2<int> pos(X, Y);
			if (!(checkIsOnSnake(pos) || pos == applePos))
			{
				applePos = pos;
				return;
			}
		}
	}

	void toWorld(vec2<int>& coord)
	{
		if (coord.X < 0)
			coord.X = fieldSizeX + coord.X;
		else if (coord.X >= fieldSizeX)
			coord.X = coord.X - fieldSizeX;
		if (coord.Y < 0)
			coord.Y = fieldSizeY + coord.Y;
		else if (coord.Y >= fieldSizeY)
			coord.Y = coord.Y - fieldSizeY;
	}

	bool checkIsWon()
	{
		bool value{ true };
		for (int Y = 0; Y < fieldSizeY; Y++)
		{
			for (int X = 0; X < fieldSizeX; X++)
			{
				if (!checkIsOnSnake(vec2<int>(X, Y)))
					value = false;
			}
		}
		return value;
	}

public:
	Game(int fieldSizeX, int fieldSizeY, vec2<int> snakePos, vec2<int> drawOffset) :
		fieldSizeX{ fieldSizeX },
		fieldSizeY{ fieldSizeY },
		direction{ MoveDirection::RIGHT },
		drawOffset{ drawOffset },
		ateApple{ false },
		losed{ false },
		score{}
	{
		vec2<int> worldSnakePos{ snakePos };
		toWorld(worldSnakePos);
		snake.push_back(worldSnakePos);
		snake.push_back(worldSnakePos);
		generateRandomApplePos();
	}

	void ProcessInput()
	{
		if (!_kbhit())
			return;

		char input = (char)_getch();
		if (input == 'w' && (direction != MoveDirection::UP && direction != MoveDirection::DOWN))
			direction = MoveDirection::UP;
		else if (input == 'a' && (direction != MoveDirection::LEFT && direction != MoveDirection::RIGHT))
			direction = MoveDirection::LEFT;
		else if (input == 's' && (direction != MoveDirection::DOWN && direction != MoveDirection::UP))
			direction = MoveDirection::DOWN;
		else if (input == 'd' && (direction != MoveDirection::RIGHT && direction != MoveDirection::LEFT))
			direction = MoveDirection::RIGHT;
		else if (input == '0')
			consoleShouldClose = true;
	}

	void Move()
	{
		if (losed || won)
			return;

		if (checkIsWon())
		{
			won = true;
			return;
		}

		prevTailPos = snake.front();
		if (!ateApple)
			snake.erase(snake.begin());
		else
			ateApple = false;

		vec2<int> newHead = snake.back();
		if (direction == MoveDirection::UP)
			newHead.Y--;
		else if (direction == MoveDirection::LEFT)
			newHead.X--;
		else if (direction == MoveDirection::DOWN)
			newHead.Y++;
		else if (direction == MoveDirection::RIGHT)
			newHead.X++;
		toWorld(newHead);

		if (checkIsOnSnake(newHead))
		{
			losed = true;
			return;
		}

		snake.insert(snake.end(), newHead);

		if (newHead == applePos)
		{
			ateApple = true;
			generateRandomApplePos();
			score++;
			return;
		}
	}

	void DrawStatic(HANDLE hConsole)
	{
		static const std::wstring SNAKEGAME_TEXT{ L"Snake Game" };
		static const std::wstring EXIT_TEXT{ L"Press zero to exit..." };
		std::wstring fieldBorders;
		for (int Y = -1; Y <= fieldSizeY; Y++)
		{
			for (int X = -1; X <= fieldSizeX; X++)
			{
				vec2<int> pos{ X, Y };
				if (pos.X == -1 || pos.X == fieldSizeX || pos.Y == -1 || pos.Y == fieldSizeY)
					fieldBorders.push_back(L'#');
				else
					fieldBorders.push_back(L' ');
			}
			fieldBorders.push_back(L'\n');
		}

		write(hConsole, SNAKEGAME_TEXT + L"\n\n" + fieldBorders + L"\n\n" + EXIT_TEXT, { (short)drawOffset.X, (short)drawOffset.Y });
	}

	void DrawDynamic(HANDLE hConsole)
	{
		static const std::wstring GAMEOVER_TEXT{ L"Game Over!" };
		static const std::wstring WON_TEXT{ L"You won!" };
		const std::wstring SCORE_TEXT{ L"Your score: " + std::to_wstring(score) };
		std::wstring fieldStr;

		fieldStr.push_back(L'\t');
		for (int Y = 0; Y < fieldSizeY; Y++)
		{
			for (int X = 0; X < fieldSizeX; X++)
			{
				vec2<int> pos(X, Y);
				wchar_t sym{ L' ' };
				if (pos == applePos && !checkIsWon())
					sym = L'$';
				else if (checkIsOnSnake(pos))
					sym = L'@';
				fieldStr.push_back(sym);
			}
			fieldStr.push_back(L'\n');
			fieldStr.push_back(L'\t');
		}

		write(hConsole, L"\n\n\n" + fieldStr + L"\n\n" + SCORE_TEXT + (won ? L"\n\n\n" + WON_TEXT : L"") + (losed ? L"\n\n\n" + GAMEOVER_TEXT : L""), { (short)drawOffset.X, (short)drawOffset.Y });
	}

	bool ConsoleShouldClose()
	{
		return consoleShouldClose;
	}

	bool Over()
	{
		return losed || won;
	}

	int GetScore()
	{
		return score;
	}
};