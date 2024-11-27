#include "category.h"
#include "item.h"
#include "my_utility.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	const int LENGTH = 512;

	LinkList category_name;
	initializeLinkList(&category_name);

	for (int i = 0; i < 5; i++)
	{
		Category temp;
		getString(&temp.category_name, LENGTH);
		int status_code = addCategorytoLinkList(&category_name, temp);

		switch (status_code)
		{
		case SUCCESS_ADD:
			puts("Success add");
			break;
		case DUPLICATED_NODE:
			puts("Duplicated node");
			break;
		}
	}

	showCategoryLinkList(&category_name);
}