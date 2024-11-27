#ifndef _ITEM_H_
#define _ITEM_H_

/**
 *@brief store the date information like
 * produce date or the due date of items
 */
typedef struct
{
    int year;  /* year, format #### */
    int month; /* month, format ## */
    int day;   /* day, format ## */
} DateInformation;

/**
 *@brief Get the today's DateInformation
 *
 * @return today's date
 */
DateInformation getToday();

/**
 *@brief store the information about items
 */
typedef struct
{
    char * category; /* the category of the item */
    char * name;     /* item's name */
    double price;    /* item's price, format *.## */
    DateInformation
        produce_date; /* the produce date of the item, default set to today */
    DateInformation
        due_date; /* the due date of the item, default set to empty */
} Item;

/**
 *@brief AVL tree's node. The left node is alphabetically less than this
and the right node is alphabetically greater than this.
 */
typedef struct AVLTreeNode
{
    struct AVLTreeNode * left;  /* pointer to the left child node */
    struct AVLTreeNode * right; /* pointer to the right child node */
    Item item;                  /* item detail */
} AVLTreeNode;

typedef AVLTreeNode * AVLTree;

#endif // !_ITEM_H_