//
//    !!!��������!!!
// ��� ������ � ���������� ���������� ������� ������ ������: ������ = 100, ������ = 75.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <Windows.h>
// ������� ������
#define MAX_SCREEN_X 100
#define MAX_SCREEN_Y 75
// ������� ������ ����� : �� ����� ��������� �� ��� �����, � ���� � �����
#define MAX_MAP_SCREEN_X 42
#define MAX_MAP_SCREEN_Y 26
// ���� ������
#define RAM_IS_OVER 0
#define FILE_NOT_FOUND 404
#define NO_ENOUGH_DATA 2
#define INCORRECT_VALUE 3
// �������������:
#define COUNT_CONFORMITY 6
#define COUNT_TXT_NAME 3
#define MAX_TXT_NAME 66
#define ESC 27

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
// ���� ���� ��������� ������ (������������, ��������, ��� ������ �������)
typedef struct _s_txt_name
{
	// ��� ���������� � �������� �������� COUNT_TXT_NAME
	char conformity[MAX_TXT_NAME];
	char map[MAX_TXT_NAME];
	char player[MAX_TXT_NAME];
} s_txt_name;

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

// ���������� ������ �� ������������ ����� �����
void print_player_on_screen(COORD screen_pos, s_player pl, HANDLE hConsole)
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
// �� ���� �������� ����� � ���������� ������ ����������� �����
void print_map(s_map map, COORD screen_pos)
{
	HANDLE hConsole = CreateConsoleScreenBuffer( GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dw=0; // ����� ������� �� �����
	COORD now={0,0};
	int size_X = MAX_MAP_SCREEN_X > map.size.X ? map.size.X : MAX_MAP_SCREEN_X;
	int size_Y = MAX_MAP_SCREEN_Y > map.size.Y ? map.size.Y : MAX_MAP_SCREEN_Y;
	SetConsoleActiveScreenBuffer(hConsole); // ��������� ��������� ������ ������
	// ���������� ����� ��� ������ � ������
	for(; now.Y < size_Y; now.Y++)
	{
		WriteConsoleOutputCharacter(hConsole, // ���������� ������ ������
			map.characters + (screen_pos.Y + now.Y)*map.size.X, // ������ ��������
			map.size.X, // ����� ������
			now, // ���������� ������ ������ �� �������
			&dw); // ���������� �������
		WriteConsoleOutputAttribute(
			hConsole, // ���������� ��������� ������
			map.colors + (screen_pos.Y + now.Y)*map.size.X, // ������ ������
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
int create_map_characters(s_txt_name txt_name, s_map *map)
{
	FILE *fmap = fopen(txt_name.map, "r");
	if(!fmap)

	return err(FILE_NOT_FOUND);
	if (fscanf(fmap, "%d%d", &map->size.X, &map->size.Y) < 2)
		return err(NO_ENOUGH_DATA);//����������� �����
	if (map->size.X < 1 || map->size.Y < 1)
		return err(INCORRECT_VALUE);
	char c = 0;

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

// �������� s_cell - ������� ��������� m �� n
int create_s_cell_matrix(s_cell ***matrix, int m, int n)
{
	if( !((*matrix)=(s_cell**)malloc(sizeof(s_cell*)*m)) )
		return err(RAM_IS_OVER);
	for(int i=0; i<m; i++)
		if( !((*matrix)[i]=(s_cell*)malloc(sizeof(s_cell)*n)) )
			return err(RAM_IS_OVER);
	return 1;
}

// �������� �������������� ��������� ������ �����
int is_player_on_map(s_player *player, int m, int n)
{
	if(player->pos.X<0 || player->pos.Y<0)
		return 0;
	if(player->pos.X>=n || player->pos.Y>=m)
		return 0;
	return 1;
}

// ��������� ������ �� ����� - �������
int set_player_on_matr(s_map *map, s_conformity conformity, s_player *player)
{
	if( !is_player_on_map(player, map->size.Y, map->size.X) )
		return err(INCORRECT_VALUE);
	map->matr[player->pos.Y][player->pos.X].pl = player; 
	map->matr[player->pos.Y][player->pos.X].ch = type_p_grass;
	map->characters[player->pos.Y*map->size.X+player->pos.X] = player->ch;
	map->matr[player->pos.Y][player->pos.X].color = conformity.backgrownd << 4 | conformity.grass;
	map->colors[player->pos.Y*map->size.X+player->pos.X] = player->color;
	return 1;
}

// �������� �����-�������
int create_map_matr(s_map *map, s_conformity conformity, s_player *player /*,s_empty *First*/)
{
	if( !create_s_cell_matrix(&(map->matr), map->size.Y, map->size.X) )
		return 0;
	for(int i=0; i<map->size.Y; i++)
	{
		for(int j=0; j<map->size.X; j++)
		{
			map->matr[i][j].ch = map->characters[map->size.X*i+j];
			map->matr[i][j].color = map->colors[map->size.X*i+j];
		}
	}
	if(!set_player_on_matr(map, conformity, player))
		return err(INCORRECT_VALUE);
	/*if(!set_enemies_on_matr())
		return 0;*/
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
int create_type_colors(s_txt_name txt_name, s_conformity *type_colors)
{
	FILE *fconf = fopen(txt_name.conformity, "r");
	if(!fconf)
		return err(FILE_NOT_FOUND);
	unsigned short *p = &(type_colors->backgrownd);
	// ���������� ����� type_colors
	for(int i=0; i<COUNT_CONFORMITY; i++, p++) // p++ - ������� � ���������� ���� ��������� s_conformity
		if(!str2color_from_file(p, &fconf))
			return 0*fclose(fconf); // ������� ���� � ������� ����
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
// ������� ��������� ���������� ������
int create_player( s_txt_name txt_name, s_player* player) 
{
		FILE* fplayer = fopen(txt_name.player, "r");
		if (!fplayer)
			return err(FILE_NOT_FOUND);
		if (!fscanf(fplayer, "%d%d", &(player->pos.X), &(player->pos.Y)))
		{
			fclose(fplayer);
			return err(NO_ENOUGH_DATA);
		}
			if (!fscanf(fplayer, "%d", &(player->ch)))
			{
				fclose(fplayer);
				return err(NO_ENOUGH_DATA);
			}
		char exstr[MAX_TXT_NAME];
		fgetc(fplayer);
		if (!fgets(exstr, MAX_TXT_NAME, fplayer))
		{
			fclose(fplayer);
			return err(NO_ENOUGH_DATA);
		}
	player->color=str2color(exstr);
		fclose(fplayer);
		return 1;
	

}
// ������ �����
int reverse_int(int n)
{
	int rev = 0;
	for(int temp=n>0 ? n : -n; temp>0; temp/=10)
		rev = rev*10+(temp%10);
	return n>0 ? rev : -rev;
}
// ������� ����� � ������
void int2str(int n, char str[], int lenght)
{
	int i = n>0 ? 0 : 1,
		rev = reverse_int(n>0 ? n : -n);
	if(i)
		str[0] = '-';
	for(; i<lenght && rev>0; i++, rev/=10)
		str[i]=rev%10+'0';
	return;
}
// �������� � txt_name ����� ��������� ������, ������������ �� ������ ������
int get_txt_name(int level, s_txt_name *txt_name)
{
	char txt_level[MAX_TXT_NAME]={'l','e','v','e','l','_',0};
	int2str(level, txt_level+6, MAX_TXT_NAME-11);// -11 = ( 1 ������ �� '\0', 4 �� ".txt", 6 �� "level_" )
	strcat(txt_level, ".txt");
	FILE *flevel = fopen(txt_level, "r"); // ������� level_*.txt ��� ������
	if(!flevel)
		return err(FILE_NOT_FOUND);
	for(char *p = txt_name->conformity, cnt=0; cnt<COUNT_TXT_NAME; cnt++, p+=MAX_TXT_NAME)
	{
		// �������� ������ �� �����
		if( !(fgets(p, MAX_TXT_NAME, flevel)) )
		{
			fclose(flevel);
			return err(NO_ENOUGH_DATA);
		}
		// ������ ������� ������
		for(int i=0; p[i]!='\0'; i++)
			if(p[i]=='\n')
				p[i]=0;
	}
	return 1;
}
// ����� (?) ������
int get_level(int *level)
{
	// ���� ��������� ����������, �� ���������� ��������� ����
	// � ����� �������� s_txt_name � ������������� COUNT_TXT_NAME
	*level = 1;
	return 1;
}

// ���������� - ������ ���������� �� ������, �������� �����
int preparation(int *level, s_map *map, s_conformity *conformity, s_player *player/* ,s_enemy **first_enemy*/)
{

	// ������ ����� �������???
	if(!get_level(level))
		return 0;
	// �������� ������ ������
	s_txt_name txt_name={0,0,0};
	get_txt_name(1, &txt_name);
	if(!create_type_colors(txt_name, conformity))
		return 0;
	if( !create_map_characters(txt_name, map) )
		return 0;
	if( !create_map_colors(map, *conformity) )
	{
		free(map->characters);
		return 0;
	}
	if( !map_characters_to_print(map) )
	{
		free(map->characters);
		free(map->colors);
		return 0;
	}
	if( !create_player(txt_name, player))
	{
		free(map->characters);
		free(map->colors);
		return 0;
	}
	if( !create_map_matr(map, *conformity, player))
	{
		free(map->characters);
		free(map->colors);
		return 0;
	}
}

int main()
{
	printf("Diamond-- by Alex, Evgen, POMAH.\n");
	system("pause");
	int level=0;
	COORD screen_pos={0,0};
	DWORD dw=0;
	s_map map={0,};
	s_conformity conformity;
	s_txt_name txt_name={0,0,0};
	s_player player = { {0,0},0,0 };
	preparation(&level, &map, &conformity, &player);
	printf("\n%d", map.size.X);
	printf("\n%d", map.size.Y);
	for(int i=0;i<map.size.Y; i++)
	{
		printf("\n");
		for(int j=0; j<map.size.X; j++)
			printf("%c", map.characters[map.size.X*i+j]);
	}
	printf("\n");
	for(int i=0;i<map.size.Y; i++)
	{
		printf("\n");
		for(int j=0; j<map.size.X; j++)
			printf("%c", map.matr[i][j].ch);
	}
	system("pause");
	print_map(map, screen_pos);
	system("pause");
	free(map.characters);
	free(map.colors);
	for(int i=0; i<map.size.Y; i++)
		free(map.matr[i]);
	free(map.matr);
	return 0;
}
