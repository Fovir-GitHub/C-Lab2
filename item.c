#include "item.h"
#include "my_utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 *@brief Recrusive add item to a AVL tree.
 *
 * @param insert_item the item to insert
 * @return AVLTreeNode* the node after inserting
 */
static AVLTreeNode * insertAVLTreeNodeHelper(AVLTreeNode * node,
                                             Item insert_item);

/**
 *@brief Help to remove item from a AVL tree recrusively.
 *
 * @param item_name the item's name
 * @return AVLTreeNode* the node after removing item
 */
static AVLTreeNode * removeItemfromAVLTreeHelper(AVLTreeNode * node,
                                                 char * item_name);

/**
 *@brief Help to free the AVL tree recrusively.
 *
 * @param node the target node
 */
static void freeAVLTreeHelper(AVLTreeNode * node);

/**
 *@brief Help to get the size of an AVL tree.
 *
 * @param node the current node
 * @return int the size of the sub-tree
 */
static int getAVLTreeSizeHelper(AVLTreeNode * node);

DateInformation makeDateInformation(int y, int m, int d)
{
    DateInformation result = {y, m, d};
    return result;
}

DateInformation getToday()
{
    time_t current_time;
    const struct tm * local_time;

    time(&current_time);                   /* get current time */
    local_time = localtime(&current_time); /* get local time */

    return makeDateInformation(
        local_time->tm_year + 1900, /* get today's year */
        local_time->tm_mon + 1,     /* get today's month */
        local_time->tm_mday         /*get today's day */
    );
}

void showDateInformation(const DateInformation * date)
{
    // format output to YYYY-MM-DD with 0 prefix
    printf("%04d-%02d-%02d\n", date->year, date->month, date->day);
    return;
}

bool validDate(DateInformation date)
{
    // non-positive number
    if (date.day <= 0 || date.month <= 0 || date.year <= 0)
        return false;

    bool leap_year = !(date.year % 4); /* judge the year is leap year */

    switch (date.month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (date.day > 31)
            return false;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (date.day > 30)
            return false;
        break;
    case 2:
        if (leap_year && date.day > 29)
            return false;
        if (!leap_year && date.day > 28)
            return false;
    default:
        return false;
    }

    return true;
}

Item makeItem(const char * category, const char * name, double price,
              DateInformation produce, DateInformation due)
{
    Item result;

    // assign static variables
    result.price = price;
    result.produce_date = produce;
    result.due_date = due;

    // assign dynamic variables
    result.category = staticString2dynamicString(category);
    result.name = staticString2dynamicString(name);

    return result;
}

void showItemInformation(Item * item)
{
    static const int ITEM_INFORMATION_WIDTH = 16;

    int ITEM_FRAME_WIDTH =
        MAX(MAX(strlen(item->category), strlen(item->name)), 12) +
        ITEM_INFORMATION_WIDTH + 4; /* get the frame width */

    puts("The item's information:\n");

    // print frame
    for (int i = 0; i < ITEM_FRAME_WIDTH; i++) putchar('*');
    putchar('\n');

    printf("%-*s: %s\n%-*s: %s\n", ITEM_INFORMATION_WIDTH, "Category",
           item->category, ITEM_INFORMATION_WIDTH, "Item Name", item->name);
    printf("%-*s: %.2lf\n", ITEM_INFORMATION_WIDTH, "Price", item->price);
    printf("%-*s: ", ITEM_INFORMATION_WIDTH, "Produce Date");
    showDateInformation(&item->produce_date);
    printf("%-*s: ", ITEM_INFORMATION_WIDTH, "Expiration Date");
    showDateInformation(&item->due_date);

    // print frame
    for (int i = 0; i < ITEM_FRAME_WIDTH; i++) putchar('*');
    puts("\n");

    return;
}

void initializeAVLTree(AVLTree * tree)
{
    *tree = NULL;
}

bool emptyAVLTree(AVLTree * tree)
{
    return *tree == NULL;
}

AVLTreeNode * makeAVLTreeNode(Item item)
{
    AVLTreeNode * new_node = (AVLTreeNode *) malloc(sizeof(AVLTreeNode));
    if (!new_node) /* can't allocate memory */
    {
        fprintf(stderr, "Can't allocate memory for new_node in %s()\n",
                __func__);  /* print error message */
        exit(EXIT_FAILURE); /* terminate program */
    }

    new_node->item = item;
    new_node->left = new_node->right = NULL;
    new_node->height = 0;

    return new_node;
}

int getAVLTreeNodeHeight(AVLTreeNode * node)
{
    return node ? node->height : -1;
}

void updateAVLTreeNodeHeight(AVLTreeNode * node)
{
    // get the heights of child nodes
    int left_height = getAVLTreeNodeHeight(node->left),
        right_height = getAVLTreeNodeHeight(node->right);

    // update the height to the bigger height + 1
    node->height =
        left_height > right_height ? left_height + 1 : right_height + 1;

    return;
}

int getBalanceFactor(AVLTreeNode * node)
{
    return node ? getAVLTreeNodeHeight(node->left) -
                      getAVLTreeNodeHeight(node->right)
                : 0;
}

AVLTreeNode * rightRotate(AVLTreeNode * node)
{
    AVLTreeNode * child = node->left;
    AVLTreeNode * grand_child = child->right;

    child->right = node;      /* move the node to down */
    node->left = grand_child; /* move the grand child node */

    // update nodes' height, the height of grand_child doesn't
    // change
    updateAVLTreeNodeHeight(node);
    updateAVLTreeNodeHeight(child);

    // the child node become the original node, return child node
    return child;
}

AVLTreeNode * leftRotate(AVLTreeNode * node)
{
    AVLTreeNode * child = node->right;
    AVLTreeNode * grand_child = child->left;

    child->left = node;        /* move the node to down */
    node->right = grand_child; /* append the grand child node to the node */

    // update nodes' height, the height of grand_child doesn't
    // change
    updateAVLTreeNodeHeight(node);
    updateAVLTreeNodeHeight(child);

    // the child node become the original node, return child node
    return child;
}

AVLTreeNode * rotateNode(AVLTreeNode * node)
{
    /**
     * Get the node's balance factor.
     * If the balance factor > 1, the left is not balanced.
     * Then get the left node's balance factor.
     * If the left node's balance factor >= 0,
     * the original node need to right rotate.
     * If the left node's balance factor < 0,
     * the left node need to left rotate at first,
     * and then right rotate the original node.
     *
     * If the balance factor < -1, the right is not balanced.
     * Then get the right node's balance factor.
     * If the right node's balance factor <=0,
     * the original node only need to left rotate.
     * If the right node's balance factor > 0,
     * the right node need to right rotate at first,
     * and then left rotate the original node.
     */

    // get the original node's balance factor
    int node_balance_factor = getBalanceFactor(node);

    if (node_balance_factor > 1) /* the left node is not balanced */
    {
        if (getBalanceFactor(node->left) >= 0) /* no right grand child node */
            return rightRotate(node);
        else /* have right grand child node */
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }

    if (node_balance_factor < -1) /* the right node is not balanced */
    {
        if (getBalanceFactor(node->right) <= 0) /* no left grand child node */
            return leftRotate(node);
        else /* have right grand child node */
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }

    // This node is balanced, no need to rotate.
    return node;
}

void insertAVLTreeNode(AVLTree * tree, Item insert_item)
{
    *tree = insertAVLTreeNodeHelper(*tree, insert_item);
    return;
}

void freeAVLTree(const AVLTree * tree)
{
    freeAVLTreeHelper(*tree);
    return;
}

void removeItemfromAVLTree(AVLTree * tree, char * item_name)
{
    *tree = removeItemfromAVLTreeHelper(*tree, item_name);

    return;
}

int getAVLTreeSize(AVLTree * tree)
{
    return getAVLTreeSizeHelper(*tree);
}

AVLTreeNode * insertAVLTreeNodeHelper(AVLTreeNode * node, Item insert_item)
{
    if (node == NULL) /* insert the node  */
        return makeAVLTreeNode(insert_item);

    // compare two items' name
    int compare_result = compare2Strings(insert_item.name, node->item.name);

    if (compare_result == -1) /* insert to left */
        node->left = insertAVLTreeNodeHelper(node->left, insert_item);
    else if (compare_result == 1) /* insert to right */
        node->right = insertAVLTreeNodeHelper(node->right, insert_item);
    else /* duplicated node */
    {
        node->item = insert_item; /* replace original item */
        return node;
    }

    updateAVLTreeNodeHeight(node); /* update the height of the new node */
    node = rotateNode(node);       /* rotate the node, make the tree balanced */

    return node;
}

AVLTreeNode * removeItemfromAVLTreeHelper(AVLTreeNode * node, char * item_name)
{
    AVLTreeNode * child;

    // the node is empty
    if (node == NULL)
        return NULL;

    int compare_result = compare2Strings(item_name, node->item.name);

    if (compare_result == -1) /* find the left branch */
        node->left = removeItemfromAVLTreeHelper(node->left, item_name);
    else if (compare_result == 1) /* find the right branch */
        node->right = removeItemfromAVLTreeHelper(node->right, item_name);
    else /* match the item name */
    {
        if (node->left == NULL || node->right == NULL)
        {
            child = node->right ? node->right : node->left;

            if (!child)       /* node has no child node */
                return NULL;  /* return NULL */
            else              /* node has one child node */
                node = child; /* replace node with child */
        }
        else /* node has two child node */
        {
            AVLTreeNode * temp = node->right; /* get the right child node */
            while (temp->left)                /* find the minmum item's name */
                temp = temp->left;

            Item temp_item = temp->item;
            /* remove the node with temp_item */
            node->right =
                removeItemfromAVLTreeHelper(node->right, temp_item.name);
            node->item = temp_item; /* replace the node's item with
                                       the temp_item */
        }
    }

    updateAVLTreeNodeHeight(node); /* update node's height */
    node = rotateNode(node);       /* rotate node to make the tree balance */

    return node;
}

void freeAVLTreeHelper(AVLTreeNode * node)
{
    if (!node) /* if the node is NULL (empty tree) */
        return;

    if (node->left)
        freeAVLTreeHelper(node->left);
    if (node->right)
        freeAVLTreeHelper(node->right);

    free(node->item.name); /* free item's name memory space */
    free(node);            /* free the node memory space */

    return;
}

int getAVLTreeSizeHelper(AVLTreeNode * node)
{
    if (!node) /* the node is NULL */
        return 0;

    return getAVLTreeSizeHelper(node->left) +
           getAVLTreeSizeHelper(node->right) + 1;
}