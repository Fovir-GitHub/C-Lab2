#include "frontend.h"
#include "my_utility.h"
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
    printMenuFrame('-');

    return;
}

void printMenuFrame(char frame_character)
{
    for (int i = 0; i < MENU_WIDTH; i++)
        putchar(frame_character);
    putchar('\n');
}

void printOptions(char * options[], int options_number)
{
#define ORDER_NUMBER_WIDTH 4

    int max_odd_string_length = 0, max_even_string_length = 0,
        max_string_length = 0;

    // get max_odd_string_length and max_even_string_length
    for (int i = 0; i < options_number; i++)
        if ((i + 1) & 1) /* odd number */
            max_odd_string_length =
                MAX(max_odd_string_length, strlen(options[i]));
        else
            max_even_string_length =
                MAX(max_even_string_length, strlen(options[i]));

    max_odd_string_length +=
        ORDER_NUMBER_WIDTH; /* get the max width of odd string */
    max_even_string_length +=
        ORDER_NUMBER_WIDTH; /* get the max width of even string */
    max_string_length =
        MAX(max_odd_string_length,
            max_even_string_length); /* get the max width of all strings */

    char * temp_string =
        (char *) malloc((max_string_length + 1) * sizeof(char));

    // get the space width between two options
    int middle_space_width =
        MENU_WIDTH - max_odd_string_length - max_even_string_length;

    for (int i = 0; i < options_number; i++)
    {
        // the order number should be q if the option is Quit
        sprintf(temp_string, "[%c] %s",
                (strcmp("Quit", options[i]) == 0) ? 'q' : (i + '1'),
                options[i]);

        if ((i + 1) & 1) /* odd order string */
            printf("%-*s%*c", max_odd_string_length, temp_string,
                   middle_space_width,
                   '\0'); /* align to left and print the middle space */
        else
            printf("%-*s\n", max_even_string_length,
                   temp_string); /* align to left and be sure that the longest
                                    string reaches the right side */
    }

    if (options_number & 1) /* if the number of options is odd, */
        putchar('\n');      /* output line breaker */

    free(temp_string);

    return;
}

void printMenuFooter(char * footer)
{
    printMenuFrame('-');
    printStringinCenter(footer);
    printMenuFrame('=');
}

void showMainMenu()
{
    printMenuTitle("Main Menu");

    static char * options[MAIN_MENU_OPTIONS_NUMBER] = {
        "Show Categories",
        "Add Category",
        "Remove Category",
        "Show All Items",
        "Show Items By Category",
        "Add Item",
        "Remove Item",
        "Edit Item",
        "Quit",
    };

    printOptions(options, MAIN_MENU_OPTIONS_NUMBER);
    printMenuFooter("Footer");
}