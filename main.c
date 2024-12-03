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
    const int N = 3;
    LinkList categories;
    char * remove_name = NULL;

    initializeLinkList(&categories);

    for (int i = 0; i < N; i++)
    {
        getString(&remove_name, CATEGORY_NAME_MAX_LENGTH);
        addCategorytoLinkList(&categories, remove_name);
    }

    while (getString(&remove_name, CATEGORY_NAME_MAX_LENGTH))
    {
        removeCategoryfromLinkList(&categories, remove_name);
        putchar('\n');
        showCategoryLinkList(&categories);
    }
    return 0;
}