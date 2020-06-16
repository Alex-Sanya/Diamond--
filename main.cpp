//
//    !!!��������!!!
// ��� ������ � ���������� ���������� ������� ������ ������: ������ = 100, ������ = 75.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <Windows.h>
#include <time.h>

#include "declarartions.h"
#include "is_something.h"
#include "moving.h"
#include "preparation.h"
#include "print.h"
#include "stones_and_diamonds.h"
#include "utilits.h"

// ��������� �������� ��� �������, �� ������� ����� ����� - ����� �����, ������� ����...
int player_on_cell(s_map *map, s_player *player, s_q_stone *q_stone)
{
	if(is_exit(map, player->pos.X, player->pos.Y))
		return -1;
	if(is_bush(map, player->pos.X, player->pos.Y))
		map->matr[player->pos.Y][player->pos.X].ch = type_p_grass;
	player_get_diamond(player, map);
	del_1_stone(q_stone, stone_in_q(q_stone, player->pos.X, player->pos.Y));
	return 1;
}


// ������� �������
int press_bottom(s_map *map, s_player *player, s_q_stone *q_stone)
{
	char bottom = 0; // ������� ������
	if(_kbhit()) //���� ������ �������
	{
		if((bottom =_getch())==ESC) // ������� �������. ��������� �� ��������� �����
			return 1;
		pointer2direction(&bottom); // ���� ������ �������, ��������� � � �����������
		while(_kbhit()) // ���� �� �������� ������, ��������� ��� ���� �������
			_getch();
		command(bottom, map, player, q_stone);	
	}
	if(player_on_cell(map, player, q_stone)==-1)
		return 1;
	add_in_q_around_player(map, player, q_stone);
	return 0;
}

// ������� ����. ���� �����������, ����� ����� �������� �� ������ ������ ��� ����� ESC
int game_process(s_map *map, s_player *player, COORD *screen_pos, s_q_stone *q_stone)
{
	int now_time = clock(); // ������� �����
	while(1)
	{
		// ��������� ������� �������
		if(press_bottom(map, player, q_stone)) // ���� ����� �� ������ - ����� �� �������
			return 1;
		if(clock()-now_time>250) // ���������� ��� � �������� �������
		{
			screen_position(screen_pos, player, map); // �������� ������� ���������� ������
			now_time = clock(); // �������� �����
			move_stone(q_stone, map, player); // ����������� �����
			del_from_q_stone(q_stone, map); // ������� �����, �� ����������� � �������
			system("cls"); // �������� �����
			print_map(*map, *screen_pos, *player); // ���������� �����
			printf("Player->diamonds = %d\n Need = %d", player->diamonds, map->diamonds); // ����������
		}
	}
	return 0; // ������ ESC - ��������� ���������� ���������
}

int main()
{
	printf("Diamond-- by Alex, Evgen, POMAH.\n");
	system("pause");
	// ���������� ����������, ����������� � ����
	int level=0, count_diamonds=0;
	COORD screen_pos={0,0};
	DWORD dw=0;
	s_map map={0,};
	s_all_colors all_colors;
	s_txt_name txt_name={0,0,0};
	s_player player = { {0,0},0,0,0,0 };
	s_q_stone q_stone = {0,0};
	// ���������� � ���� - �������� ������, ����� (?) ������
	if(!preparation(&level, &map, &all_colors, &player))
		return 0;
	// ������� ���������� �����
	game_process(&map, &player, &screen_pos, &q_stone);
	// ������� ��������� � ���������� ����
	if(!great_victory(&map, &player))
		game_over();
	// ������������ �������������� ������
	q_stone_clear(&q_stone);
	free(map.characters);
	free(map.colors);
	for(int i=0; i<map.size.Y; i++)
		free(map.matr[i]);
	free(map.matr);
	q_stone_clear(&q_stone);
	return 0;
}
