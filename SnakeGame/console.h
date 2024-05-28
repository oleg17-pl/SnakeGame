#pragma once

#include <Windows.h>
#include <string>

void write(HANDLE hConsole, std::wstring str, COORD coord, wchar_t newLineChar = L'\n', wchar_t skipChar = L'\t')
{
	COORD startCoord{ coord };
	std::wstring tempStr;
	DWORD dwBytesWritten{};
	for (wchar_t ch : str)
	{
		if (ch == newLineChar)
		{
			if (!tempStr.empty())
			{
				WriteConsoleOutputCharacter(hConsole, tempStr.c_str(), tempStr.length(), coord, &dwBytesWritten);
				tempStr.clear();
			}
			coord.X = startCoord.X;
			coord.Y++;
		}
		else if (ch == skipChar)
		{
			coord.X++;
		}
		else
		{
			tempStr.push_back(ch);
		}
	}
	if (!tempStr.empty())
	{
		WriteConsoleOutputCharacter(hConsole, tempStr.c_str(), tempStr.length(), coord, &dwBytesWritten);
	}
}