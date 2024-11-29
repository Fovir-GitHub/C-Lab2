#ifndef _FILE_OPERATIONS_H_
#define _FILE_OPERATIONS_H_

#include "category.h"
#include "item.h"
#include "my_utility.h"
#include <stdio.h>
#include <stdlib.h>

#define CATEGORY_FILE_PATH "./data/categories.dat"
#define ITEM_FILE_PATH "./data/items.dat"

/**
 *@brief Read categories from file.
 *
 * @param list the link list of categories
 */
void readCategoryDatafromFile(LinkList * list);

/**
 *@brief Read items from file.
 *
 * @param list the category's link list
 */
void readItemsDatafromFile(LinkList * list);

#endif // !_FILE_OPERATIONS_H_