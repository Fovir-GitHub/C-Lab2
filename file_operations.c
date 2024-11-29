#include "file_operations.h"
#include <string.h>

/**
 *@brief Recrusively output items' data to the file from the AVL tree.
 *
 * @param node the AVL tree node
 * @param output the FILE pointer
 */
static void outputItemsDatatoFileHelper(AVLTreeNode * node, FILE * output);

void readCategoryDatafromFile(LinkList * list)
{
    FILE * read_category = fopen(CATEGORY_FILE_PATH, "rb");
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
    FILE * read_item = fopen(ITEM_FILE_PATH, "rb");
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

void outputCategoryDatatoFile(LinkList * list)
{
    FILE * output_category = fopen(CATEGORY_FILE_PATH, "wb");
    if (!output_category) /* can't open file */
    {
        // print error message
        fprintf(stderr, "Can't open file %s\n", CATEGORY_FILE_PATH);
        exit(EXIT_FAILURE); /* terminate program */
    }

    for (LinkListNode * current = *list; current; current = current->next)
        fprintf(output_category, "%s\n", current->category_item.category_name);

    return;
}

void outputItemsDatatoFile(LinkList * list)
{
    FILE * output_item = fopen(ITEM_FILE_PATH, "wb"); /* open file */
    if (!output_item)                                 /* can't open file */
    {
        // print error message
        fprintf(stderr, "Can't open file %s\n", ITEM_FILE_PATH);
        exit(EXIT_FAILURE); /* terminate program */
    }

    // traverse all link list node and apply output item to file method
    for (LinkListNode * current = *list; current; current = current->next)
        outputItemsDatatoFileHelper(current->category_item.item_tree,
                                    output_item);

    fclose(output_item); /* close file */
    return;
}

void outputItemsDatatoFileHelper(AVLTreeNode * node, FILE * output)
{
    if (!node)
        return;

    // traverse left child node first
    outputItemsDatatoFileHelper(node->left, output);

    // output this node's information
    fprintf(output, "%s@%s@%.2lf@%04d-%02d-%02d@%04d-%02d-%02d\n",
            node->item.category, node->item.name, node->item.price,
            node->item.produce_date.year, node->item.produce_date.month,
            node->item.produce_date.day, node->item.due_date.year,
            node->item.due_date.month, node->item.due_date.day);

    // traverse right child node
    outputItemsDatatoFileHelper(node->right, output);

    return;
}
