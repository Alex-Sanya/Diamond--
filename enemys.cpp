#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "enemys.h"
#include "declarations.h"
#include "utilits.h"

//создание одного врага
s_enemy* create_enemy(s_enemy* info)
{
	s_enemy* new_enemy = (s_enemy*)malloc(sizeof(s_enemy));
	if (!new_enemy)
		return (s_enemy*)err(RAM_IS_OVER);
	new_enemy->ch = info->ch;
	new_enemy->color = info->color;
	new_enemy->pos = info->pos;
	new_enemy->next = NULL;
	return new_enemy;
}

//добавление врага
int add_in_head(s_enemy* new_enemy, s_enemy **first_enemy)
{
	if(new_enemy==0)
		return 0;
	if (!first_enemy)
	{
		*first_enemy = new_enemy;
		return 1;
	}
	new_enemy->next = *first_enemy;
	*first_enemy = new_enemy;
	return 1;
}

//создание списка врагов
int create_enemys(s_txt_name txt_name, s_enemy** first_enemy)
{
	s_enemy temp = {0,};
	FILE* fenemy = fopen(txt_name.enemy, "r");
	if (!fenemy)
		return err(FILE_NOT_FOUND);
	while (1)
	{
		if (!fscanf(fenemy, "%d", &(temp.pos.X)) || !fscanf(fenemy, "%d", &(temp.pos.Y)))
		{
			fclose(fenemy);
			return err(NO_ENOUGH_DATA);
		}
		temp.ch=fgetc(fenemy);
		if (!fscanf(fenemy, "%d", &(temp.ch)))
		{
			fclose(fenemy);
			return err(NO_ENOUGH_DATA);
		}
		if (!fscanf(fenemy, "%d", &(temp.d)))
		{
			fclose(fenemy);
			return err(NO_ENOUGH_DATA);
		}
		if (fgetc(fenemy) == '.')
			break;
		fgetc(fenemy);
		if (!(add_in_head(create_enemy(&temp), first_enemy)))
		{
			fclose(fenemy);
			return 0;
		}
	}
	fclose(fenemy);
	return 1;
}