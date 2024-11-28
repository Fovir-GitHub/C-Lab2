#include "category.h"
#include "file_operations.h"
#include "item.h"
#include "my_utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    LinkList category_link_list;

    initializeLinkList(&category_link_list);
    readCategoryDatafromFile(&category_link_list);

    showCategoryLinkList(&category_link_list);

    return 0;
}