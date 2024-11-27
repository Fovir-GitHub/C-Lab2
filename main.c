#include "category.h"
#include "item.h"
#include "my_utility.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const int LENGTH = 512;

    LinkList category_name;
    char remove_category_name[LENGTH];
    initializeLinkList(&category_name);

    for (int i = 0; i < 5; i++)
    {
        Category temp;
        getString(&temp.category_name, LENGTH);
        int status_code = addCategorytoLinkList(&category_name, temp);

        // switch (status_code)
        // {
        // case SUCCESS_ADD:
        // 	puts("Success add");
        // 	break;
        // case DUPLICATED_NODE:
        // 	puts("Duplicated node");
        // 	break;
        // }
    }

    showCategoryLinkList(&category_name);
    while (scanf("%s", remove_category_name) == 1)
    {
        int status =
            removeCategoryfromLinkList(&category_name, remove_category_name);
        switch (status)
        {
        case SUCCESS_REMOVE:
            puts("Remove successfully!");
            break;
        case FAILED_REMOVE_EMPTY_LINK_LIST:
            puts("The link list is empty!");
            break;
        case NODE_DOES_NOT_EXIST:
            puts("Category doesn't exist!");
            break;
        }
        showCategoryLinkList(&category_name);
    }

    freeCategoryLinkList(&category_name);
    puts("Free successfully!");

    return 0;
}