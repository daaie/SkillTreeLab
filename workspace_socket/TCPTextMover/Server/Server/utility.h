#pragma once
#include <conio.h>
#include <ctime>

////////////////////////
//������ ��ƿ��Ƽ Ŭ����
////////////////////////
class Utility
{
public:
	//��ġ�� ����
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

	//�ܼ� ȭ�鿡 Ŀ���� �ű��
	static void gotoxy(int x, int y)
	{
		COORD Cur;
		Cur.X = x;
		Cur.Y = y;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
	}

	//Ű �Է��� �޾Ƽ� Ű �ڵ带 �Ѱ���
	static int Input()
	{
		int key = _getch();

		return key;
	}

	//ȭ���� ����
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

	//��¥ �����
	static std::string Date()
	{
		const auto now = std::time(nullptr);
		char cstr[256]{};
		return std::strftime(cstr, sizeof(cstr), "%Y%m%d", std::localtime(&now)) ? cstr : "";
	}

	//�α� ����
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
