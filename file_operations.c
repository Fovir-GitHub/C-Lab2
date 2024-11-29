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

        addCategorytoLinkList(list, temp_string); /* add the category */
    }

    fclose(read_category); /* close file */

    return;
}

void readItemsDatafromFile(LinkList * list)
{
    FILE * read_item = fopen(ITEM_FILE_PATH, "r");
    if (!read_item) /* can't open file */
    {
        // print error message
        fprintf(stderr, "Can't open file %s\n", ITEM_FILE_PATH);
        exit(EXIT_FAILURE); /* terminate program */
    }

    // get per line content
    char line[ITEM_FILE_PER_LINE_MAX_LENGTH];
    while (fgets(line, ITEM_FILE_PER_LINE_MAX_LENGTH, read_item))
    {
        char temp_category_name[CATEGORY_NAME_MAX_LENGTH];
        char temp_item_name[ITEM_NAME_MAX_LENGTH];
        double price = (double) 0;
        int produce_date_year = 0, produce_date_month = 0, produce_date_day = 0;
        int due_date_year = 0, due_date_month = 0, due_date_day = 0;

        // read information from the line string
        sscanf(line, "%[^@]@%[^@]@%lf@%d-%d-%d@%d-%d-%d", temp_category_name,
               temp_item_name, &price, &produce_date_year, &produce_date_month,
               &produce_date_day, &due_date_year, &due_date_month,
               &due_date_day);

        // make the item and add to the list
        addItemstoCategory(
            list,
            makeItem(temp_category_name, temp_item_name, price,
                     makeDateInformation(produce_date_year, produce_date_month,
                                         produce_date_day),
                     makeDateInformation(due_date_year, due_date_month,
                                         due_date_day)),
            CREATE_NEW_CATEGORY);
    }

    fclose(read_item); /* close file */

    return;
}
