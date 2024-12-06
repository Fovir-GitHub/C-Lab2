#include "frontend.h"
#include "my_utility.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ORDER_NUMBER_WIDTH 4

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
static void printShowItems(LinkListNode * node, int current_page,
                           int total_page);

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
                   temp_string); /* align to left and be sure that the longest
                                    string reaches the right side */
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
            printf("You entered an invalid chioce! Press Enter to continue...");
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

    showCategoriesHelper(*list, 1, total_page_number); /* show the first page */
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
        getchar();
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
            return;

        // get the status code after remove
        int remove_result =
            removeCategoryfromLinkList(list, remove_category_name);

        if (remove_result == FAILED_ALLOCATE_MEMORY) /* can't allocate memory */
            exit(EXIT_FAILURE);

        // the category does not exist
        if (remove_result == NODE_DOES_NOT_EXIST)
        {
            // ask user whether to enter a new category or not, default is yes
            printf("The category \"%s\" does not exist! "
                   "Enter another one? [Y/n] ",
                   remove_category_name);

            int choice = getchar();

            // the choice is default
            if (choice == '\n')
                continue;
            eatLine();

            // the user doesn't want to remove new category
            if (tolower(choice) == 'n')
                break;
        }
        else /* remove the category successfully */
        {
            printf("You have removed the category \"%s\"!\n",
                   remove_category_name);
            printf("Press Enter to continue...");
            getchar();
            break;
        }
    }

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
            return;

        if (!legalString(add_category_name)) /* the name is illegal */
        {
            printf("The category name is illegal! "
                   "Press Enter to continue...");
            getchar();
            continue;
        }

        // get the status code after add to link list
        int add_result = addCategorytoLinkList(list, add_category_name);
        switch (add_result)
        {
        case DUPLICATED_NODE:
        case ILLEGAL_STRING:
            printf("The category \"%s\" exists or has illegal character!\n",
                   add_category_name);
            printf("Enter another category name? [Y/n] ");

            int choice = getchar();     /* get choice */
            if (tolower(choice) == 'n') /* terminate */
                return;
            else
                continue;

        case FAILED_ALLOCATE_MEMORY:
            exit(EXIT_FAILURE); /* terminate the programme */

        case SUCCESS_ADD: /* successfully added the category */
            printf("Successfully add new category \"%s\"\n", add_category_name);
            printf("Press Enter to continue...");
            getchar();
            return;
        }
    }
}

void showItems(LinkList * list)
{
    int total_page = getLinkListSize(list);
    showItemsHelper(*list, 1, total_page);

    return;
}

void addItem(LinkList * list)
{
    DateInformation produce_date = getToday(); /* set produce date to today */
    DateInformation due_date =
        makeDateInformation(0, 0, 0); /* set due date to empty */
    char * category_name = NULL;      /* category's name */
    char * item_name = NULL;          /* item's name */
    double price = (double) 0;        /* item's price */
    char date_string[12];             /* string to get date from user */

    // temp data
    DateInformation temp_date;
    int temp_year = 0, temp_month = 0, temp_day = 0;

    while (1)
    {
        printf("Please enter the category's name (quit to quit): ");
        getString(&category_name, CATEGORY_NAME_MAX_LENGTH);

        if (strcmp("quit", category_name) == 0)
            return;

        // the category does not exist
        if (!findCategoryinLinkList(list, category_name))
        {
            printf("The category \"%s\" does not exist! "
                   "Enter another category? [Y/n] ",
                   category_name);

            // get user's choice and judge
            if (tolower(getchar()) == 'n')
                return;
            else
                continue;
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
            return;

        if (!legalString(item_name)) /* the name is illegal */
        {
            printf("The item name \"%s\" is illegal! Enter another one? [Y/n] ",
                   item_name);

            if (tolower(getchar()) == 'n')
                return;
            else
                continue;
        }

        break;
    }

    // get the price
    printf("Please enter the price: ");
    while (scanf("%lf", &price) != 1)
    {
        printf("Please enter the right form! Enter another price? [Y/n] ");
        if (tolower(getchar()) == 'n')
            return;
        printf("Please enter the price: ");
    }

    // get produce date
    printf("Please enter the produce date in YYYY-MM-DD form (leave blank will "
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
    printf("Please enter the expiration date in YYYY-MM-DD form (leave blank "
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
        list, makeItem(category_name, item_name, price, produce_date, due_date),
        CREATE_NEW_CATEGORY);

    if (status == FAILED_TO_ADD)
        printf("Failed to add the item! ");
    else
        printf("Add item successfully! ");

    printf("Press Enter to continue...");
    getchar();

    return;
}

void removeItem(LinkList * list)
{
    clearScreen();

    char * remove_item_name = NULL;
    char * remove_item_category_name = NULL;

    printf("Please enter the item's name (quit to quit): ");
    while (getString(&remove_item_name, ITEM_NAME_MAX_LENGTH))
    {
        if (strcmp("quit", remove_item_name) == 0)
            return;
        if (!legalString(remove_item_name))
            printf("The item's name is illegal! Enter another one? [Y/n] ");
    }
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

void printShowItems(LinkListNode * node, int current_page, int total_page)
{
    clearScreen(); /* clear screen at first */

    // get footer
    char footer[MENU_WIDTH];
    sprintf(footer, "[p]revious    %d/%d    [n]ext", current_page, total_page);

    printMenuTitle("Item");

    // print category name
    printf("%-*s\n", MENU_WIDTH, node->category_item.category_name);

    // print items' name
    traverseAVLTree(&node->category_item.item_tree,
                    printAVLTreeNodeCenterHelper);
    printStringinCenter("[q] quit"); /* print quit option */
    printMenuFooter(footer);         /* print footer */

    return;
}

void showItemsHelper(LinkListNode * current, int current_page_number,
                     int total_page_number)
{
    // show current page at first
    printShowItems(current, current_page_number, total_page_number);

    int choice = 0;                     /* user's choice */
    while ((choice = getchar()) != 'q') /* choice is not quit */
    {
        if (choice == 'p' && current_page_number > 1) /* previous page */
            return showItemsHelper(current->previous, current_page_number - 1,
                                   total_page_number);

        if (choice == 'n' &&
            current_page_number < total_page_number) /* next page */
            return showItemsHelper(current->next, current_page_number + 1,
                                   total_page_number);

        // invalid option, then show the same page
        printShowItems(current, current_page_number, total_page_number);
    }

    return;
}

void printAVLTreeNodeCenterHelper(AVLTreeNode * node)
{
    // print item's name in the center of the menu
    printStringinCenter(node->item.name);
    return;
}
