#include <Windows.h>

#include "snakegame.h"
#include "vec.h"

int main()
{
	const float ms_UPDATE_SPEED{ 100 };
	int xsize{}, ysize{};
	std::cout << "Enter x size: ";
	std::cin >> xsize;
	std::cout << "Enter y size: ";
	std::cin >> ysize;
	system("cls");

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;

	SetConsoleActiveScreenBuffer(hConsole);
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	vec2<int> snakePos{ -60, 1 }, offset{ 0, 0 };
	Game game(xsize, ysize, snakePos, offset);

	game.DrawStatic(hConsole);
	while (!game.ConsoleShouldClose())
	{
		game.DrawDynamic(hConsole);
		game.ProcessInput();
		game.Move();
		if (!game.Over())
			Sleep(ms_UPDATE_SPEED);
	}

	return 0;
}
