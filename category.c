#include "category.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareCategoryName(LinkListNode * first, LinkListNode * last)
{
	// get the alphabetically compare result
	int compare_result = strcmp(first->category_item.category_name,
	                            last->category_item.category_name);

	if (compare_result < 0)
		return -1;
	else if (compare_result > 0)
		return 1;
	else
		return 0;
}

void initializeLinkList(LinkList * list)
{
	*list = NULL; /* set the link list root to NULL */

	// assign NULL to the previous and next node of the root node
	(*list)->previous = (*list)->next = NULL;
	return;
}

int addCategorytoLinkList(LinkList * list, Category cate)
{
	LinkListNode * add_node =
	    (LinkListNode *)malloc(sizeof(LinkListNode)); /* need free */
	if (!add_node) /* can't allocate memory */
	{
		// print error message
		fprintf(stderr, "Can't allocate memory for new category\n");
		return FAILED_ALLOCATE_MEMORY; /* return status code */
	}

	add_node->category_item = cate; /* assign the category */

	// set the previous and the next node to NULL
	add_node->previous = add_node->next = NULL;

	if (*list == NULL) /* the link list is empty */
		*list = add_node;
	else
	{
		LinkListNode * current = *list; /* traverse the link list */
		while (current)                 /* the current node is not NULL */
		{
			// get the compare result between categories
			int compare_result =
			    compareCategoryName(add_node, current->category_item);

			if (compare_result == 0)    /* the two categories are the same */
				return DUPLICATED_NODE; /* return the status code */

			/**
			 * the name of the new category is alphabetically less than the
			 * current category, then insert the new node before current node
			 */
			if (compare_result < 0)
			{
				/**
				 * if the current node is the root node
				 * then insert the new node before the root node
				 * and change the root node to the new node
				 */
				if (current == *list)
				{
					// insert the new node before the root node
					add_node->next = *list;
					(*list)->previous = add_node;

					// change the root node to the new node
					*list = add_node;
				}
				else
				{
					/**
					 * backup_previous <---> current
					 * backup_previous <---> new_node <---> current
					 *
					 */
					LinkListNode * backup_previous_node = current->previous;

					// link the new node and the previous node
					backup_previous_node->next = add_node;
					add_node->previous = backup_previous_node;

					// link the new node and current node
					add_node->next = current;
					current->previous = add_node;
				}

				break;
			}
			current = current->next;
		}
	}

	return SUCCESS_ADD; /* return success status code */
}
