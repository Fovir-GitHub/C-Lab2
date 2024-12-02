#include "category.h"
#include "file_operations.h"
#include "frontend.h"
#include "item.h"
#include "my_utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char user_choice = 0;

    showMainMenu();
    while (!(user_choice = getChoicefromUser("12345678q")))
    {
        printf("You entered an invalid choice! Press any key to continue...");
        getchar();
        clearScreen();
        showMainMenu();
    }
    printf("choice: %c\n", user_choice);
    return 0;
}