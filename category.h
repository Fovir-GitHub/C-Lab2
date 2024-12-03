#ifndef _CATEGORY_H_
#define _CATEGORY_H_

#include "item.h"
#include <stdbool.h>
#include <stdlib.h>
#define CATEGORY_NAME_MAX_LENGTH 128

/**
 *@brief The status code of link list's operation.
 */
enum LINK_LIST_STATUS_CODES {
    SUCCESS_ADD,                   /* add new node successfully */
    SUCCESS_REMOVE,                /* remove node successfully */
    FAILED_ALLOCATE_MEMORY,        /* can't allocate memory for new node */
    FAILED_REMOVE_EMPTY_LINK_LIST, /* remove node in a empty link list */
    NODE_DOES_NOT_EXIST,           /* try to remove a non-exist node */
    DUPLICATED_NODE                /* the node item (category) exists */
};

/**
 *@brief Store the information of categories.
 */
typedef struct
{
    char * category_name;
    AVLTree item_tree; /* item's tree */
} Category;

/**
 *@brief Construct Category with its name.
 *
 * @param name category's name
 * @return Category the object after constructing
 */
Category makeCategory(char * name);

/**
 *@brief Use double way link list to store categories,
storing in alphabetical order.
 */
typedef struct LinkListNode
{
    Category category_item;         /* the category */
    struct LinkListNode * previous; /* pointer to the previous node */
    struct LinkListNode * next;     /* pointer to the next node */
} LinkListNode;

/**
 *@brief Used to sort the link list in alphabetical order.
 *
 * @param first the first link list node
 * @param second  the other one link list node
 * @return int  if first < last, return -1
 *  if first > last, return 1
 *  otherwise, return 0
 */
int compareCategoryName(LinkListNode * first, LinkListNode * second);

typedef LinkListNode * LinkList; /* define the LinkList */

/**
 * @brief Initialize a link list.
 * @param list the link list will be initialized
 */
void initializeLinkList(LinkList * list);

/**
 *@brief Judge whether the link list is empty.
 *
 * @param list the link list to check
 * @return true the link list is emtpy
 * @return false the link list is not empty
 */
bool emptyLinkList(LinkList * list);

/**
 *@brief Add new category to the link list.
 *
 * @param list the target link list
 * @param category_name the category's name
 * @return if added successfully, it returns SUCCESS_ADD.
 * if can't allocate memory, it returns FAILED_ALLOCATE_MEMORY
 * if there has existed a same category, it returns DUPLICATED_NODE
 */
int addCategorytoLinkList(LinkList * list, char * category_name);

/**
 *@brief Remove a categoy from the link list.
 *
 * @param list the target link list
 * @param category_name the category's name that want to remove
 * @return
 * if remove successfully, it returns SUCCESS_REMOVE
 * if can't allocate memory, it returns FAILED_ALLOCATE_MEMORY
 * if there doesn't exist the category, it returns NODE_DOES_NOT_EXIST
 * if the link list is empty, it returns FAILED_REMOVE_EMPTY_LINK_LIST
 */
int removeCategoryfromLinkList(LinkList * list, char * category_name);

/**
 *@brief Traverse the link list and apply operations.
 *
 * @param list the target link list
 * @param function_pointer the function pointer
 */
void traverseCategoryLinkList(LinkList * list,
                              void (*function_pointer)(LinkListNode *));

/**
 *@brief Show all elements in the link list.
 *
 * @param list the target link list
 */
void showCategoryLinkList(LinkList * list);

/**
 *@brief Free the memory of the link list.
 *
 * @param list the target link list
 */
void freeCategoryLinkList(LinkList * list);

void freeCategoryLinkListNode(LinkListNode * node);

/**
 *@brief Find the category in the link list.
 *
 * @param list the link list to search
 * @param category the category to find
 * @return LinkListNode* the pointer to the place of the node with the category,
 * if not found, return NULL.
 */
LinkListNode * findCategoryinLinkList(LinkList * list, char * category_name);

/**
 *@brief The operation choice for addItemstoCategory() failed to find the
 *category in the link list.
 */
enum IF_ADD_ITEM_TO_CATEGORY_LINK_LIST_FAILED {
    CREATE_NEW_CATEGORY, /* create a new category */
    TERMINATE_DIRECTLY   /* terminate the add process */
};

/**
 *@brief The status code returned by addItemstoCategory() function
 */
enum ADD_ITEM_TO_CATEGORY_STATUS_CODE {
    ADD_SUCCESS,     /* add item normally */
    CREATE_CATEGORY, /* create new category */
    FAILED_TO_ADD    /* terminate and failed to add item */
};

/**
 *@brief Add items link list according to category.
 *
 * @param list the category's link list
 * @param item the item to add
 * @param if_failed operation to do if failed to add item
 * @return int the status code from ADD_ITEM_TO_CATEGORY_STATUS_CODE
 */
int addItemstoCategory(LinkList * list, Item item, int if_failed);

#endif // !_CATEGORY_H_