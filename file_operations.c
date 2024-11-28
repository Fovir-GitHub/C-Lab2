#include "file_operations.h"
#include <string.h>

void readCategoryDatafromFile(LinkList * list)
{
    FILE * read_category = fopen(CATEGORY_FILE_PATH, "r");
    if (!read_category) /* can't open file */
    {
        // print error message
        fprintf(stderr, "Can't open file %s\n", CATEGORY_FILE_PATH);
        exit(EXIT_FAILURE); /* terminate program */
    }

    char temp_string[CATEGORY_NAME_MAX_LENGTH];
    while (fgets(temp_string, CATEGORY_NAME_MAX_LENGTH, read_category))
    {
        // remove line breaker
        char * find = strchr(temp_string, '\n');
        if (find)
            *find = '\0';

        Category category = makeCategory(temp_string);
        addCategorytoLinkList(list, category);
    }

    fclose(read_category);

    return;
}