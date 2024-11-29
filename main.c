#include "category.h"
#include "file_operations.h"
#include "item.h"
#include "my_utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void show(LinkListNode * node)
{
    puts(node->category_item.category_name);
    showAVLTree(&node->category_item.item_tree);

    return;
}

int main(void)
{
    LinkList category_list;

    initializeLinkList(&category_list);
    readCategoryDatafromFile(&category_list);
    readItemsDatafromFile(&category_list);

    traverseCategoryLinkList(&category_list, show);
}