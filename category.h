#ifndef _CATEGORY_H_
#define _CATEGORY_H_

#include "item.h"

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

void initializeLinkList(LinkList * list);
int addCategorytoLinkList(LinkList * list, Category cate);

#endif // !_CATEGORY_H_