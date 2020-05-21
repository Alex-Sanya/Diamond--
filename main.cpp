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

// ������. ����� ���������� 0
int ERR(int type)
{
	switch(type)
	{
	case 0:
		printf("\nRAM is over!\n");
		return 0;
	default: return 0;
	}
	return 0;
}


HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//int b=SetConsoleActiveScreenBuffer(hConsole);

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

char* create_map(char *txt_name, int *map_size_X, int *map_size_Y)
{
	FILE *fmap = fopen(txt_name, "r");
	if(!fmap)
		return NULL;
	fscanf(fmap, "%d%d", map_size_Y, map_size_X);
	char *map;
	if( !(map = (char*)malloc( (*map_size_X)*(*map_size_Y)*sizeof(char) )) )
		return (char*)ERR(0);
	for(int i=0; i<(*map_size_Y)*(*map_size_X); i++)
		if( (map[i] = fgetc(fmap)) == '\n' )
			map[i] = fgetc(fmap);
	return map;
}

void main()
{
	printf("Hello from POMAH\n");
	int map_size_X, map_size_Y;
	char *map;
	if( !(map=create_map("map.txt", &map_size_X, &map_size_Y)) )
	{
	system("pause");
	return;
	}
	printf("\n");
	for(int i=0; i<map_size_Y; i++)
	{
		for(int j=0; j<map_size_X; j++)
			printf("%c", map[i*map_size_X+j]);
		printf("\n");
	}
	system("pause");
	return;
}
