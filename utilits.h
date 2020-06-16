#ifndef _UTILITS_H
#define _UTILITS_H

#include "declarartions.h"
#include "moving.h"
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

// ����� ��������� �� ���� ������. ����� ���������� 0
int err(int type);

// ������� �������� ����� �� ���� ����� � ��� ������
int type_char_to_print(char *c);

// ������� ���� �������� ����� �� ���� ����� � ��� ������
int map_characters_to_print(s_map *map);

// ������� ������ ����� � ������� �������� �����
int str2color(char *str);

// ������� ������ ����� � ������� �������� �����, ������ ������ �� �����
int str2color_from_file(unsigned short *parametr, FILE **fin);

// ������ �����
int reverse_int(int n);

// ������� ����� � ������
void int2str(int n, char str[], int lenght);

// ������� ��������� � �����������
int pointer2direction(char *bottom);

// ��������� ������� �� ������� �������
void command(char bottom, s_map *map, s_player *player, s_q_stone *q_stone);

// �������� s_cell - ������� ��������� m �� n
int create_s_cell_matrix(s_cell ***matrix, int m, int n);


// ��������� ������ ������������ ������
void screen_position(COORD *screen_pos, s_player *player, s_map *map);

#endif //_UTILITS_H