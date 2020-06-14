//
//    !!!��������!!!
// ��� ������ � ���������� ���������� ������� ������ ������: ������ = 100, ������ = 75.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <Windows.h>
#include <time.h>
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
#define BORDER_SIZE 2
#define BORDER_CHAR '%'
#define ESC 27
#define MAX(x,y) (x)>(y) ? (x) : (y);
#define MIN(x,y) (x)<(y) ? (x) : (y);

// ���� ������
enum type_cell
{
		type_bush = 'b', // �����: ������, ������������ ������ � �����. ��� ��������� ������ �� �����, ����� �������� � ������������ � �����.
		type_p_bush = 176, // ������ b �� ������ 
		type_exit = 'x', // �����
		type_p_exit = '#',
		type_grass = 'g', // ��������� ������
		type_p_grass = ' ',
		type_stone = 's', // ������
		type_p_stone = '0',
		type_wall = 'w', // �����
		type_p_wall = 219,
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


// ���������� ������, ��������� ������ �� �������� c. ���� is_new_line!=0, �������� ������� �� ����� ������.
void print_line(char c, int size, int is_new_line)
{
	for(int i=0; i<size; i++)
		printf("%c", c);
	if(is_new_line)
		printf("\n");
}

// ���������� ������ �� ������������ ����� �����
int print_player(COORD screen_pos, s_player pl, int i, int j)
{
	if(pl.pos.X == j && pl.pos.Y == i)
	{
		printf("%c", pl.ch);
		return 1;
	}
	return 0;
}

// ���������� ������ ����� (�����, ����� ��� ������)
void print_cell(s_map map, int i, int j)
{
	printf("%c", map.matr[i][j].ch);
}

// ��������� ����� �����.
// �� ���� �������� ����� � ���������� ������ ����������� �����
void print_map(s_map map, COORD screen_pos, s_player player/*, s_enemies First*/)
{
	// ������� ���������� ������ ����
	int size_X = MIN(MAX_MAP_SCREEN_X, map.size.X);
	int size_Y = MIN(MAX_MAP_SCREEN_Y, map.size.Y);
	// ������� ������ ������� ���������� ������ ����
	for(int i=0; i<BORDER_SIZE; i++)
		print_line(BORDER_CHAR, BORDER_SIZE*2+size_X, 1);
	for(int i=0; i<size_Y; i++)
	{
		// ����� ����
		print_line(BORDER_CHAR, BORDER_SIZE, 0);
		// ������� �����
		for(int j=0; j<size_X; j++)
		{ 
			if(print_player(screen_pos, player, i, j))
				continue;
			print_cell(map, i, j);
		}
		// ����� ����
		print_line(BORDER_CHAR, BORDER_SIZE, 1);
	}
	// ������ ������ ������� ������
	for(int i=0; i<BORDER_SIZE; i++)
		print_line(BORDER_CHAR, BORDER_SIZE*2+size_X, 1);
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
			map->matr[i][j].dir = left;
			map->matr[i][j].pl = NULL;
			map->matr[i][j].en = NULL;
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
int create_all_colors(s_txt_name txt_name, s_conformity *type_colors)
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
	if(!get_txt_name(1, &txt_name))
		return 0;
	if(!create_all_colors(txt_name, conformity))
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
	map->matr[player->pos.Y][player->pos.X].pl = player;// ��������� ������ �� �����
	return 1;
}

// ����� 1, ���� ������ ������ - �����, 0 - ���� �� �����, ��� ���� �����
int is_grass(s_map *map, int X, int Y)
{
	if(X<0 || X>=map->size.X)
		return 0;
	if(Y<0 || Y>=map->size.Y)
		return 0;
	if(map->matr[Y][X].ch == type_p_grass || map->matr[Y][X].ch==type_grass)
		return 1;
	return 0;
}

// �������� �������� ������� ������
s_stone* create_stone(s_cell *info, int X, int Y)
{
	s_stone *new_stone = (s_stone*)malloc(sizeof(s_stone));
	if(!new_stone)
		return (s_stone*)err(RAM_IS_OVER);
	// ���������� �����
	new_stone->ch = info->ch;
	new_stone->color = info->color;
	new_stone->next = NULL;
	new_stone->pos.X = X;
	new_stone->pos.Y = Y;
	return new_stone;
}

// ���������� � �����
int add_stone_in_end(s_q_stone *q_stone, s_stone *stone)
{
	// �������� �� �������������
	if(!stone)
		return err(NO_ENOUGH_DATA);
	// ������� �����
	if(!q_stone->head)
	{
		q_stone->head = q_stone->tail = stone;
		return 1;
	}
	// ������� �� �����
	q_stone->tail->next = stone;
	q_stone->tail = stone;
	return 1;
}

// ����������� ���������� � �������
int rec_add_in_q(s_q_stone *q_stone, s_map *map, int X, int Y)
{
	if(X<0 || X>=map->size.X)
		return 1; // ���� �����-�� �������������
	if(Y<0 || Y>=map->size.Y)
		return 1; // ���� �����-�� �������������
	if(map->matr[Y][X].ch != type_p_stone)
		return 1; // ��� �� ������ - ����� �� ���� ��������
	if( ! (is_grass(map, X, Y+1) || // ����� ������ ����
		is_grass(map, X-1, Y)&&is_grass(map, X-1, Y+1)&&map->matr[Y+1][X].ch == type_p_stone || // ����� ������ ����� � ������ ����� �� �����
		is_grass(map, X+1, Y)&&is_grass(map, X+1, Y+1)&&map->matr[Y+1][X].ch == type_p_stone) ) // ����� ������ ������ � ������ ����� �� �����
		return 1; // ����� ������ ������ - ����� �� ���� ��������
	if(!add_stone_in_end(q_stone, create_stone(map->matr[Y]+X, X, Y)))
		return 0;
	map->matr[Y][X].ch = type_p_grass;
	if(!rec_add_in_q(q_stone,map,X,Y-1))
		return 0;
	if(!rec_add_in_q(q_stone,map,X-1,Y-1))
		return 0;
	if(!rec_add_in_q(q_stone,map,X+1,Y-1))
		return 0;
	return 1;
}

// �������� �� ������� ������, ��������� ����, � ������, ���� �� ���������� ��������� ������
void del_from_q_stone(s_q_stone *q_stone, s_map *map)
{
	while(q_stone->head)
	{
		int X = q_stone->head->pos.X, Y = q_stone->head->pos.Y;
		// ����� ��������
		if(is_grass(map, X, Y+1))
			return;
		// ����� ��������
		if(is_grass(map, X-1, Y)&&is_grass(map, X-1, Y+1)&&map->matr[Y+1][X].ch == type_p_stone)
			return;
		// ������ ��������
		if(is_grass(map, X+1, Y)&&is_grass(map, X+1, Y+1)&&map->matr[Y+1][X].ch == type_p_stone)
			return;
		s_stone *cur = q_stone->head; // ��������� ��� ������������ ������
		map->matr[Y][X].ch = type_p_stone; // ���������� �� �����
		q_stone->head = cur->next; // ������ �� �������
		free(cur); // ������������ ������
	}
	// � ������� ������ ���
	q_stone->head = q_stone->tail = NULL;
}

// ������� ���� �������
void q_stone_clear(s_q_stone *q_stone)
{
	while(q_stone->head)
	{
		s_stone *cur = q_stone->head;
		q_stone->head = cur->next;
		free(cur);
	}
	q_stone->head = q_stone->tail = NULL;
}

// �������� ���� ������ �� 1 ���
void move_stone(s_q_stone *q_stone, s_map *map)
{
	for(s_stone *cur=q_stone->head; cur; cur=cur->next)
	{
		SHORT *X = &(cur->pos.X), *Y = &(cur->pos.Y);
		// ����� ��������
		if( is_grass(map, *X, *Y+1) && !map->matr[*Y+1][*X].pl )
		{
			map->matr[*Y][*X].ch = type_p_grass;
			cur->pos.Y++;
			map->matr[*Y][*X].ch = type_p_stone;
			continue;
		}
		// ����� ��������
		if( is_grass(map, *X-1, *Y)&&is_grass(map, *X-1, *Y+1)&&map->matr[*Y+1][*X].ch == type_p_stone && !map->matr[*Y+1][*X].pl )
		{
			map->matr[*Y][*X].ch = type_p_grass;
			cur->pos.X--;
			cur->pos.Y++;
			map->matr[*Y][*X].ch = type_p_stone;
			continue;
		}
		// ������ ��������
		if( is_grass(map, *X+1, *Y)&&is_grass(map, *X+1, *Y+1)&&map->matr[*Y+1][*X].ch == type_p_stone && !map->matr[*Y+1][*X].pl )
		{
			map->matr[*Y][*X].ch = type_p_grass;
			cur->pos.X++;
			cur->pos.Y++;
			map->matr[*Y][*X].ch = type_p_stone;
			continue;
		}
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
	s_q_stone q_stone = {0,0};
	preparation(&level, &map, &conformity, &player);
	print_map(map, screen_pos, player);
	system("pause");
	int d=clock();
	char c=0;
	while(c!=ESC)
	{
		if(_kbhit())
			{
				if( (c=_getch()) == ESC )
					break;
				while(_kbhit())
					char cccp=getch();
				switch(c)
				{
				case 'w':
					if(player.pos.Y>0)
					{
						map.matr[player.pos.Y][player.pos.X].pl=NULL;
						player.pos.Y--;
						map.matr[player.pos.Y][player.pos.X].pl=&player;
					}
					break;
				case 's':
					if(player.pos.Y<map.size.Y-1)
					{
						map.matr[player.pos.Y][player.pos.X].pl=NULL;
						player.pos.Y++;
						map.matr[player.pos.Y][player.pos.X].pl=&player;
					}
					break;
				case 'a':
					if(player.pos.X>0)
					{
						map.matr[player.pos.Y][player.pos.X].pl=NULL;
						player.pos.X--;
						map.matr[player.pos.Y][player.pos.X].pl=&player;
					}
					break;
				case 'd':
					if(player.pos.X<map.size.X-1)
					{
						map.matr[player.pos.Y][player.pos.X].pl=NULL;
						player.pos.X++;
						map.matr[player.pos.Y][player.pos.X].pl=&player;
					}
					break;
				case '*':
					rec_add_in_q(&q_stone, &map, player.pos.X, player.pos.Y-2);
					break;
				default:break;
				};
			}
		if(clock()-d>250)
		{
			d = clock();
			move_stone(&q_stone, &map);
			del_from_q_stone(&q_stone, &map);
			system("cls");
			print_map(map, screen_pos, player);
		}
	}
	for(s_stone *cur = q_stone.head; cur; cur=cur->next)
		printf("\n%d %d",cur->pos.X, cur->pos.Y);
	system("pause");
	free(map.characters);
	free(map.colors);
	for(int i=0; i<map.size.Y; i++)
		free(map.matr[i]);
	free(map.matr);
	q_stone_clear(&q_stone);
	return 0;
}
