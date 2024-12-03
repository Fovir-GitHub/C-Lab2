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
    char * remove_name = NULL;

    initializeLinkList(&category_list);
    readCategoryDatafromFile(&category_list);

    removeCategory(&category_list);
    showCategories(&category_list);

    return 0;
}