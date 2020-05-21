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

// ��������� ������ (����� ���� ���������)
typedef struct _player
{
	COORD pos; // ������� �� �����
	LPCSTR ch; // ������ ������
	WORD *color; // ���� ���� � ���� ������
} player;


HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
int b=SetConsoleActiveScreenBuffer(hConsole);

// ���������� ������ �� ������������ ����� �����
void print_player_on_screen(COORD screen_pos, player pl)
{
	COORD now = {pl.pos.X-screen_pos.X, pl.pos.Y-screen_pos.Y}; // ���������� ������ �� ������
	DWORD dw = 0; // ����� ������� �� �����
	if( now.X >= 0 && now.X < MAX_MAP_SCREEN_X)
		if(now.Y >= 0 && now.Y < MAX_MAP_SCREEN_Y)
		{
			WriteConsoleOutputCharacter(hConsole, // ���������� ������ ������
				pl.ch, // ������ ��������
				1, // ����� ������
				now, // ���������� ������ ������ �� �������
				&dw); // ���������� �������
			WriteConsoleOutputAttribute(
				hConsole, // ���������� ��������� ������
				pl.color, // ������ ������
				1, // ����� ������
				now, // ���������� ������ ������ �� �������
				&dw);// ���������� �������
		}
	return;
}

// ��������� ����� �����.
// �� ���� �������� ���������� ������ ����������� �����, �����(�������+�����), ���������� ������, ������ ��������� ������
void print_map(char *map,  unsigned short *map_colors, COORD screen_pos, player pl, COORD *enemies)
{
	DWORD dw=0; // ����� ������� �� �����
	COORD now={0,0};
	// ���������� ����� ��� ������ � ������
	for(; now.Y < MAX_MAP_SCREEN_Y; now.Y++)
	{
		WriteConsoleOutputCharacter(hConsole, // ���������� ������ ������
			map + (screen_pos.Y + now.Y)*MAX_MAP_SCREEN_X, // ������ ��������
			MAX_MAP_SCREEN_X, // ����� ������
			now, // ���������� ������ ������ �� �������
			&dw); // ���������� �������
		WriteConsoleOutputAttribute(
			hConsole, // ���������� ��������� ������
			map_colors+(screen_pos.Y + now.Y)*MAX_MAP_SCREEN_X, // ������ ������
			MAX_MAP_SCREEN_X, // ����� ������
			now, // ���������� ������ ������ �� �������
			&dw);// ���������� �������
	}
	// ��������� ������, ���� �� ��������� �� ������������ ����� ������
	print_player_on_screen(screen_pos, pl);
	/* ��������� ������ */
	return;
}

void main()
{
	printf("Hello from POMAH\n");
	system("pause");
	return;
}
