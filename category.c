#include "category.h"
#include "constants.h"
#include "my_utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Category makeCategory(const char * name)
{
    Category result;

    // copy category's name
    result.category_name = (char *) malloc((strlen(name) + 1) * sizeof(char));
    strcpy(result.category_name, name);

    // initialize the AVL tree
    initializeAVLTree(&result.item_tree);

    return result;
}

int compareCategoryName(const LinkListNode * first, const LinkListNode * second)
{
    return compare2Strings(first->category_item.category_name,
                           second->category_item.category_name);
}

void initializeLinkList(LinkList * list)
{
    *list = NULL; /* set the link list root to NULL */

    return;
}

bool emptyLinkList(LinkList * list)
{
    return *list == NULL;
}

int addCategorytoLinkList(LinkList * list, char * category_name)
{
    if (!legalString(category_name)) /* if the category's name is illegal */
        return ILLEGAL_STRING;

    LinkListNode * add_node = (LinkListNode *) malloc(sizeof(LinkListNode));
    if (!add_node) /* can't allocate memory */
    {
        // print error message
        fprintf(stderr, "Can't allocate memory for new category in %s()\n",
                __func__);
        return FAILED_ALLOCATE_MEMORY; /* return status code */
    }

    add_node->category_item =
        makeCategory(category_name); /* assign the category */

    // set the previous and the next node to NULL
    add_node->previous = add_node->next = NULL;

    if (emptyLinkList(list)) /* the link list is empty */
        *list = add_node;
    else
    {
        LinkListNode * current = *list; /* traverse the link list */

        // compare with the root node of the link list
        int compare_result = compareCategoryName(current, add_node);
        if (compare_result == 0)    /* same categories */
            return DUPLICATED_NODE; /* return status code */
        else if (compare_result == 1)
        {
            /**
             * the root node is alphabetically greater than the new node
             * insert the node before the root node
             * and replace the root node with the new node
             */

            add_node->next = current;     /* new node links to root node */
            current->previous = add_node; /* root link to the new node */
            *list = add_node;             /* replace the root node */

            return SUCCESS_ADD; /* return status code */
        }

        while (current->next) /* the current->next node is not NULL */
        {
            // get the compare result between categories
            compare_result = compareCategoryName(current->next, add_node);

            if (compare_result == 0)    /* the two categories are the same */
                return DUPLICATED_NODE; /* return the status code */

            /**
             * the name of the new category is alphabetically greater than
             * the next category, then insert the new node after current
             * node
             */
            if (compare_result == 1)
            {
                LinkListNode * backup_next_node = current->next;

                // link current node and the new node
                current->next = add_node;
                add_node->previous = current;

                // link the new node and the backup next node
                add_node->next = backup_next_node;
                backup_next_node->previous = add_node;

                return SUCCESS_ADD;
            }
            current = current->next;
        }
        /**
         * the node is alphabetically greatest node
         * append it to the end of the link list
         */
        current->next = add_node;     /* link the last node to the new node */
        add_node->previous = current; /* link the new node to the last node */
    }

    return SUCCESS_ADD; /* return success status code */
}

int removeCategoryfromLinkList(LinkList * list, char * category_name)
{
    LinkListNode * current = *list;
    LinkListNode * temp_node =
        (LinkListNode *) malloc(sizeof(LinkListNode)); /* for compare */

    if (!temp_node) /* can't allocate memory */
    {
        fprintf(stderr, "Can't allocate memory for temp_node in %s()\n",
                __func__);
        return FAILED_ALLOCATE_MEMORY;
    }

    temp_node->category_item.category_name = category_name;

    if (!current) /* the link list is empty */
    {
        free(temp_node);
        return FAILED_REMOVE_EMPTY_LINK_LIST;
    }

    while (current)
    {
        int compare_result = compareCategoryName(current, temp_node);

        /**
         * the alphabetical order is greater than the name
         * the name doesn't exist
         * return status code directly
         */
        if (compare_result == 1)
        {
            free(temp_node); /* free temp node */
            return NODE_DOES_NOT_EXIST;
        }

        if (compare_result == 0) /* match the name */
        {
            if (current == *list) /* remove the root node */
            {
                if (current->next)         /* if there exists next node */
                    *list = current->next; /* update root node */
                else                       /* if there is no next node */
                {
                    // free memory
                    free(current->category_item.category_name);
                    freeAVLTree(&current->category_item.item_tree);

                    *list = NULL;          /* assign root to NULL */
                    return SUCCESS_REMOVE; /* return status code */
                }
            }
            else
            {
                // update the previous node's next node
                current->previous->next = current->next;

                // update the next node's previous node
                if (current->next) /* avoid the last node */
                    current->next->previous = current->previous;
            }

            freeCategoryLinkListNode(current); /* free current node */
            free(temp_node);                   /* free temp node*/
            return SUCCESS_REMOVE;             /* return status code */
        }
        current = current->next;
    }

    return NODE_DOES_NOT_EXIST; /* the node doesn't exist */
}

void traverseCategoryLinkList(LinkList * list,
                              void (*function_pointer)(LinkListNode *))
{
    for (LinkListNode * current = *list; current; current = current->next)
        function_pointer(current);

    return;
}

void freeCategoryLinkList(LinkList * list)
{
    traverseCategoryLinkList(list, freeCategoryLinkListNode);
    return;
}

LinkListNode * findCategoryinLinkList(LinkList * list, char * category_name)
{
    if (!legalString(category_name)) /* the category name is illegal */
        return NULL;

    for (LinkListNode * current = *list; current; current = current->next)
    {
        int compare_result = compare2Strings(
            current->category_item.category_name, category_name);

        if (compare_result == 0) /* match the category's name */
            return current;
        if (compare_result == 1) /* not possible find the category's name */
            return NULL;
    }

    return NULL; /* category doesn't exist */
}

int getLinkListSize(LinkList * list)
{
    int size = 0;
    for (LinkListNode * iter = *list; iter; iter = iter->next) size++;

    return size;
}

int addItemstoCategory(LinkList * list, Item item, int if_failed)
{
    // try to find the category's position
    LinkListNode * category_position =
        findCategoryinLinkList(list, item.category);

    if (!category_position)                  /* not found the category */
        if (if_failed == TERMINATE_DIRECTLY) /* terminate */
            return FAILED_TO_ADD;
        else /* create new category */
        {

            addCategorytoLinkList(
                list, item.category); /* add the category to the link list */
            addItemstoCategory(list, item, if_failed); /* readd the item */

            return CREATE_NEW_CATEGORY; /* return the status code */
        }

    // find the category, insert the item
    insertAVLTreeNode(&category_position->category_item.item_tree, item);
    return ADD_SUCCESS; /* return status code */
}

void freeCategoryLinkListNode(LinkListNode * node)
{
    free(node->category_item.category_name);     /* free item name string */
    freeAVLTree(&node->category_item.item_tree); /* free tree */
    free(node);                                  /* free the node space */
}