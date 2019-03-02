#pragma once
#include <stdbool.h>

int menu();
void menu_divider();
void menu_row();
void menu_print(char*, bool);
void menu_print_2(char*, char*, bool);
char* string_con(char*, char*);