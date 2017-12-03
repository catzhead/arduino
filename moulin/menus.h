#ifndef __MENUS__
#define __MENUS__

#define LAST_MENU_INDEX 1

typedef void (* fptr)(); 
extern const fptr menus[LAST_MENU_INDEX+1];

#endif
