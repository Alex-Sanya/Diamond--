//
//    !!!��������!!!
// ��� ������ � ���������� ���������� ������� ������ ������: ������ = 100, ������ = 75.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <Windows.h>
#include <time.h>

#include "declarations.h"
#include "is_something.h"
#include "moving.h"
#include "preparation.h"
#include "print.h"
#include "stones_and_diamonds.h"
#include "utilits.h"

// ��������� �������� ��� �������, �� ������� ����� ����� - ����� �����, ������� ����...
int player_on_cell(s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone)
{
	if(is_exit(map, player->pos.X, player->pos.Y))
		return -1;
	if(is_checkpoint(map, player->pos.X, player->pos.Y))
		if(!save_on_checkpoint(map, player, q_stone, save_map, save_player, save_q_stone))
			return -1;
	if(is_bush(map, player->pos.X, player->pos.Y))
		map->matr[player->pos.Y][player->pos.X].ch = type_p_grass;
	player_get_diamond(player, map);
	del_1_stone(q_stone, stone_in_q(q_stone, player->pos.X, player->pos.Y));
	return 1;
}

// ������� �������
int press_bottom(s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone)
{
	char bottom = 0; // ������� ������
	if(_kbhit()) //���� ������ �������
	{
		if((bottom =_getch())==ESC) // ������� �������. ��������� �� ��������� �����
			return 1;
		special_bottom(&bottom); // ���� ������ �������, ��������� � � �����������
		while(_kbhit()) // ���� �� �������� ������, ��������� ��� ���� �������
			_getch();
		command(bottom, map, player, q_stone, save_map, save_player, save_q_stone);	
	}
	if(player->lives<0)
		return 1;
	if(player_on_cell(map, player, q_stone, save_map, save_player, save_q_stone)==-1) // �����
		return 1;
	if(!add_in_q_around_player(map, player, q_stone))
		return 1; // �������� ������ - �������
	return 0;
}

// ������� ����. ���� �����������, ����� ����� �������� �� ������ ������ ��� ����� ESC
int game_process(s_map *map, s_player *player, COORD *screen_pos, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone)
{
	int now_time = clock(); // ������� �����
	while(1)
	{
		// ��������� ������� �������
		if(press_bottom(map, player, q_stone, save_map, save_player, save_q_stone)) // ���� ����� �� ������ - ����� �� �������
			return 1;
		if(clock()-now_time>250) // ���������� ��� � �������� �������
		{
			screen_position(screen_pos, player, map); // �������� ������� ���������� ������
			now_time = clock(); // �������� �����
			move_stone(q_stone, map, player); // ����������� �����
			del_from_q_stone(q_stone, map); // ������� �����, �� ����������� � �������
			system("cls"); // �������� �����
			print_map(map, screen_pos, player); // ���������� �����
			printf("\nDiamonds = %d / %d\nLives = %d", player->diamonds, map->diamonds, player->lives); // ����������
		}
	}
	return 0; // ������ ESC - ��������� ���������� ���������
}

int main()
{
	printf("Diamond-- by Alex, Evgen, POMAH.\n");
	system("pause");
	// ���������� ����������, ����������� � ����
	int level = 0, count_diamonds = 0;
	COORD screen_pos = {0,0};
	DWORD dw = 0;
	s_map map = {0,}, save_map = {0,};
	s_all_colors all_colors;
	s_txt_name txt_name = {0,0,0};
	s_player player = { 0,0,{0,0},0,0 }, save_player = { 0,0,{0,0},0,0};
	s_q_stone q_stone = {0,0}, save_q_stone = {0,0};
	// ���������� � ���� - �������� ������, ����� (?) ������
	if(!preparation(&level, &map, &all_colors, &player, &save_map))
		return 0;
	// ������� ���������� �����
	game_process(&map, &player, &screen_pos, &q_stone, &save_map, &save_player, &save_q_stone);
	// ������� ��������� � ���������� ����
	if(!great_victory(&map, &player))
		game_over();
	// ������������ �������������� ������
	free_all(&map, &save_map, &q_stone, &save_q_stone);
	return 0;
}
