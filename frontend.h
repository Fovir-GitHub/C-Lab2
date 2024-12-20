#ifndef _FRONTEND_H_
#define _FRONTEND_H_

#include "category.h"

#define MENU_WIDTH 50 /* the width of menu */
#define POWER_FOOTER "Powered by Fovir"

// show how many categories in one page
#define CAGEGORY_NUMBER_PER_PAGE 5

/**
 *@brief Store the result of selecting item or category.
 *
 */
typedef struct
{
    LinkListNode * category_position; /* the position of category */
    AVLTreeNode * item_position;      /* the position of item */
} SelectResult;

/**
 *@brief Construct SelectResult object.
 *
 * @param category_position the pointer to category
 * @param item_position the pointer to item
 * @return SelectResult the result of constructing
 */
SelectResult makeSelectResult(LinkListNode * category_position,
                              AVLTreeNode * item_position);

/**
 *@brief Calculate how many space need for center the string in the menu.
 *
 * @param str the string to calculate center space
 * @return int the prefix space length of the center title
 */
int calculateCenterStringSpace(const char * str);

/**
 *@brief Print a string in the center of the menu line.
 *
 * @param str the string want to print in the center line.
 */
void printStringinCenter(const char * str);

/**
 *@brief Print a menu's title and title frame.
 *
 * @param title the menu's title
 */
void printMenuTitle(const char * title);

/**
 *@brief Print the menu frame with the character.
 *
 * @param frame_character the frame character
 */
void printMenuFrame(char frame_character);

/**
 *@brief Print options to menu. Two options will be in a single line with order
 *number.
 *
 * @param options the array of options
 * @param options_number the size of the options array
 */
void printOptions(const char * options[], int options_number);

/**
 *@brief Print the footer of a menu.
 *
 * @param footer the footer content
 */
void printMenuFooter(const char * footer);

/**
 * Main menu include:
 * 1. show all categories
 * 2. add category
 * 3. remove category
 * 4. show all items
 * 5. show items according categories
 * 6. add item
 * 7. remove item
 * 8. edit item information
 * q. quit
 */

#define MAIN_MENU_OPTIONS_NUMBER 8 /* the number of options of main menu */
#define MAIN_MENU_OPTIONS_STRING \
    "1234567q" /* the options' stirng for main menu */

enum MAIN_MENU_OPTIONS {
    SHOW_ALL_CATEGORIES, /* show all categories */
    ADD_CATEGORY,        /* add category */
    REMOVE_CATEGORY,     /* remove category */
    SHOW_ALL_ITEMS,      /* show all items */
    ADD_ITEM,            /* add item */
    REMOVE_ITEM,         /* remove item */
    EDIT_ITEM,           /* edit item information */
    QUIT                 /* quit */
};

/**
 *@brief Display the main menu
 */
void showMainMenu();

/**
 *@brief Get main menu's choice from user's input.
 *
 * @return int user's choice
 */
int getMainMenuChoicefromUser();

/**
 *@brief Show categories function.
 *
 * @param list the categories' link list
 */
void showCategories(LinkList * list);

void printShowCategories(LinkListNode * current, int current_page_number,
                         int total_page_number);

/**
 *@brief Remove category with user's input.
 *
 * @param list the link list stores categories
 */
void removeCategory(LinkList * list);

/**
 *@brief Print categories in list form.
 *  title
 *  [index1] category name 1
 *  [index2] category name 2
 *  ...
 *  [q] quit
 *  footer
 *
 * @param list the categories' link list
 */
void printCategoryListinListForm(LinkList * list);

/**
 *@brief Add new category from user
 *
 * @param list the target link list
 */
void addCategory(LinkList * list);

/**
 *@brief Show all items according by category.
 *
 * @param list the link list of categories
 */
void showItems(LinkList * list);

/**
 *@brief Add item from user input.
 *
 * @param list the target category link list
 */
void addItem(LinkList * list);

/**
 *@brief Remove an item from user input.
 */
void removeItem(LinkList * list);

/**
 * @brief Edit an item's information in a link list.
 *
 * @param list the category's link list
 */
void editItem(LinkList * list);

/**
 *@brief Get category with user selection.
 *
 * @param list the link list that contains categories
 * @return selectResult the result of selecting category
 */
SelectResult selectCategory(LinkList * list);

/**
 *@brief Convert AVL tree into an array of pointer.
 *
 * @param tree the tree to be converted
 * @return AVLTreeNode** the result
 */
AVLTreeNode ** generateAVLTreeNodePointerArray(AVLTree * tree);

/**
 *@brief This function is used to select a item from a link list.
 *
 * @param list the link list
 * @return the position of the item
 */
SelectResult selectItem(LinkList * list);

#endif // !_FRONTEND_H_