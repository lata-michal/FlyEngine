#pragma once

#include "pch.h"

#include <Windows.h> // if not on windows remove it and funcs and vars from it

namespace feng {

	class Log
	{
	public:
		static inline HWND hTextBox = NULL;

		static void Print(const std::string& message)
		{
			if(hTextBox == NULL)
				PrintConsole(message);
			else 
				PrintWin32(message);
		}

		static void SetHandleTextBox(HWND hwnd) { hTextBox = hwnd; }
	private:
		static void PrintConsole(const std::string& message)
		{
			std::cout << message;
		}

		static void PrintWin32(const std::string& message)
		{
			std::wstring prevText;
			DWORD length = GetWindowTextLength(hTextBox);
			if (length > 0)
			{
				LPWSTR buffer = new WCHAR[length + 1];
				GetWindowText(hTextBox, buffer, length + 1);

				prevText += buffer;
				delete[] buffer;
			}

			std::string output;
			uint32_t numLinesToScroll = 0;

			for (auto character : message)
			{
				if (character == '\n')
				{
					output += "\r\n";
					numLinesToScroll += 30;
				}
				else
				{
					output += character;
				}
			}

			std::wstring wmessage = prevText + std::wstring(output.begin(), output.end());

			SetWindowText(hTextBox, wmessage.c_str());

			if (length > 0)
				SendMessage(hTextBox, EM_LINESCROLL, 0, numLinesToScroll);
		}
	};

}