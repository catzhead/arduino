#ifndef __MENUS__
#define __MENUS__

#define LAST_MENU_INDEX 6

typedef void (* fptr)(); 
extern const fptr menu[LAST_MENU_INDEX+1];

typedef void (* fptr_int)(int); 
extern const fptr_int menu_buttons[LAST_MENU_INDEX+1];

#endif
