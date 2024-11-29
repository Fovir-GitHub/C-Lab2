#include "frontend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int calculateCenterStringSpace(char * str)
{
    // calculate center the string need how many space before it
    return abs(MENU_WIDTH - strlen(str)) / 2;
}

void printStringinCenter(char * str)
{
    printf("%*s%s\n", calculateCenterStringSpace(str) - 1, "", str);
    return;
}

void printMenuTitle(char * title)
{
    printMenuFrame('=');
    printStringinCenter(title);
    printMenuFrame('=');

    return;
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
    printMenuTitle(title);
}