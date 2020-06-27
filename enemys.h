#ifndef _ENEMYS_H
#define _ENEMYS_H

#include <stdio.h>
#include "declarations.h"
#include "utilits.h"

//создание одного врага
s_enemy* create_enemy(s_enemy* info);

//добавление врага
int add_in_head(s_enemy* new_enemy, s_enemy** first_enemy);

//создание списка врагов
int create_enemys(s_txt_name txt_name, s_enemy** first_enemy);

#endif //_ENEMYS_H