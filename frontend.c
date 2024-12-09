#include "frontend.h"
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

/**
 *@brief Used to print the items according its category to every page.
 *
 * @param node the link list node
 */
static void printShowItems(char * category_name, AVLTreeNode * node_array,
                           int array_size, int current_page, int total_page);

/**
 *@brief Show items with category in a recrusive way.
 *
 * @param current current category
 * @param current_page_number current page number
 * @param total_page_number total page number
 */
static void showItemsHelper(LinkListNode * current, int current_page_number,
                            int total_page_number);

/**
 *@brief Used to traverse AVL tree to print the item's name in the center.
 *
 * @param node the AVL tree node
 */
static void printAVLTreeNodeCenterHelper(AVLTreeNode * node);

/**
 *@brief Convert AVL tree to an array.
 *
 * @param tree the AVL tree to be converted
 * @param tree_size the size of the AVL tree
 * @return AVLTreeNode* the result array
 */
static AVLTreeNode * generateAVLTreeNodeArray(AVLTree * tree, int tree_size);

// the index of AVL tree node array when generating
static int generate_AVL_tree_node_array_index = 0;

/**
 *@brief Help to generate AVL tree node array.
 *
 * @param node the current AVL tree node
 * @param node_array the nodes' array
 */
static void generateAVLTreeNodeArrayHelper(AVLTreeNode * node,
                                           AVLTreeNode * node_array);

int calculateCenterStringSpace(char * str)
{
    // calculate center the string need how many space before it
    return abs(MENU_WIDTH - strlen(str)) / 2;
}

void printStringinCenter(char * str)
{
    printf("%*s%s\n", calculateCenterStringSpace(str) - 1, "", str);
    return;
}

void printMenuTitle(char * title)
{
    printMenuFrame('=');
    printStringinCenter(title);
    printMenuFrame('-');

    return;
}

void printMenuFrame(char frame_character)
{
    for (int i = 0; i < MENU_WIDTH; i++)
        putchar(frame_character);
    putchar('\n');
}

void printOptions(char * options[], int options_number)
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

void printMenuFooter(char * footer)
{
    printMenuFrame('-');
    printStringinCenter(footer);
    printMenuFrame('=');
}

void showMainMenu()
{
    clearScreen();
    printMenuTitle("Main Menu");

    static char * options[MAIN_MENU_OPTIONS_NUMBER] = {
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

    showCategoriesHelper(*list, total_page_number % 1,
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
}

void removeCategory(LinkList * list)
{
    if (emptyLinkList(list)) /* there is no category */
    {
        printf("You don't have any category! "
               "Press Enter to continue...");
        eatLine();
        return;
    }
    char * remove_category_name = NULL;

    while (1)
    {
        clearScreen();
        printRemoveCategoryList(list); /* show all categories at first */
        printf("Please enter the name of the category (quit to quit): ");

        getString(&remove_category_name, CATEGORY_NAME_MAX_LENGTH);

        if (strcmp(remove_category_name, "quit") == 0) /* if the name is quit */
            break;                                     /* jump out the loop */

        // get the status code after remove
        int remove_result =
            removeCategoryfromLinkList(list, remove_category_name);

        if (remove_result == FAILED_ALLOCATE_MEMORY) /* can't allocate memory */
        {
            free(remove_category_name); /* free space */
            exit(EXIT_FAILURE);         /* terminate program */
        }

        // the category does not exist
        if (remove_result == NODE_DOES_NOT_EXIST)
        {
            // ask user whether to enter a new category or not, default
            // is yes
            printf("The category \"%s\" does not exist! "
                   "Enter another one? [Y/n] ",
                   remove_category_name);

            if (tolower(getchar()) == 'n')
                break;
            else
            {
                free(remove_category_name); /* free the space */
                continue;
            }
        }
        else /* remove the category successfully */
        {
            printf("You have removed the category \"%s\"!\n",
                   remove_category_name);
            printf("Press Enter to continue...");
            eatLine();
            break;
        }
    }

    free(remove_category_name);
    return;
}

void printRemoveCategoryList(LinkList * list)
{
    printMenuTitle("Category");
    for (LinkListNode * iter = *list; iter; iter = iter->next)
        printStringinCenter(iter->category_item.category_name);
    printMenuFooter(POWER_FOOTER);

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
            printf("The category name is illegal! "
                   "Press Enter to continue...");
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
            printf("The category \"%s\" exists or has illegal "
                   "character!\n",
                   add_category_name);
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
            printf("Press Enter to continue...");
            eatLine();
            free(add_category_name);
            return;
        }
    }

    return;
}

void showItems(LinkList * list)
{
    int total_page = getLinkListSize(list);

    if (total_page == 0) /* no category */
    {
        clearScreen(); /* clear screen at first */

        printMenuTitle("Item");
        printStringinCenter("No items!");
        printMenuFooter("Press Enter to continue...");

        eatLine();

        return;
    }

    showItemsHelper(*list, 1, total_page);

    return;
}

void addItem(LinkList * list)
{
    DateInformation produce_date = getToday(); /* set produce date to today */
    DateInformation due_date =
        makeDateInformation(0, 0, 0);         /* set due date to empty */
    char * category_name = NULL;              /* category's name */
    char * item_name = NULL;                  /* item's name */
    double price = (double) 0;                /* item's price */
    char date_string[DATE_STRING_MAX_LENGTH]; /* string to get date from user */

    // temp data
    DateInformation temp_date;
    Item temp_item;
    int temp_year = 0, temp_month = 0, temp_day = 0;

    while (1)
    {
        printf("Please enter the category's name (quit to quit): ");
        getString(&category_name, CATEGORY_NAME_MAX_LENGTH);

        if (strcmp("quit", category_name) == 0)
        {
            free(category_name); /* free space */
            return;
        }

        // the category does not exist
        if (!findCategoryinLinkList(list, category_name))
        {
            printf("The category \"%s\" does not exist! "
                   "Enter another category? [Y/n] ",
                   category_name);

            free(category_name); /* free space */

            // get user's choice and judge
            if (tolower(getchar()) == 'n')
                return;
        }
        else
            break;
    }

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
    int status =
        addItemstoCategory(list,
                           temp_item = makeItem(category_name, item_name, price,
                                                produce_date, due_date),
                           CREATE_NEW_CATEGORY);

    if (status == FAILED_TO_ADD) /* failed to add the item */
        printf("Failed to add the item! ");
    else
    {
        puts("Add item successfully!\n");

        // show item's information after being added
        showItemInformation(&temp_item);
    }

    printf("Press Enter to continue...");
    eatLine();

    return;
}

void removeItem(LinkList * list)
{
    if (getNumberofItems(list) == 0) /* there is no item */
    {
        // print message
        printf("No items! Press Enter to continue...");
        eatLine();

        return; /* termiante the remove process */
    }

    char * remove_item_name = NULL;

    // get item's name
    printf("Please enter the item's name (quit to quit): ");
    while (getString(&remove_item_name, ITEM_NAME_MAX_LENGTH))
    {
        if (strcmp("quit", remove_item_name) == 0) /*  quit */
            break;

        // find how many item have the same name in the whole system
        int item_number = findIteminLinkList(list, remove_item_name);
        if (item_number == 0) /* the item's name does not exist */
        {
            printf("The item \"%s\" does not exist! Enter another one? "
                   "[Y/n] ",
                   remove_item_name);
            if (tolower(getchar()) == 'n')
                break;
            else
            {
                printf("Please enter the item's name (quit to "
                       "quit): ");

                // free memory space before next entering
                free(remove_item_name);
                continue;
            }
        }

        // there is only one item has this name
        if (item_number == 1)
        {
            // remove directly
            if (removeItemfromCategoryLinkList(list, remove_item_name))
                printf("Remove successfully! Press Enter to continue...");
            else
                printf("Fail to remove! Press Enter to continue...");
            eatLine();
            break;
        }

        // there are duplicate items have the same name
        if (item_number > 1)
        {
            // ask for the category
            char * remove_item_category_name = NULL;
            printf("Please enter the category of \"%s\" (quit to quit): ",
                   remove_item_name);

            while (
                getString(&remove_item_category_name, CATEGORY_NAME_MAX_LENGTH))
            {
                LinkListNode * temp_node = NULL;

                if (strcmp(remove_item_category_name, "quit") == 0) /* quit */
                    break;

                // the category does not exist in the link list
                if (!(temp_node = findCategoryinLinkList(
                          list, remove_item_category_name)))
                    printf("The category \"%s\" does not exist! ",
                           remove_item_category_name);
                // the category's name is incorrect
                else if (!findIteminAVLTree(&temp_node->category_item.item_tree,
                                            remove_item_name))
                    printf("The category \"%s\" does not have item "
                           "\"%s\"! ",
                           remove_item_category_name, remove_item_name);
                // get the correct category name
                else
                {
                    // remove the item
                    removeItemfromAVLTree(&temp_node->category_item.item_tree,
                                          remove_item_name);
                    printf("Remove successfully! Press Enter "
                           "to continue...");
                    eatLine();

                    break;
                }

                // handle exceptions
                printf("Enter another one? [Y/n] ");
                if (tolower(getchar()) == 'n')
                    break;
                else
                {
                    printf("Please enter the category of \"%s\" "
                           "(quit to quit): ",
                           remove_item_name);
                    free(remove_item_category_name); /* free space */
                    continue;
                }
            }
            free(remove_item_category_name); /* free memory space */
            break;
        }
    }

    free(remove_item_name); /* free space */
    return;
}

void editItem(LinkList * list)
{
    char * edit_item_category_name = NULL;
    LinkListNode * category_position = NULL;

    printf("Please enter the item's category (quit to quit): ");

    while (getString(&edit_item_category_name, CATEGORY_NAME_MAX_LENGTH))
    {
        if (strcmp(edit_item_category_name, "quit") == 0) /* quit */
        {
            free(edit_item_category_name); /* free space */
            return;
        }

        // find the category
        category_position =
            findCategoryinLinkList(list, edit_item_category_name);

        if (!category_position) /* the category does not exist */
        {
            printf(
                "The category \"%s\" does not exist! Enter another one? [Y/n] ",
                edit_item_category_name);

            free(edit_item_category_name); /* free space */

            if (tolower(getchar()) == 'n')
                return;

            printf("Please enter the item's category (quit to quit): ");
        }
        else
            break;
    }

    free(edit_item_category_name); /* free the space */

    // clear the screen and show all items in this category
    clearScreen();
    puts("Items:");
    showAVLTree(&category_position->category_item.item_tree);

    // get the item to edit
    printf("Please enter the item you want to edit: ");
    while (getString(&edit_item_category_name, ITEM_NAME_MAX_LENGTH))
    {
        // find the item's position in AVL tree
        AVLTreeNode * item_position =
            findIteminAVLTree(&category_position->category_item.item_tree,
                              edit_item_category_name);

        if (!item_position) /* the item does not exist */
        {
            printf("The item \"%s\" does not exist! Enter another one? [Y/n] ",
                   edit_item_category_name);

            free(edit_item_category_name); /* free space before continue */
            if (tolower(getchar()) == 'n')
                return;

            printf("Please enter the item you want to edit: ");
            continue;
        }
        else /* find the item */
        {
            free(edit_item_category_name); /* free the space at first */

            // show the item's information at first
            showItemInformation(&item_position->item);

            // back up original data
            double price = item_position->item.price;
            DateInformation produce_date = item_position->item.produce_date;
            DateInformation due_date = item_position->item.due_date;

            // record whether the item is changed
            bool changed = false;

            // the new item will be added after the process
            Item new_item = item_position->item;

            // temp data
            char * temp_string = NULL;
            char date_string[DATE_STRING_MAX_LENGTH]; /* get date information */
            int temp_year = 0, temp_month = 0, temp_day = 0;
            DateInformation temp_date;

            printf("Please enter the new category (leave blank will change "
                   "nothing): ");
            getString(&temp_string, CATEGORY_NAME_MAX_LENGTH);
            if (legalString(temp_string)) /* the string is valid */
            {
                changed = true; /* update change status */

                // assign the category name
                new_item.category = staticString2dynamicString(temp_string);
            }
            else
                puts("The string is illegal! The category does not change!");

            free(temp_string); /* free space */

            printf("Please enter the new item's name (leave blank will change "
                   "nothing): ");
            getString(&temp_string, ITEM_NAME_MAX_LENGTH);
            if (legalString(temp_string))
            {
                changed = true; /* update status */

                // assign the name
                new_item.name = staticString2dynamicString(temp_string);
            }
            else
                puts("The string is illegal! The item's name does not change!");

            free(temp_string); /* free space */

            printf(
                "Please enter the new price (leave blank will do nothing): ");
            getString(&temp_string, PRICE_STRING_MAX_LENGTH);

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
                     !validDate(temp_date = makeDateInformation(
                                    temp_year, temp_month, temp_day)))
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
                     !validDate(temp_date = makeDateInformation(
                                    temp_year, temp_month, temp_day)))
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
                    &category_position->category_item.item_tree,
                    item_position->item.name);

                // insert the new item
                insertAVLTreeNode(&category_position->category_item.item_tree,
                                  new_item);

                // message user
                printf("The item has been updated! Press Enter to continue...");
            }
            else
                printf(
                    "There is nothing to change! Press Enter to continue...");

            eatLine();
            break;
        }
    }

    return;
}

void showCategoriesHelper(LinkListNode * current, int current_page_number,
                          int total_page_number)
{
    // show current page
    clearScreen();
    printShowCategories(current, current_page_number, total_page_number);

    int choice = 0; /* get user's choice */

    while ((choice = getchar()) != 'q') /* the choice is not quit */
    {
        LinkListNode * iter = current;
        int counter = 1;

        if (choice == 'p' || choice == 'n') /* legal option */
        {
            // judge whether the next page or the previous page exist
            for (; iter && counter <= CAGEGORY_NUMBER_PER_PAGE;
                 iter = (choice == 'p' ? iter->previous : iter->next))
                counter++;
            if (!iter) /* the page does not exist, skip */
                continue;
            else /* the page exist, go to the page */
                return showCategoriesHelper(iter,
                                            (choice == 'p'
                                                 ? (current_page_number - 1)
                                                 : (current_page_number + 1)),
                                            total_page_number);
        }

        clearScreen();
        printShowCategories(current, current_page_number, total_page_number);
    }
    return;
}

void printShowItems(char * category_name, AVLTreeNode * node_array,
                    int array_size, int current_page, int total_page)
{
    clearScreen(); /* clear screen at first */

    char item_name[ITEM_NAME_MAX_LENGTH]; /* store item's name */

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
            // show items
            sprintf(item_name, "[%d] %s", i + 1, node_array[i].item.name);
            printStringinCenter(item_name);
        }

    printStringinCenter("[q] quit"); /* print quit option */
    printMenuFooter(footer);         /* print footer */

    printf("Enter your choice: "); /* ask user to enter choice */
    return;
}

void showItemsHelper(LinkListNode * current, int current_page_number,
                     int total_page_number)
{
    // get the size of the tree
    int tree_size = getAVLTreeSize(&current->category_item.item_tree);

    // generate the tree node array
    AVLTreeNode * tree_node_array =
        generateAVLTreeNodeArray(&current->category_item.item_tree, tree_size);

    // show current page at first
    printShowItems(current->category_item.category_name, tree_node_array,
                   tree_size, current_page_number, total_page_number);

    int choice = 0;                     /* user's choice */
    while ((choice = getchar()) != 'q') /* choice is not quit */
    {
        eatLine(); /* avoid multiple characters */

        if (choice == 'p' && current_page_number > 1) /* previous page */
        {
            free(tree_node_array); /* free space before returning */
            return showItemsHelper(current->previous, current_page_number - 1,
                                   total_page_number);
        }

        if (choice == 'n' &&
            current_page_number < total_page_number) /* next page */
        {
            free(tree_node_array); /* free space before returning */
            return showItemsHelper(current->next, current_page_number + 1,
                                   total_page_number);
        }

        // available choice to see item's information
        if (isdigit(choice) && 1 <= (choice - '0') &&
            (choice - '0') <= tree_size)
        {
            // show item's information
            showItemInformation(&tree_node_array[choice - '0' - 1].item);
            printf("Press Enter to continue..."); /* ask user to continue */
            eatLine();
        }

        // invalid option, then show the same page
        printShowItems(current->category_item.category_name, tree_node_array,
                       tree_size, current_page_number, total_page_number);
    }

    free(tree_node_array); /* free memory space */
    return;
}

void printAVLTreeNodeCenterHelper(AVLTreeNode * node)
{
    // print item's name in the center of the menu
    printStringinCenter(node->item.name);
    return;
}

AVLTreeNode * generateAVLTreeNodeArray(AVLTree * tree, int tree_size)
{
    // allocate memory for the tree node array
    AVLTreeNode * tree_node_array =
        (AVLTreeNode *) malloc(tree_size * sizeof(AVLTreeNode));
    generate_AVL_tree_node_array_index = 0; /* reset the index to 0 */

    // start generating
    generateAVLTreeNodeArrayHelper(*tree, tree_node_array);

    return tree_node_array; /* return the array */
}

void generateAVLTreeNodeArrayHelper(AVLTreeNode * node,
                                    AVLTreeNode * node_array)
{
    if (!node) /* the node is NULL */
        return;

    // traverse left child node at first
    generateAVLTreeNodeArrayHelper(node->left, node_array);

    // assign the node
    *(node_array + generate_AVL_tree_node_array_index++) = *node;

    // traverse right child node
    generateAVLTreeNodeArrayHelper(node->right, node_array);

    return;
}