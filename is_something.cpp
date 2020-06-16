#include "is_something.h"

// ����������� �� ������ �����
int is_on_map(s_map *map, int X, int Y)
{
	if(X<0 || X>=map->size.X)
		return 0;
	if(Y<0 || Y>=map->size.Y)
		return 0;
	return 1;
}

// ������ 1, ���� ������ ������ - �����, 0 - ���� �� �����, ��� ���� �����
int is_grass(s_map *map, int X, int Y)
{
	if(!is_on_map(map, X, Y))
		return 0;
	// ������� ����� 128 �������� ��� ������������� �� ������ 256
	if((map->matr[Y][X].ch+256)%256 == type_p_grass || (map->matr[Y][X].ch+256)%256==type_grass)
		return 1;
	return 0;
}

// �������� �� ������ ������
int is_stone(s_map *map, int X, int Y)
{
	if(!is_on_map(map, X, Y))
		return 0;
	// ������� ����� 128 �������� ��� ������������� �� ������ 256
	if((map->matr[Y][X].ch+256)%256 == type_p_stone || (map->matr[Y][X].ch+256)%256 == type_stone)
		return 1;
	return 0;
}

// ������ - �����
int is_bush(s_map *map, int X, int Y)
{
	if(!is_on_map(map, X, Y))
		return 0;
	// ������� ����� 128 �������� ��� ������������� �� ������ 256
	if((map->matr[Y][X].ch+256)%256 == type_p_bush || (map->matr[Y][X].ch+256)%256 == type_bush)
		return 1;
	return 0;
}

// ������ - �����
int is_wall(s_map *map, int X, int Y)
{
	if(!is_on_map(map, X, Y))
		return 0;
	// ������� ����� 128 �������� ��� ������������� �� ������ 256
	if((map->matr[Y][X].ch+256)%256 == type_p_wall || (map->matr[Y][X].ch+256)%256 == type_wall)
		return 1;
	return 0;
}

// ������ - �����
int is_exit(s_map *map, int X, int Y)
{
	if(!is_on_map(map, X, Y))
		return 0;
	// ������� ����� 128 �������� ��� ������������� �� ������ 256
	if((map->matr[Y][X].ch+256)%256 == type_p_exit || (map->matr[Y][X].ch+256)%256 == type_exit)
		return 1;
	return 0;
}
