#ifndef _FILE_OPERATIONS_H_
#define _FILE_OPERATIONS_H_

#include "category.h"
#include "item.h"
#include "my_utility.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * The categories.dat format:
 *  category name 1
 *  category name 2
 *  ...
 */
#define CATEGORY_FILE_PATH "./data/categories.dat"

/**
 * The items.dat format (per line):
 * category_name@item_name@price@produce_date@due_date
 * the produce_date and due_date formats are:
 * YYYY-MM-DD
 * and if they are blank, the format should also be
 * 0000-00-00
 */
#define ITEM_FILE_PATH "./data/items.dat"

// the max length of every line in items.dat
#define ITEM_FILE_PER_LINE_MAX_LENGTH \
    CATEGORY_NAME_MAX_LENGTH + ITEM_NAME_MAX_LENGTH + 32

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