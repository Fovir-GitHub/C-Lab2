#ifndef _CATEGORY_H_
#define _CATEGORY_H_

#include "item.h"

/**
 *@brief the status code of link list's operation
 */
enum LINK_LIST_STATUS_CODES {
	SUCCESS_ADD,            /* add new node successfully */
	FAILED_ALLOCATE_MEMORY, /* can't allocate memory for new node */
	DUPLICATED_NODE         /* the node item (category) exists */
};

/**
 *@brief store the information of categories
 */
typedef struct
{
	char * category_name;
	AVLTree item_tree; /* item's tree */
} Category;

/**
 *@brief use double way link list to store categories,
storing in alphabetical order
 */
typedef struct LinkListNode
{
	Category category_item;         /* the category */
	struct LinkListNode * previous; /* pointer to the previous node */
	struct LinkListNode * next;     /* pointer to the next node */
} LinkListNode;

/**
 *@brief use to sort the link list in alphabetical order
 *
 * @param first the first link list node
 * @param last  the other one link list node
 * @return int  if first < last, return -1
 *  if last < first, return 1
 *  otherwise, return 0
 */
int compareCategoryName(LinkListNode * first, LinkListNode * last);

typedef LinkListNode * LinkList; /* define the LinkList */

/**
 * @brief initialize a link list
 * @param list the link list will be initialized
 */
void initializeLinkList(LinkList * list);

/**
 *@brief add new category to the link list
 *
 * @param list the target link list
 * @param cate the category will be added
 * @return if added successfully, it returns SUCCESS_ADD.
 * if can't allocate memory, it returns FAILED_ALLOCATE_MEMORY
 * if there has existed a same category, it returns DUPLICATED_NODE
 */
int addCategorytoLinkList(LinkList * list, Category cate);

#endif // !_CATEGORY_H_