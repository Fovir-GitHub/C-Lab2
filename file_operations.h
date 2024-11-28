#ifndef _FILE_OPERATIONS_H_
#define _FILE_OPERATIONS_H_

#include "category.h"
#include "item.h"
#include "my_utility.h"
#include <stdio.h>
#include <stdlib.h>

#define CATEGORY_FILE_PATH "./data/categories.dat"
#define ITEM_FILE_PATH "./data/items.dat"

void readCategoryDatafromFile(LinkList * list);

#endif // !_FILE_OPERATIONS_H_