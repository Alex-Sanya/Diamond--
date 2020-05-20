//
//    !!!��������!!!
// ��� ������ � ���������� ���������� ������� ������ ������: ������ = 100, ������ = 75.

#include <stdio.h>
#include <Windows.h>
// ������� ������
#define MAX_SCREEN_X 100
#define MAX_SCREEN_Y 75
// ������� ������ ����� : �� ����� ��������� �� ��� ����, � ���� � �����
#define MAX_MAP_SCREEN_X 55
#define MAX_MAP_SCREEN_Y 34

enum tupe_cell
{
		grass = 'g',// ��������� ������
		bush = 'b',// ������, ������������ ������ � �����
		stone = 's',// ������
		wall = 'w',// �����
		// ����� � �������� ������������� �� �����
};

HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
int b=SetConsoleActiveScreenBuffer(hConsole);

// ��������� ����� �����.
// �� ���� �������� ���������� ������ ����������� �����, �����(�������+�����), ���������� ������, ������ ��������� ������
int print_map(char *map, COORD pos, unsigned short *map_colors, COORD player, COORD *enemies)
{
	DWORD dw=0; // ����� ������� �� �����
	for(COORD now={0,0}; now.Y < MAX_MAP_SCREEN_Y; now.Y++)
	{
		WriteConsoleOutputCharacter(hConsole, // ���������� ������ ������
			map + (pos.Y + now.Y)*MAX_MAP_SCREEN_X, // ������ 
			MAX_MAP_SCREEN_X, // ����� ������
			now, // ���������� ������ ������ �� �������
			&dw); // ���������� �������
	}
	/* ��������� ������ */
	/* ��������� ������ */
	return 0;
}

void main()
{
	printf("Hello from POMAH\n");
	system("pause");
	return;
}
