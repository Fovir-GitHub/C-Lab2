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
    LinkList category_list;
    initializeLinkList(&category_list); /* initialize the link list */

    // read data from files
    readCategoryDatafromFile(&category_list);
    readItemsDatafromFile(&category_list);

    int user_choice = 0; /* stores user's choice */

    // continuously get user's choice until QUIT
    while ((user_choice = getMainMenuChoicefromUser()) != QUIT)
    {
        switch (user_choice)
        {
        case SHOW_ALL_CATEGORIES:
            showCategories(&category_list);
            break;
        case ADD_CATEGORY:
            addCategory(&category_list);
            break;
        case REMOVE_CATEGORY:
            removeCategory(&category_list);
            break;
        case SHOW_ALL_ITEMS:
            showItems(&category_list);
            break;
        case ADD_ITEM:
            addItem(&category_list);
            break;
        case REMOVE_ITEM:
            removeItem(&category_list);
            break;
        case EDIT_ITEM:
            editItem(&category_list);
            break;
        default:
            printf("Please enter the right option! Press Enter to continue...");
            eatLine();
        }
    }

    // before quit the program
    puts("Bye!");
    printf("Press Enter to exit...");
    eatLine();

    // output data to file
    outputCategoryDatatoFile(&category_list);
    outputItemsDatatoFile(&category_list);

    clearScreen();

    return 0;
}