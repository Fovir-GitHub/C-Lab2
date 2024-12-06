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
    initializeLinkList(&category_list);

    readCategoryDatafromFile(&category_list);

    int user_choice = 0;

    while ((user_choice = getMainMenuChoicefromUser()) != QUIT)
    {
        switch (user_choice)
        {
        case SHOW_ALL_CATEGORIES:
            showCategories(&category_list);
            break;
        case REMOVE_CATEGORY:
            removeCategory(&category_list);
            break;
        default:
            printf("%d\n", user_choice);
        }
        eatLine();
    }

    puts("Bye");

    return 0;
}