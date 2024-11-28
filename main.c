#include "category.h"
#include "item.h"
#include "my_utility.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const int N = 3;
    const int LENGTH = 1024;

    AVLTree items_tree;
    initializeAVLTree(&items_tree);

    for (int i = 0; i < N; i++)
    {
        Item temp_item;
        getString(&temp_item.name, LENGTH);

        insertAVLTreeNode(&items_tree, temp_item);
    }

    showAVLTree(&items_tree);
    freeAVLTree(&items_tree);
    puts("Success free");

    return 0;
}