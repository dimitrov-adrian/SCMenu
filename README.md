# Simple C Menu
Simple menu library for your C++ projects.
Main goal is to be Arduino compatible and help developing projects with displays,\
but it could be used in any c++ project as it is independent from any Arduino libraries.

## Requirements
C++11 (optional)
c++03 (stdlib.h)

## Example
```c++
#include "SCMenu.h"

// Define new menu
SCMenu mainmenu;

// Function callback that will be used when start rendering the menu.
void display_menu_before() {
  system("clear");
  std::cout<<"- Menu -------";
}

// Function callback that will be used when ending rendering the menu.
void display_menu_after() {
	std::flush(std::cout);
  std::cout<<"\n--------------";
}

// Function callback that will be used when rendering single menu item.
void display_menu_item(SCMenuItem *item) {
  std::cout<<"\n"<<(item->isFocused() ? "*" : " ")<<( item->hasChildrens() ? "+" : " ")<<item->getLabel();
}

// Function callback that will be used when some menu item is selected.
void on_select(SCMenuItem *item) {
  std::cout<<"\n[Selected "<<item->getLabel()<<" ]";
	std::flush(std::cout);
	usleep(700000);
}

int main() {

  // Add menu items.

  mainmenu.addItem((char*)"Item 1");

  mainmenu.addItem((char*)"Item 2");

  // mainmenu.addItem() returns pointer to newly created menu item
  // which we will use to create sub-menu
  SCMenuItem* MENU_ITEM_3 = mainmenu.addItem((char*)"Item 3");

  // Add menu item as sub-menu item in MENU_ITEM_3
  mainmenu.addItem((char*)"Item 3.1", MENU_ITEM_3);
  mainmenu.addItem((char*)"Item 3.2", MENU_ITEM_3);

  SCMenuItem* MENU_ITEM_3_3 = mainmenu.addItem((char*)"Item 3.3", MENU_ITEM_3);
  mainmenu.addItem((char*)"Item 3.3.1", MENU_ITEM_3_3);
  mainmenu.addItem((char*)"Item 3.3.2", MENU_ITEM_3_3);

  mainmenu.addItem((char*)"Item 4");

  SCMenuItem* MENU_ITEM_5 = mainmenu.addItem((char*)"Item 5");
  mainmenu.addItem((char*)"Item 5.1", MENU_ITEM_5);
  mainmenu.addItem((char*)"Item 5.2", MENU_ITEM_5);

  // Set callbacks.
  mainmenu.setRendererMenuBefore(&display_menu_before);
  mainmenu.setRendererMenuAfter(&display_menu_after);
  mainmenu.setRendererMenuItem(&display_menu_item);
  mainmenu.setSelectEventListener(&on_select);

  // Render menu.
	mainmenu.render();

  char c;
  do {
    c = getch();
    if (c == '[') {
      mainmenu.prev(true);
      mainmenu.render();
    }
    else if (c == ']') {
      mainmenu.next(true);
      mainmenu.render();
    }
    else if (c == 10) {
      mainmenu.select();
      mainmenu.render();
    }
    else if (c == 27) {
      mainmenu.back();
			mainmenu.render();
    }
  } while (true);

  return 0;
}
```
