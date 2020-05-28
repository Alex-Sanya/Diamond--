//
//    !!!��������!!!
// ��� ������ � ���������� ���������� ������� ������ ������: ������ = 100, ������ = 75.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
// ������� ������
#define MAX_SCREEN_X 100
#define MAX_SCREEN_Y 75
// ������� ������ ����� : �� ����� ��������� �� ��� ����, � ���� � �����
#define MAX_MAP_SCREEN_X 55
#define MAX_MAP_SCREEN_Y 34
#define COUNT_CONFORMITY_TYPES 6

// ���� ������
#define RAM_IS_OVER 0
#define FILE_NOT_FOUND 404
#define NO_ENOUGH_DATA 2
#define INCORRECT_VALUE 3

// ���� ������
enum type_cell
{
		type_bush = 'b', // ������, ������������ ������ � �����
		type_p_bush = 176, // ������ b �� ������ 
		type_exit = 'x', // �����
		type_p_exit = '#', // ����� ������ 
		type_grass = 'g', // ��������� ������
		type_p_grass = 'X',
		type_stone = 's', // ������
		type_p_stone = '0',
		type_wall = 'w', // �����
		type_p_wall = 177,
		// ����� � �������� ������������� �� �����
};
// �����������
enum direction
{
	up = 'w',
	left = 'a',
	down = 's',
	right = 'd'
};
// ����� � �������
enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};
// ������������ ��� ������ - ���� ������
typedef struct _s_conformity
{
	unsigned short backgrownd,
	  bush,
	  exit,
	  grass,
	  stone,
	  wall;
	//��� ��������� �������� COUNT_CONFORMITY_TYPES � confirm.txt
} s_conformity;
// ��������� ������ (����� ���� ���������)
typedef struct _s_player
{
	COORD pos; // ������� �� �����
	char ch; // ������ ������
	unsigned short	color; // ���� ���� � ���� ������
} s_player;
// ��������� �����
typedef struct _s_enemy
{
	COORD pos;
	char ch; // ������ ����
	unsigned short	color; // ���� ���� � ���� ������
	direction d; //
	struct _enemy *next, *prev;
} s_enemy;
// ��������� �����
typedef struct _s_stone
{
	COORD pos; // ������� �����
	char ch; // ������ �����
	unsigned short	color; // ���� �����
	_s_stone *next; // ��������� �� ���������
} s_stone;
// ��������� ������� ������
typedef struct _s_q_stone
{
	s_stone *head, // ������ �������
		*tail; // ����� �������
} s_q_stone;
// ��������� ������ �����
typedef struct _s_cell
{
	char ch; // ������
	char color; // ���� ������
	direction dir; // ����������� ������������ ��������
	s_enemy *en; // ��������� �� ����� (���� �� ������ ������ ����)
	s_player *pl; // ��������� �� ������ (���� �� ������ ������ �����)
} s_cell;
// ��������� ���� �����
typedef struct _s_map
{
	COORD size;
	s_cell **matr;
	char *characters;
	unsigned short *colors;
} s_map;
// ����� ��������� �� ���� ������. ����� ���������� 0
int err(int type)
{
	// ����� � ����������� ����� ������ (� �������)
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	switch(type)
	{
	case RAM_IS_OVER:
		printf("\nRAM is over!\n");
		break;
	case FILE_NOT_FOUND:
		printf("\nError 404: file not found");
		break;
	case NO_ENOUGH_DATA:
		printf("\nNo enough data...");
		break;
	case INCORRECT_VALUE:
		printf("\nIncorrect value");
		break;
	default: return 0;
	}
	system("pause");
	return 0;
}

HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

// ��������� ����� �����.
// �� ���� �������� ���������� ������ ����������� �����, �����(�������+�����), ���������� ������, ������ ��������� ������
void print_map(s_map map, COORD screen_pos)
{
	DWORD dw=0; // ����� ������� �� �����
	COORD now={0,0};
	int size_X = MAX_MAP_SCREEN_X > map.size.X ? map.size.X : MAX_MAP_SCREEN_X;
	int size_Y = MAX_MAP_SCREEN_Y > map.size.Y ? map.size.Y : MAX_MAP_SCREEN_Y;
	SetConsoleActiveScreenBuffer(hConsole); // ��������� ��������� ������ ������
	// ���������� ����� ��� ������ � ������
	for(; now.Y < size_Y; now.Y++)
	{
		WriteConsoleOutputCharacter(hConsole, // ���������� ������ ������
			map.characters + (screen_pos.Y + now.Y)*size_X, // ������ ��������
			map.size.X, // ����� ������
			now, // ���������� ������ ������ �� �������
			&dw); // ���������� �������
		WriteConsoleOutputAttribute(
			hConsole, // ���������� ��������� ������
			map.colors + (screen_pos.Y + now.Y)*size_X, // ������ ������
			map.size.X, // ����� ������
			now, // ���������� ������ ������ �� �������
			&dw);// ���������� �������
	}
	return;
}

// ������� �������� ����� �� ���� ����� � ��� �����
int type_char_to_print(char *c)
{
	switch(int(*c))
	{
	case type_bush:
		*c = type_p_bush;
		return 1;
	case type_exit:
		*c = type_p_exit;
		return 1;
	case type_grass:
		*c = type_p_grass;
		return 1;
	case type_stone:
		*c = type_p_stone;
		return 1;
	case type_wall:
		*c = type_p_wall;
		return 1;
	default:
		return err(INCORRECT_VALUE);
	};
	return 0;
}

// ������� ���� �������� ����� �� ���� ����� � ��� ������
int map_characters_to_print(s_map *map)
{
	for(int i=0; i<map->size.X*map->size.Y; i++)
		if( !type_char_to_print( &(map->characters[i]) ) )
			return 0;
	return 1;
}

// �������� ����� �� ���������� �����
int create_map(char *txt_name, s_map *map)
{
	FILE *fmap = fopen(txt_name, "r");
	if(!fmap)
		return err(FILE_NOT_FOUND);
	if( fscanf(fmap, "%d%d", &map->size.X, &map->size.Y) < 2 )
		return err(NO_ENOUGH_DATA);
	if( map->size.X<1 || map->size.Y<1 )
		return err(INCORRECT_VALUE);
	char c=0;
	int i=0;
	if( !(map->characters = (char*)malloc( map->size.X*map->size.Y*sizeof(char) )) )
	{
		fclose(fmap);
		return err(RAM_IS_OVER);
	}
	while( (c=fgetc(fmap)) != EOF ) // ���� �� ����� �����
	{
		if( (map->characters[i] = c) == '\n' )
			continue;
		i++;
		if( i==map->size.X * map->size.Y ) // ����� ������� ���������
			break;
	}
	fclose(fmap);
	if( i<map->size.X * map->size.Y )
	{
		free(map->characters);
		return err(NO_ENOUGH_DATA);
	}
	return 1;
}

// ������� ������ ����� � ������� �������� �����
int str2color(char *str)
{
	if(!strcmp(str, "Black\n"))
		return Black;
	if(!strcmp(str, "Blue\n"))
		return Blue;
	if(!strcmp(str, "Green\n"))
		return Green;
	if(!strcmp(str, "Cyan\n"))
		return Cyan;
	if(!strcmp(str, "Red\n"))
		return Red;
	if(!strcmp(str, "Magenta\n"))
		return Magenta;
	if(!strcmp(str, "Brown\n"))
		return Brown;
	if(!strcmp(str, "LightGray\n"))
		return LightGray;
	if(!strcmp(str, "DarkGray\n"))
		return DarkGray;
	if(!strcmp(str, "LightBlue\n"))
		return LightBlue;
	if(!strcmp(str, "LightGreen\n"))
		return LightGreen;
	if(!strcmp(str, "LightRed\n"))
		return LightRed;
	if(!strcmp(str, "LightCyan\n"))
		return LightCyan;
	if(!strcmp(str, "LightMagenta\n"))
		return LightMagenta;
	if(!strcmp(str, "Yellow\n"))
		return Yellow;
	if(!strcmp(str, "White\n"))
		return White;
	return err(INCORRECT_VALUE);
}

// ������� ������ ����� � ������� �������� �����, ������ ������ �� �����
int str2color_from_file(unsigned short *parametr, FILE **fin)
{
	char str_f[21]={0,}; // ���� ��������� ����� � ���� ������
	if(!fgets(str_f, 20, *fin))
	{
		fclose(*fin);
		return err(NO_ENOUGH_DATA);
	}
	*parametr = str2color(str_f);
	return 1; // ���� ��������� ����� �� ���� �������
}

// �������� ������������ ���� ������ � ������ �� ���������� �����
int create_type_colors(char *txt_conformity, s_conformity *type_colors)
{
	FILE *fconf = fopen(txt_conformity, "r");
	if(!fconf)
		return err(FILE_NOT_FOUND);
	unsigned short *p = &(type_colors->backgrownd);
	// ���������� ����� type_colors
	for(int i=0; i<COUNT_CONFORMITY_TYPES; i++, p++) // p++ - ������� � ���������� ���� ��������� s_conformity
		if(!str2color_from_file(p, &fconf))
			return 0*fclose(fconf); // ������� ���� � ������� ����
		else
			printf("\n%d", *p);
	fclose(fconf);
	return 1;
}

// �������� ������� ������ �����
int create_map_colors(s_map *map, s_conformity type_colors)
{
	if( !(map->colors = (unsigned short*)malloc( sizeof(unsigned short)*map->size.X*map->size.Y )) )
		return err(RAM_IS_OVER);
	for(int i=0; i<map->size.X*map->size.Y; i++)
	{
		switch(int(map->characters[i]))
		{
		case type_bush:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.bush;
			break;
		case type_exit:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.exit;
			break;
		case type_grass:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.grass;
			break;
		case type_stone:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.stone;
			break;
		case type_wall:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.wall;
			break;
		default:
			map->colors[i] = 0;
		};
	}
	return 1;
}



void main()
{
	printf("Hello from POMAH\n");
	s_map map={0,};
	s_conformity type_colors;
	char conf_txt[]="conformity.txt";
	if(!create_type_colors(conf_txt, &type_colors))
		return;
	unsigned short *p = &type_colors.backgrownd;
	printf("\n\n");
	for(int i=0; i<COUNT_CONFORMITY_TYPES; i++, p++)
		printf("\n%d", *p);
	char map_txt[]="map.txt";
	if( !create_map(map_txt, &map) )
		return;
	printf("\n");
	for(int i=0; i<map.size.X; i++)
	{
		for(int j=0; j<map.size.X; j++)
			printf(" %3c", map.characters[i*map.size.X+j]);
		printf("\n");
	}
	if( !create_map_colors(&map, type_colors) )
	{
		free(map.characters);
		return;
	}
	for(int i=0; i<map.size.Y; i++)
	{
		for(int j=0; j<map.size.X; j++)
			printf(" %3d", map.colors[i*map.size.X+j]);
		printf("\n");
	}
	if( !map_characters_to_print(&map) )
	{
		system("pause");
		free(map.characters);
		free(map.colors);
		return;
	}
	system("pause");
	COORD screen_pos={0,0};
	s_player pl={{10,6},0,0};
	print_map(map,screen_pos);
	system("pause");
	free(map.characters);
	free(map.colors);
	return;
}
