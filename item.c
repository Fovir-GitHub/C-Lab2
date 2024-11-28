#include "item.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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