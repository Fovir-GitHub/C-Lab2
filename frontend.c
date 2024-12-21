#include "frontend.h"
#include "constants.h"
#include "my_utility.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ORDER_NUMBER_WIDTH 4
#define PRICE_STRING_MAX_LENGTH 24
#define DATE_STRING_MAX_LENGTH 12

/**
 *@brief Used to show the categories in a menu form.
 *
 * @param current the current category location
 * @param current_page_number the current page number
 * @param total_page_number the total page numebr
 */
static void showCategoriesHelper(LinkListNode * current,
                                 int current_page_number,
                                 int total_page_number);

// the index of category list
static int print_category_in_list_form_index = 0;

/**
 *@brief Help to print categories in list form.
 *
 * @param node the node to operate
 */
static void printCategoryListinListFormHelper(const LinkListNode * node);

// the index of generating AVL tree node into array
static int generate_AVL_tree_node_pointer_array_index = 0;

/**
 *@brief Help to generate AVL tree pointer array.
 *
 * @param node the node to operate
 * @param result_array the result array
 */
static void generateAVLTreeNodePointerArrayHelper(AVLTreeNode * node,
                                                  AVLTreeNode ** result_array);

/**
 *@brief Help to select an item.
 *
 * @param node the link list node to select the item from
 * @param current_page current page number
 * @param total_page total page number
 * @return SelectResult the result of selection
 */
static SelectResult selectItemHelper(LinkListNode * node, int current_page,
                                     int total_page);

/**
 *@brief Show the interface for selectItemHelper() function.
 *
 * @param category_name the category's name to be shown in the first line
 * @param node_array the pointer array of tree node pointer
 * @param array_size the size of the array
 * @param current_page current page number
 * @param total_page total page number
 */
static void selectItemHelperHelper(const char * category_name,
                                   AVLTreeNode ** node_array, int array_size,
                                   int current_page, int total_page);

SelectResult makeSelectResult(LinkListNode * category_position,
                              AVLTreeNode * item_position)
{
    SelectResult result = {NULL, NULL};
    result.category_position = category_position;
    result.item_position = item_position;

    return result;
}

int calculateCenterStringSpace(const char * str)
{
    // calculate center the string need how many space before it
    return abs(MENU_WIDTH - strlen(str)) / 2;
}

void printStringinCenter(const char * str)
{
    printf("%*s%s\n", calculateCenterStringSpace(str) - 1, "", str);
    return;
}

void printMenuTitle(const char * title)
{
    printMenuFrame('=');
    printStringinCenter(title);
    printMenuFrame('-');

    return;
}

void printMenuFrame(char frame_character)
{
    for (int i = 0; i < MENU_WIDTH; i++) putchar(frame_character);
    putchar('\n');
}

void printOptions(const char * options[], int options_number)
{
    int max_odd_string_length = 0, max_even_string_length = 0,
        max_string_length = 0;

    // get max_odd_string_length and max_even_string_length
    for (int i = 0; i < options_number; i++)
        if ((i + 1) & 1) /* odd number */
            max_odd_string_length =
                MAX(max_odd_string_length, strlen(options[i]));
        else
            max_even_string_length =
                MAX(max_even_string_length, strlen(options[i]));

    max_odd_string_length +=
        ORDER_NUMBER_WIDTH; /* get the max width of odd string */
    max_even_string_length +=
        ORDER_NUMBER_WIDTH; /* get the max width of even string */
    max_string_length =
        MAX(max_odd_string_length,
            max_even_string_length); /* get the max width of all strings */

    char * temp_string =
        (char *) malloc((max_string_length + 1) * sizeof(char));

    // get the space width between two options
    int middle_space_width =
        MENU_WIDTH - max_odd_string_length - max_even_string_length;

    for (int i = 0; i < options_number; i++)
    {
        // the order number should be q if the option is Quit
        sprintf(temp_string, "[%c] %s",
                (strcmp("Quit", options[i]) == 0) ? 'q' : (i + '1'),
                options[i]);

        if ((i + 1) & 1) /* odd order string */
            printf("%-*s%*c", max_odd_string_length, temp_string,
                   middle_space_width,
                   '\0'); /* align to left and print the middle space */
        else
            printf("%-*s\n", max_even_string_length,
                   temp_string); /* align to left and be sure that the
                                    longest string reaches the right side */
    }

    if (options_number & 1) /* if the number of options is odd, */
        putchar('\n');      /* output line breaker */

    free(temp_string);

    return;
}

void printMenuFooter(const char * footer)
{
    printMenuFrame('-');
    printStringinCenter(footer);
    printMenuFrame('=');
}

void showMainMenu()
{
    clearScreen();
    printMenuTitle("Main Menu");

    const static char * options[MAIN_MENU_OPTIONS_NUMBER] = {
        "Show Categories", "Add Category", "Remove Category", "Show All Items",
        "Add Item",        "Remove Item",  "Edit Item",       "Quit",
    };

    printOptions(options, MAIN_MENU_OPTIONS_NUMBER);
    printMenuFooter(POWER_FOOTER);
    printf("Please enter your choice: ");
}

int getMainMenuChoicefromUser()
{
    char user_choice = 0;

    // initialized status
    showMainMenu();

    while (user_choice = getchar()) /* get user's input */
    {
        if (user_choice == '\n') /* user inputs line breaker */
            continue;            /* ignore */

        // find whether the choice is valid
        if (!strchr(MAIN_MENU_OPTIONS_STRING, user_choice)) /* not valid */
        {
            // tell user to input new option.
            printf("You entered an invalid chioce! Press Enter to "
                   "continue...");
            eatLine(); /* avoid multiple characters */
        }
        else
        {
            eatLine(); /* avoid multiple characters */

            // judge user's choice and return the choice
            switch (user_choice)
            {
            case '1':
                return SHOW_ALL_CATEGORIES;
            case '2':
                return ADD_CATEGORY;
            case '3':
                return REMOVE_CATEGORY;
            case '4':
                return SHOW_ALL_ITEMS;
            case '5':
                return ADD_ITEM;
            case '6':
                return REMOVE_ITEM;
            case '7':
                return EDIT_ITEM;
            case 'q':
                return QUIT;
            }
        }

        eatLine();
        showMainMenu(); /* show the new main menu */
    }

    return 0;
}

void showCategories(LinkList * list)
{
    // get total page number
    int total_page_number =
        ceil((double) getLinkListSize(list) / CAGEGORY_NUMBER_PER_PAGE);

    showCategoriesHelper(*list, total_page_number > 0 ? 1 : 0,
                         total_page_number); /* show the first page */
    return;
}

void printShowCategories(LinkListNode * current, int current_page_number,
                         int total_page_number)
{
    printMenuTitle("Category");

    // print categories' name
    char output_string[CATEGORY_NAME_MAX_LENGTH + ORDER_NUMBER_WIDTH];
    int counter = 1;
    for (LinkListNode * iter = current;
         iter && counter <= CAGEGORY_NUMBER_PER_PAGE;
         iter = iter->next, counter++)
    {
        sprintf(output_string, "[%d] %s", counter,
                iter->category_item.category_name);
        printStringinCenter(output_string);
    }
    printStringinCenter("[q] quit"); /* print the quit option */

    // print footer
    sprintf(output_string, "[p]revious page    %d/%d    [n]ext page",
            current_page_number, total_page_number);
    printMenuFooter(output_string);
    printf("Please enter your choice: ");
}

void removeCategory(LinkList * list)
{
    if (emptyLinkList(list)) /* there is no category */
    {
        printf("You don't have any category! %s", ENTER_CONTINUE);
        eatLine();
        return;
    }

    SelectResult select_result = selectCategory(list);
    if (!select_result.category_position) /* quit */
        return;

    if (!emptyAVLTree(
            &select_result.category_position->category_item.item_tree))
    {
        printf("The category is not empty! Removing the category will also "
               "remove the items!\nConfirm to remove? [y/N] ");

        if (tolower(getchar()) != 'y')
            return;
        else
            eatLine();
    }

    // get the status after removing category
    int status = removeCategoryfromLinkList(
        list, select_result.category_position->category_item.category_name);

    if (status == SUCCESS_REMOVE) /* successfully remove */
        printf("Remove successfully! %s", ENTER_CONTINUE);
    else /* failed to remove */
        printf("Failed to remove the category! %s", ENTER_CONTINUE);

    eatLine();
    return;
}

void printCategoryListinListForm(LinkList * list)
{
    // initialize the index
    print_category_in_list_form_index = 1;

    printMenuTitle("Category"); /* print the title */

    // print the options
    traverseCategoryLinkList(list, printCategoryListinListFormHelper);

    printStringinCenter("[q] quit"); /* print the quit opiton */
    printMenuFooter(POWER_FOOTER);   /* print the footer */

    return;
}

void addCategory(LinkList * list)
{
    char * add_category_name = NULL; /* the name of the new category */

    while (1)
    {
        printf("Enter the new category's name (quit to quit): ");
        getString(&add_category_name, CATEGORY_NAME_MAX_LENGTH);

        if (strcmp("quit", add_category_name) == 0) /* quit */
            break;                                  /* jump out the loop */

        if (!legalString(add_category_name)) /* the name is illegal */
        {
            printf("The category name is illegal! %s", ENTER_CONTINUE);
            eatLine();
            free(add_category_name);
            continue;
        }

        // get the status code after add to link list
        int add_result = addCategorytoLinkList(list, add_category_name);

        switch (add_result)
        {
        case DUPLICATED_NODE:
        case ILLEGAL_STRING:
            printf("The category \"%s\" exists!\n", add_category_name);
            printf("Enter another category name? [Y/n] ");

            if (tolower(getchar()) == 'n')
            {
                free(add_category_name);
                return;
            }
            else
            {
                free(add_category_name);
                continue;
            }
        case FAILED_ALLOCATE_MEMORY:
            free(add_category_name);
            exit(EXIT_FAILURE); /* terminate the programme */

        case SUCCESS_ADD: /* successfully added the category */
            printf("Successfully add new category \"%s\"\n", add_category_name);
            printf("%s", ENTER_CONTINUE);
            eatLine();
            free(add_category_name);
            return;
        }
    }

    return;
}

void showItems(LinkList * list)
{
    if (emptyLinkList(list))
    {
        printf("No items! %s", ENTER_CONTINUE);
        eatLine();

        return;
    }

    SelectResult select_result = selectItem(list);

    if (select_result.category_position && select_result.item_position)
    {
        showItemInformation(&select_result.item_position->item);

        printf("Pree Enter to continue...");
        eatLine();
    }

    return;
}

void addItem(LinkList * list)
{
    DateInformation produce_date = getToday(); /* set produce date to today */
    DateInformation due_date =
        makeDateInformation(0, 0, 0);         /* set due date to empty */
    char * item_name = NULL;                  /* item's name */
    double price = (double) 0;                /* item's price */
    char date_string[DATE_STRING_MAX_LENGTH]; /* string to get date from user */

    // temp data
    DateInformation temp_date;
    Item temp_item;
    int temp_year = 0, temp_month = 0, temp_day = 0;

    const LinkListNode * category_position =
        selectCategory(list).category_position;

    if (!category_position)
        return;

    while (1)
    {
        // get item's name
        printf("Please enter the item's name (quit to quit): ");
        getString(&item_name, ITEM_NAME_MAX_LENGTH);

        if (strcmp("quit", item_name) == 0) /* quit */
        {
            free(item_name); /* free space */
            return;
        }

        if (!legalString(item_name)) /* the name is illegal */
        {
            printf("The item name \"%s\" is illegal! Enter another "
                   "one? [Y/n] ",
                   item_name);

            free(item_name); /* free space */

            if (tolower(getchar()) == 'n')
                return;
        }
        else
            break;
    }

    // get the price
    printf("Please enter the price: ");
    while (scanf("%lf", &price) != 1) /* fail to get price */
    {
        printf("The format is invalid! Enter another price? [Y/n] ");
        if (tolower(getchar()) == 'n')
            return;

        printf("Please enter the price: ");
    }

    eatLine(); /* flush the buffer */

    // get produce date
    printf(
        "Please enter the produce date in YYYY-MM-DD format (leave blank will "
        "set it to today): ");
    fgets(date_string, sizeof(date_string), stdin); /* read the whole line */

    // read data from string
    sscanf(date_string, "%d-%d-%d", &temp_year, &temp_month, &temp_day);

    if (date_string[0] != '\n') /* the string is empty */
        if (!validDate(
                temp_date = makeDateInformation(
                    temp_year, temp_month, temp_day))) /* the date is invalid */
            puts("The date is invalid! Set produce date to today by default!");
        else                          /* the date is valid */
            produce_date = temp_date; /* assign the date */

    // get due date
    printf("Please enter the expiration date in YYYY-MM-DD format (leave blank "
           "will set it to blank): ");
    fgets(date_string, sizeof(date_string), stdin); /* read the whole line */

    // read data from string
    sscanf(date_string, "%d-%d-%d", &temp_year, &temp_month, &temp_day);

    if (date_string[0] != '\n') /* the string is empty */
        if (!validDate(temp_date = makeDateInformation(temp_year, temp_month,
                                                       temp_day)))
            puts("The date is invalid! Set expiration date to blank by "
                 "default!");
        else
            due_date = temp_date;

    // get status code after add item
    int status = addItemstoCategory(
        list,
        temp_item = makeItem(category_position->category_item.category_name,
                             item_name, price, produce_date, due_date),
        CREATE_NEW_CATEGORY);

    if (status == FAILED_TO_ADD) /* failed to add the item */
        printf("Failed to add the item! ");
    else
    {
        puts("Add item successfully!\n");

        // show item's information after being added
        showItemInformation(&temp_item);
    }

    printf("%s", ENTER_CONTINUE);
    eatLine();

    return;
}

void removeItem(LinkList * list)
{
    SelectResult select_result = selectItem(list);
    if (!select_result.category_position || !select_result.item_position)
        return;

    removeItemfromAVLTree(
        &select_result.category_position->category_item.item_tree,
        select_result.item_position->item.name);

    printf("Remove successfully! %s", ENTER_CONTINUE);
    eatLine();

    return;
}

void editItem(LinkList * list)
{
    SelectResult select_result = selectItem(list);

    // the user choose to quit
    if (!(select_result.category_position && select_result.item_position))
        return;

    clearScreen(); /* clear screen before showing item's information */

    // show the item's information at first
    showItemInformation(&select_result.item_position->item);

    // back up original data
    double price = select_result.item_position->item.price;

    // record whether the item is changed
    bool changed = false;

    // the new item will be added after the process
    Item new_item = select_result.item_position->item;

    // temp data
    char * temp_string = NULL;
    char date_string[DATE_STRING_MAX_LENGTH]; /* get date information */
    int temp_year = 0, temp_month = 0, temp_day = 0;
    DateInformation temp_date;

    printf("Please enter the new category (leave blank will change "
           "nothing): ");
    getString(&temp_string, CATEGORY_NAME_MAX_LENGTH);

    if (!emptyString(temp_string)) /* avoid alarm when user leave blank */
        if (legalString(temp_string) &&
            findCategoryinLinkList(list, temp_string)) /* the string is valid */
        {
            changed = true; /* update change status */

            // assign the category name
            new_item.category = staticString2dynamicString(temp_string);
        }
        else
            puts("The string is illegal or the category does not exist! The "
                 "category does not change!");

    free(temp_string); /* free space */

    printf("Please enter the new item's name (leave blank will change "
           "nothing): ");
    getString(&temp_string, ITEM_NAME_MAX_LENGTH);

    if (!emptyString(temp_string)) /* avoid alarm when user leave blank */
        if (legalString(temp_string))
        {
            changed = true; /* update status */

            // assign the name
            new_item.name = staticString2dynamicString(temp_string);
        }
        else
            puts("The string is illegal! The item's name does not "
                 "change!");

    free(temp_string); /* free space */

    printf("Please enter the new price (leave blank will do nothing): ");
    getString(&temp_string, PRICE_STRING_MAX_LENGTH);

    if (!emptyString(temp_string))
        if (sscanf(temp_string, "%lf", &new_item.price) != 1)
        {
            puts("Invalid price format! The price does not change!");
            new_item.price = price; /* reset to the previous price */
        }
        else
            changed = true; /* update status */

    free(temp_string); /* free space */

    // get the produce date
    printf("Please enter the produce date (leave blank will do "
           "nothing): ");
    fgets(date_string, sizeof(date_string), stdin);
    if (date_string[0] == '\n') /* the line is blank */
        ;                       /* do nothing and skip */
    // invalid date
    else if (sscanf(date_string, "%d-%d-%d", &temp_year, &temp_month,
                    &temp_day) != 3 ||
             !validDate(temp_date = makeDateInformation(temp_year, temp_month,
                                                        temp_day)))
        puts("Invalid date! The produce date does not change!");
    else /* the date in valid */
    {
        changed = true;                    /* update the status */
        new_item.produce_date = temp_date; /* assign the date */
    }

    // get new due date
    printf("Please enter the expiration date (leave blank will do "
           "nothing): ");
    fgets(date_string, sizeof(date_string), stdin);
    if (date_string[0] == '\n')
        ;
    else if (sscanf(date_string, "%d-%d-%d", &temp_year, &temp_month,
                    &temp_day) != 3 ||
             !validDate(temp_date = makeDateInformation(temp_year, temp_month,
                                                        temp_day)))
        puts("Invalid date! The expiration date does not change!");
    else
    {
        changed = true;
        new_item.due_date = temp_date;
    }

    if (changed) /* if the information is changed */
    {
        // remove the original item at first
        removeItemfromAVLTree(
            &select_result.category_position->category_item.item_tree,
            select_result.item_position->item.name);

        // re-add the item
        addItemstoCategory(list, new_item, TERMINATE_DIRECTLY);

        // print message
        puts("The item has been updated!");

        showItemInformation(&new_item);
        printf("%s", ENTER_CONTINUE);
    }
    else
        printf("There is nothing to change! %s", ENTER_CONTINUE);

    eatLine();

    return;
}

SelectResult selectCategory(LinkList * list)
{
    int list_size = getLinkListSize(list); /* get the list size */
    int user_choice = 0;                   /* the user's choice */
    SelectResult result = {NULL, NULL};    /* initialize the result */

    // show all categories in list form
    clearScreen();
    printCategoryListinListForm(list);

    // ask user to select
    printf("Please select the category: ");

    // the input is number
    while ((user_choice = readNumberOrAlpha()) != READ_NUMBER_OR_ALPHA_Q)
    {
        if (user_choice != READ_NUMBER_OR_ALPHA_P &&
            user_choice != READ_NUMBER_OR_ALPHA_N &&
            user_choice != READ_NUMBER_OR_ALPHA_ERROR)
        {
            // the choice is avaliable
            if (1 <= user_choice && user_choice <= list_size)
            {
                // find the category's position
                int counter = 1;
                LinkListNode * iter = NULL;
                for (iter = *list; iter && counter < user_choice;
                     iter = iter->next, counter++);

                result = makeSelectResult(iter, NULL);
                break;
            }
        }
        else
        {
            clearScreen();
            printCategoryListinListForm(list);

            printf("Please select the category: ");
        }
    }

    return result;
}

AVLTreeNode ** generateAVLTreeNodePointerArray(AVLTree * tree)
{
    int tree_size = getAVLTreeSize(tree); /* get the tree size */

    // initialize the index
    generate_AVL_tree_node_pointer_array_index = 0;
    // allocate memory
    AVLTreeNode ** result_array =
        (AVLTreeNode **) malloc(tree_size * sizeof(AVLTreeNode *));
    if (!result_array) /* can't allocate memory */
    {
        // print error message
        fprintf(stderr, "Can't allocate memory for result_array in %s()!",
                __func__);
        exit(EXIT_FAILURE); /* terminate program */
    }

    // generate the array
    generateAVLTreeNodePointerArrayHelper(*tree, result_array);

    return result_array;
}

SelectResult selectItem(LinkList * list)
{
    if (noIteminLinkList(list))              /* there is no items */
        return makeSelectResult(NULL, NULL); /* return NULL */

    return selectItemHelper(*list, 1, getLinkListSize(list));
}

void showCategoriesHelper(LinkListNode * current, int current_page_number,
                          int total_page_number)
{
    // show current page
    clearScreen();
    printShowCategories(current, current_page_number, total_page_number);

    int choice = 0; /* get user's choice */

    while ((choice = readNumberOrAlpha()) !=
           READ_NUMBER_OR_ALPHA_Q) /* the choice is not quit */
    {
        LinkListNode * iter = current;

        if (choice == READ_NUMBER_OR_ALPHA_P ||
            choice == READ_NUMBER_OR_ALPHA_N) /* legal option */
        {
            int counter = 1;
            // judge whether the next page or the previous page exist
            for (; iter && counter <= CAGEGORY_NUMBER_PER_PAGE;
                 iter = (choice == READ_NUMBER_OR_ALPHA_P ? iter->previous
                                                          : iter->next))
                counter++;

            if (iter) /* the page exist, go to the page */
                return showCategoriesHelper(iter,
                                            (choice == READ_NUMBER_OR_ALPHA_P
                                                 ? (current_page_number - 1)
                                                 : (current_page_number + 1)),
                                            total_page_number);
        }

        clearScreen();
        printShowCategories(current, current_page_number, total_page_number);
    }
    return;
}

void printCategoryListinListFormHelper(const LinkListNode * node)
{
    /**
     * the output line format: [xx] name
     * + 1 is allocate for the '\0'
     * + 3 is allocate for the [] and the space
     */

    // allocate the memory
    char * output_line = (char *) malloc(
        strlen(node->category_item.category_name) +
        getNumberofDigits(print_category_in_list_form_index) + 1 + 3);
    if (!output_line) /* can't allocate memory */
    {
        // print error message
        fprintf(stderr, "Can't allocate memory for output_line in %s()!",
                __func__);
        exit(EXIT_FAILURE); /* terminat the program */
    }

    // construct the option's string
    sprintf(output_line, "[%d] %s", print_category_in_list_form_index,
            node->category_item.category_name);

    printStringinCenter(output_line); /* print the option */

    free(output_line); /* free memory space */

    print_category_in_list_form_index++; /* increase the index */
    return;
}

void generateAVLTreeNodePointerArrayHelper(AVLTreeNode * node,
                                           AVLTreeNode ** result_array)
{
    if (!node)
        return;

    // traverse left
    generateAVLTreeNodePointerArrayHelper(node->left, result_array);

    // assign the pointer and increase the index
    *(result_array + generate_AVL_tree_node_pointer_array_index++) = node;

    // traverse right
    generateAVLTreeNodePointerArrayHelper(node->right, result_array);

    return;
}

SelectResult selectItemHelper(LinkListNode * node, int current_page,
                              int total_page)
{
    // get the size of item's tree
    int tree_size = getAVLTreeSize(&node->category_item.item_tree);

    // construct the pointer array for tree node pointer
    AVLTreeNode ** tree_node_pointer_array =
        generateAVLTreeNodePointerArray(&node->category_item.item_tree);

    // show the interface
    selectItemHelperHelper(node->category_item.category_name,
                           tree_node_pointer_array, tree_size, current_page,
                           total_page);

    int user_choice = 0; /* user's choice */

    // user's choice is not quit
    while ((user_choice = readNumberOrAlpha()) != READ_NUMBER_OR_ALPHA_Q)
    {
        // previous page
        if (user_choice == READ_NUMBER_OR_ALPHA_P && current_page > 1)
        {
            free(tree_node_pointer_array);
            return selectItemHelper(node->previous, current_page - 1,
                                    current_page);
        }
        else if (user_choice == READ_NUMBER_OR_ALPHA_N &&
                 current_page < total_page) /* next page */
        {
            free(tree_node_pointer_array);
            return selectItemHelper(node->next, current_page + 1, total_page);
        }
        else if (1 <= user_choice && user_choice <= tree_size)
        {
            // select an item
            AVLTreeNode * item_position =
                *(tree_node_pointer_array + user_choice - 1);
            free(tree_node_pointer_array);

            return makeSelectResult(node, item_position);
        }

        // invalid choice, show current page again.
        selectItemHelperHelper(node->category_item.category_name,
                               tree_node_pointer_array, tree_size, current_page,
                               total_page);
    }

    // choose quit
    free(tree_node_pointer_array);       /* free memory space */
    return makeSelectResult(NULL, NULL); /* return NULL */
}

void selectItemHelperHelper(const char * category_name,
                            AVLTreeNode ** node_array, int array_size,
                            int current_page, int total_page)
{
    clearScreen(); /* clear screen at first */

    // get footer
    char footer[MENU_WIDTH];
    sprintf(footer, "[p]revious    %d/%d    [n]ext", current_page, total_page);

    printMenuTitle("Item");

    // print category name
    printf("%-*s\n", MENU_WIDTH, category_name);

    // there is no item
    if (array_size == 0)
        printStringinCenter("No items!");
    else
        for (int i = 0; i < array_size; i++)
        {
            char item_name[ITEM_NAME_MAX_LENGTH]; /* store item's name */

            // show items
            sprintf(item_name, "[%d] %s", i + 1, (*node_array[i]).item.name);
            printStringinCenter(item_name);
        }

    printStringinCenter("[q] quit"); /* print quit option */
    printMenuFooter(footer);         /* print footer */

    printf("Enter your choice: "); /* ask user to enter choice */
    return;
}
