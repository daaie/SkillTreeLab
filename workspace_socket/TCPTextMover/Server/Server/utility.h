#pragma once
#include <conio.h>
#include <ctime>

////////////////////////
//여러가 유틸리티 클래스
////////////////////////
class Utility
{
public:
	//수치를 제한
	static void clamp(int& n, int min, int max)
	{
		if (n < min)
		{
			n = min;
		}
		else if (n > max)
		{
			n = max;
		}
	}

	//콘솔 화면에 커서를 옮기기
	static void gotoxy(int x, int y)
	{
		COORD Cur;
		Cur.X = x;
		Cur.Y = y;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
	}

	//키 입력을 받아서 키 코드를 넘겨줌
	static int Input()
	{
		int key = _getch();

		return key;
	}

	//화면을 삭제
	static void ClearScreen()
	{
		system("cls");
	}

	//Timestamp
	static std::string TimeStamp()
	{
		const auto now = std::time(nullptr);
		char cstr[256]{};
		return std::strftime(cstr, sizeof(cstr), "%Y%m%d_%H%M%S", std::localtime(&now)) ? cstr : "";
	}

	//날짜 만들기
	static std::string Date()
	{
		const auto now = std::time(nullptr);
		char cstr[256]{};
		return std::strftime(cstr, sizeof(cstr), "%Y%m%d", std::localtime(&now)) ? cstr : "";
	}

	//로그 쓰기
	static void Log(std::string& log)
	{
		std::string logfile = std::string(Utility::Date() + ".log");
		FILE* fp = fopen(logfile.c_str(), "a+");
		if (fp)
		{
			std::string temp = Utility::TimeStamp() + " " + log;
			fprintf(fp, "%s\n", temp.c_str());
			fclose(fp);
		}
	}

};
