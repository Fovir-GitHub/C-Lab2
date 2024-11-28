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
    int height;                 /* the tree node height */
} AVLTreeNode;

typedef AVLTreeNode * AVLTree;

void initializeAVLTree(AVLTree * tree);

/**
 *@brief Construct AVLTreeNode
 *
 * @param item the node's item value
 * @return AVLTreeNode pointer to the new tree node
 */
AVLTreeNode * makeAVLTreeNode(Item item);

/**
 *@brief Get AVL tree node's height.
 *
 * @param node the AVL tree node
 * @return int The height of the node. If the node is empty, it returns -1.
 */
int getAVLTreeNodeHeight(AVLTreeNode * node);

/**
 *@brief Update the height of the AVL tree node.
 *
 * @param node the target node
 */
void updateAVLTreeNodeHeight(AVLTreeNode * node);

/**
 *@brief Get the balance factor of the node
 *
 * @param node the target node
 * @return int If the node is empty, the balance factor will be 0.
 * If the node is not empty, return the left height - right height.
 */
int getBalanceFactor(AVLTreeNode * node);

/**
 *@brief Right rotate to make the tree balanced.
 *
 * @param node the node want to right rotate
 * @return AVLTreeNode* The node after rotate.
 */
AVLTreeNode * rightRotate(AVLTreeNode * node);

/**
 *@brief Lefet rotate to make the tree balanced.
 *
 * @param node the node want to left rotate
 * @return AVLTreeNode* The node after rotate.
 */
AVLTreeNode * leftRotate(AVLTreeNode * node);

/**
 *@brief Rotate the node to keep the tree balanced.
 *
 * @param node the node want to rotate
 * @return AVLTreeNode* The node after rotate.
 */
AVLTreeNode * rotateNode(AVLTreeNode * node);

/**
 *@brief Insert item to AVLTree.
 *
 * @param tree the target tree
 * @param insert_item the item want to add
 */
void insertAVLTreeNode(AVLTree * tree, Item insert_item);

/**
 *@brief Traverse the tree and apply function to every node.
 *
 * @param tree the target tree
 * @param pointer_function the pointer to the function
 */
void traverseAVLTree(AVLTree * tree, void (*pointer_function)(AVLTreeNode *));

/**
 *@brief Show all elements in the AVL tree in alphabetical order.
 *
 * @param tree the target tree
 */
void showAVLTree(AVLTree * tree);

/**
 *@brief Free the memory of the AVL tree.
 *
 * @param tree the AVL tree to free memory space
 */
void freeAVLTree(AVLTree * tree);

/**
 *@brief Remove a item from a AVL tree according item's name.
 *
 * @param tree the target tree
 * @param item_name the item's name
 */
void removeItemfromAVLTree(AVLTree * tree, char * item_name);

#endif // !_ITEM_H_