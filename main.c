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
    const int N = 3;

    LinkList category_list;
    char * category_name;
    char * item_name;
    char * remove_category_name;

    initializeLinkList(&category_list);

    puts("Enter 3 category name:");
    for (int i = 0; i < N; i++)
        addCategorytoLinkList(
            &category_list,
            getString(&category_name, CATEGORY_NAME_MAX_LENGTH));

    puts("Enter 3 category and item name:");
    for (int i = 0; i < N; i++)
    {
        printf("Please enter category's name: ");
        getString(&category_name, CATEGORY_NAME_MAX_LENGTH);
        printf("Please enter item's name: ");
        getString(&item_name, ITEM_NAME_MAX_LENGTH);

        addItemstoCategory(&category_list, makeItem(category_name, item_name),
                           CREATE_NEW_CATEGORY);
    }

    traverseCategoryLinkList(&category_list, show);

    freeCategoryLinkList(&category_list);
    return 0;
}