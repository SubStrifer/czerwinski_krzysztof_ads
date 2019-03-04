#pragma once
#include <stdbool.h>
#include "..\include\submenu.h"

void menu();
void menu_draw(submenu_t*);
void menu_divider();
void menu_row();
void menu_print(char*, bool);
void menu_print_2(char*, char*, bool);
void menu_wait();
char* string_con(char*, char*);