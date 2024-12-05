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
    showCategories(&category_list);

    return 0;
}