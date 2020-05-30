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
#define COUNT_CONFORMITY_TYPES 7

enum tupe_cell
{
		bush = 'b', // ������, ������������ ������ � �����
		exit_map = 'x', // �����
		grass = 'g', // ��������� ������
		stone = 's', // ������
		wall = 'w' // �����
		// ����� � �������� ������������� �� �����
};

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

typedef struct _conformity
{
	unsigned short backgrownd,
	  bush,
	  enemy,
	  exit,
	  grass,
	  stone,
	  wall;
	//��� ��������� �������� COUNT_CONFORMITY_TYPES � confirm.txt
}conformity;
// ��������� ������ (����� ���� ���������)
typedef struct _s_player
{
	COORD pos; // ������� �� �����
	char ch; // ������ ������
	unsigned short	color; // ���� ���� � ���� ������
} s_player;

// ����� ��������� �� ���� ������. ����� ���������� 0
int err(int type)
{
	// ����� � ����������� ����� ������ (� �������)
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	switch(type)
	{
	case 0:
		printf("\nRAM is over!\n");
		break;
	case 404:
		printf("\nError 404: file not found");
		break;
	case 500:
		printf("\nNo enough data...");
		break;
	default: return 0;
	}
	system("pause");
	return 0;
}


HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

// ���������� ������ �� ������������ ����� �����
void print_player_on_screen(COORD screen_pos, s_player pl)
{
	COORD now = {pl.pos.X-screen_pos.X, pl.pos.Y-screen_pos.Y}; // ���������� ������ �� ������
	DWORD dw = 0; // ����� ������� �� �����
	char ch[2]={0,}; // ������ ��� ������ ������� ������
	ch[0] = pl.ch;
	WORD color[2]={0,}; // ������ ��� ������ ����� ���� � ������� ������
	color[0] = pl.color;
	if( now.X >= 0 && now.X < MAX_MAP_SCREEN_X)
		if(now.Y >= 0 && now.Y < MAX_MAP_SCREEN_Y)
		{
			WriteConsoleOutputCharacter(hConsole, // ���������� ������ ������
				ch, // ������ ��������
				1, // ����� ������
				now, // ���������� ������ ������ �� �������
				&dw); // ���������� �������
			WriteConsoleOutputAttribute(
				hConsole, // ���������� ��������� ������
				color, // ������ ������
				1, // ����� ������
				now, // ���������� ������ ������ �� �������
				&dw);// ���������� �������
		}
	return;
}

// ��������� ����� �����.
// �� ���� �������� ���������� ������ ����������� �����, �����(�������+�����), ���������� ������, ������ ��������� ������
void print_map(char *map,  unsigned short *map_colors, int map_size_X, int map_size_Y, COORD screen_pos, s_player pl, COORD *enemies)
{
	DWORD dw=0; // ����� ������� �� �����
	COORD now={0,0};
	int size_X = MAX_MAP_SCREEN_X > map_size_X ? map_size_X : MAX_MAP_SCREEN_X;
	int size_Y = MAX_MAP_SCREEN_Y > map_size_Y ? map_size_Y : MAX_MAP_SCREEN_Y;
	SetConsoleActiveScreenBuffer(hConsole); // ��������� ��������� ������ ������
	// ���������� ����� ��� ������ � ������
	for(; now.Y < size_Y; now.Y++)
	{
		WriteConsoleOutputCharacter(hConsole, // ���������� ������ ������
			map + (screen_pos.Y + now.Y)*size_X, // ������ ��������
			size_X, // ����� ������
			now, // ���������� ������ ������ �� �������
			&dw); // ���������� �������
		WriteConsoleOutputAttribute(
			hConsole, // ���������� ��������� ������
			map_colors+(screen_pos.Y + now.Y)*size_X, // ������ ������
			size_X, // ����� ������
			now, // ���������� ������ ������ �� �������
			&dw);// ���������� �������
	}
	// ��������� ������, ���� �� ��������� �� ������������ ����� ������
	print_player_on_screen(screen_pos, pl);
	/* ��������� ������ */
	return;
}

// �������� ����� �� ���������� �����
char* create_map(char *txt_name, int *map_size_X, int *map_size_Y)
{
	FILE *fmap = fopen(txt_name, "r");
	if(!fmap)
		return (char*)err(404);
	if (!fscanf(fmap, "%d%d", map_size_Y, map_size_X))
		return (char*)err(500);
	char *map, c=0;
	int i=0;
	if( !(map = (char*)malloc( (*map_size_X)*(*map_size_Y)*sizeof(char) )) )
	{
		fclose(fmap);
		return (char*)err(0);
	}
	while((c=fgetc(fmap))!=EOF) // ���� �� ����� �����
	{
		if( (map[i] = c) == '\n' )
			continue;
		i++;
		if(i==(*map_size_Y)*(*map_size_X)) // ����� ������� ���������
			break;
	}
	fclose(fmap);
	if(i<(*map_size_Y)*(*map_size_X))
		return (char*)err(500);
	return map;
}

// ������� ������ ����� � ������� �������� �����
int str2color(char *str)
{
	int r=404;
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
	return err(404);
}

// ���������� ���� ��������� conformity
int conf_parametr(unsigned short *parametr, FILE **fconf)
{
	char str_b[20]={0,}, str_f[20]={0,}; // ������ ����� ���� � ��������� �����
	int b=0, f=0; // ����� ���� � ��������� ����� 
	if(!fgets(str_b, 20, *fconf) || !fgets(str_f, 20, *fconf))
	{
		fclose(*fconf);
		return err(500);
	}
	b = str2color(str_b);
	f = str2color(str_f);
	*parametr = b << 4 | f;
	return 1;
}

// �������� ������������ ���� ������ � ������ �� ���������� �����
int create_type_colors(char *txt_conformity, conformity *type_colors)
{
	char str_b[20]={0,}, str_f[20]={0,}; // ������ ����� ���� � ��������� �����
	FILE *fconf = fopen(txt_conformity, "r");
	if(!fconf)
		return err(404);
	unsigned short *p = &(type_colors->backgrownd);
	// ���������� ����� type_colors
	for(int i=0; i<COUNT_CONFORMITY_TYPES; i++, p++)
		if(!conf_parametr(p, &fconf))
			return 0;
	fclose(fconf);
	return 1;
}

// �������� ������� ������ �����
unsigned short * create_map_colors(char map[], int map_size_X, int map_size_Y, conformity type_colors)
{
	unsigned short *map_colors;
	if( !(map_colors = (unsigned short*)malloc( sizeof(unsigned short)*map_size_X*map_size_Y )) )
		return (unsigned short *)err(0);
	for(int i=0; i<map_size_X*map_size_Y; i++)
	{
		switch(int(map[i]))
		{
		case bush:
			map_colors[i] = type_colors.bush;
			break;
		case exit_map:
			map_colors[i] = type_colors.exit;
			break;
		case grass:
			map_colors[i] = type_colors.grass;
			break;
		case stone:
			map_colors[i] = type_colors.stone;
			break;
		case wall:
			map_colors[i] = type_colors.wall;
			break;
		default:
			map_colors[i] = 0;
		};
	}
	return map_colors;
}
int create_player(s_player* player, char* txt_name)
{
		FILE* fplayer = fopen(txt_name, "r");
		if (!fplayer)
			return err(404);
		if (!fscanf(fplayer, "%d%d",&(player->pos.X),&(player->pos.Y)))
			return err(500);
		if (!fscanf(fplayer, "%d",&(player->ch)))
			return err(500);
		char exstr[20];
		fgetc(fplayer);
		if (!fgets(exstr,20,fplayer))
			return err(500);
	(player->color)=str2color(exstr);
		return 1;

}

void main()
{
	printf("Hello from POMAH\n");
	int map_size_X, map_size_Y;
	char *map;
	unsigned short *map_colors;
	conformity type_colors;
	char s[] = "conformity.txt";
	if(!create_type_colors(s, &type_colors))
		return;
	unsigned short *p = &type_colors.backgrownd;
	for(int i=0; i<COUNT_CONFORMITY_TYPES; i++, *p+=sizeof(unsigned short))
		printf("\n%d", *p);
	char s2[] = "map.txt";
	if( !(map=create_map(s2, &map_size_X, &map_size_Y)) )
		return;
	printf("\n");
	for(int i=0; i<map_size_Y; i++)
	{
		for(int j=0; j<map_size_X; j++)
			printf(" %3c", map[i*map_size_X+j]);
		printf("\n");
	}
	if(! (map_colors = create_map_colors(map, map_size_X, map_size_Y, type_colors)) )
	{
		free(map);
		return;
	}
	for(int i=0; i<map_size_Y; i++)
	{
		for(int j=0; j<map_size_X; j++)
			printf(" %3d", map_colors[i*map_size_X+j]);
		printf("\n");
	}
	system("pause");
	COORD screen_pos={0,0};
	char s3[] = "player.txt";
	s_player pl={1,0,0,0};
	create_player(&pl, s3);
	print_map(map, map_colors, map_size_X, map_size_Y, screen_pos, pl, &screen_pos);
	create_player( &pl,s3);
	system("pause");
	free(map);
	free(map_colors);
	return;
}
