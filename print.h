#ifndef _PRINT_H
#define _PRINT_H

#define _CRT_SECURE_NO_WARNINGS
#include "declarartions.h"
#include "is_something.h"
#include "utilits.h"
#include <stdio.h>

// ���������� ������, ��������� ������ �� �������� c. ���� is_new_line!=0, �������� ������� �� ����� ������.
void print_line(char c, int size, int is_new_line);

// ���������� ������ �� ������������ ����� �����
int print_player(COORD screen_pos, s_player pl, int i, int j);

// ���������� ������ ����� (�����, ����� ��� ������)
void print_cell(s_map map, int i, int j);

// ��������� ����� �����.
// �� ���� �������� ����� � ���������� ������ ����������� �����
void print_map(s_map map, COORD screen_pos, s_player player/*, s_enemies First*/);

// ������� ������
int great_victory(s_map *map, s_player *player);

// ���������� ����
int game_over();

#endif // _PRINT_H