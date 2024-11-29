#ifndef _FRONTEND_H_
#define _FRONTEND_H_

#define MENU_WIDTH 40 /* the width of menu */

/**
 *@brief Center the title.
 *
 * @param title the menu title
 * @return int the prefix space length of the center title
 */
int centerMenuTitle(char * title);

/**
 *@brief Print the menu frame with the character.
 *
 * @param frame_character the frame character
 */
void printMenuFrame(char frame_character);

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

void showMainMenu();

#endif // !_FRONTEND_H_