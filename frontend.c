#include "frontend.h"
#include <stdio.h>
#include <string.h>

int centerMenuTitle(char * title)
{
    return (MENU_WIDTH - strlen(title)) / 2;
}

void printMenuFrame(char frame_character)
{
    for (int i = 0; i < MENU_WIDTH; i++)
        putchar(frame_character);
    putchar('\n');
}

void showMainMenu()
{
    char * title = "Main Menu";
}