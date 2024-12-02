#ifndef _FRONTEND_H_
#define _FRONTEND_H_

#define MENU_WIDTH 50 /* the width of menu */
#define POWER_FOOTER "Powered by Fovir"

/**
 *@brief Calculate how many space need for center the string in the menu.
 *
 * @param str the string to calculate center space
 * @return int the prefix space length of the center title
 */
int calculateCenterStringSpace(char * str);

/**
 *@brief Print a string in the center of the menu line.
 *
 * @param str the string want to print in the center line.
 */
void printStringinCenter(char * str);

/**
 *@brief Print a menu's title and title frame.
 *
 * @param title the menu's title
 */
void printMenuTitle(char * title);

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
void printOptions(char * options[], int options_number);

/**
 *@brief Print the footer of a menu.
 *
 * @param footer the footer content
 */
void printMenuFooter(char * footer);

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

#define MAIN_MENU_OPTIONS_NUMBER 9 /* the number of options of main menu */

enum MAIN_MENU_OPTIONS {
    SHOW_ALL_CATEGORIES,           /* show all categories */
    ADD_CATEGORY,                  /* add category */
    REMOVE_CATEGORY,               /* remove category */
    SHOW_ALL_ITEMS,                /* show all items */
    SHOW_ITEMS_ACCORDING_CATEGORY, /* show items according categories */
    ADD_ITEM,                      /* add item */
    REMOVE_ITEM,                   /* remove item */
    EDIT_ITEM,                     /* edit item information */
    QUIT                           /* quit */
};

/**
 *@brief Display the main menu
 */
void showMainMenu();

#define INVALID_CHOICE 0
/**
 *@brief Get the choice from user input. And judge whether the choice is valid.
 *
 * @param options the options' string
 * @return char the user's choice,
 * if the choice is invalid, returns INVALID_CHOICE.
 */
char getChoicefromUser(const char * options);

#endif // !_FRONTEND_H_