#include "category.h"
#include "item.h"
#include "my_utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const int N = 3;
    const int LENGTH = 1024;

    AVLTree items_tree;
    char * remove_item_name;
    initializeAVLTree(&items_tree);

    for (int i = 0; i < N; i++)
    {
        Item temp_item;
        getString(&temp_item.name, LENGTH);

        insertAVLTreeNode(&items_tree, temp_item);
    }

    puts("Remove:");
    for (int i = 0; i < N; i++)
    {
        getString(&remove_item_name, LENGTH);
        removeItemfromAVLTree(&items_tree, remove_item_name);
        showAVLTree(&items_tree);
        putchar('\n');
    }

    freeAVLTree(&items_tree);
    puts("free success");
    return 0;
}