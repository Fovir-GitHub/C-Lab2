#include "item.h"
#include "my_utility.h"
#include <stdio.h>
#include <stdlib.h>
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
 *@brief Recrusive apply function to all nodes in the AVL tree.
 *
 * @param node the node
 * @param pointer_function the pointer to function
 */
static void traverseAVLTreeHelper(AVLTreeNode * node,
                                  void (*pointer_function)(AVLTreeNode *));

/**
 *@brief Help to free the AVL tree.
 *
 * @param node the target node
 */
static void freeAVLTreeHelper(AVLTreeNode * node);

/**
 *@brief Help to show the AVL tree node
 *
 * @param node the target node
 */
static void showAVLTreeHelper(AVLTreeNode * node);

DateInformation getToday()
{
    DateInformation today;
    time_t current_time;
    struct tm * local_time;

    time(&current_time);                   /* get current time */
    local_time = localtime(&current_time); /* get local time */

    today.year = local_time->tm_year + 1900; /* get today's year */
    today.month = local_time->tm_mon + 1;    /* get today's month */
    today.day = local_time->tm_mday;         /*get today's day */

    return today;
}

void initializeAVLTree(AVLTree * tree)
{
    *tree = NULL;
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

    // update nodes' height, the height of grand_child doesn't change
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

    // update nodes' height, the height of grand_child doesn't change
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

void traverseAVLTree(AVLTree * tree, void (*pointer_function)(AVLTreeNode *))
{
    traverseAVLTreeHelper(*tree, pointer_function);

    return;
}

void showAVLTree(AVLTree * tree)
{
    traverseAVLTree(tree, showAVLTreeHelper);

    return;
}

void freeAVLTree(AVLTree * tree)
{
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
    else /* duplicated node, no action to do */
        return node;

    updateAVLTreeNodeHeight(node); /* update the height of the new node */
    node = rotateNode(node);       /* rotate the node, make the tree balanced */

    return node;
}

void traverseAVLTreeHelper(AVLTreeNode * node,
                           void (*pointer_function)(AVLTreeNode *))
{
    if (node->left)
        traverseAVLTreeHelper(node->left, pointer_function);
    pointer_function(node);
    if (node->right)
        traverseAVLTreeHelper(node->right, pointer_function);

    return;
}

void freeAVLTreeHelper(AVLTreeNode * node)
{
    free(node->item.name); /* free item's name memory space */
    free(node);            /* free the node memory space */

    return;
}

void showAVLTreeHelper(AVLTreeNode * node)
{
    puts(node->item.name);

    return;
}
