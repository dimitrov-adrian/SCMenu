/*
 * SCMenu - Simple C Menu
 * ======================
 * https://github.com/dimitrov-adrian/scmenu
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Adrian Dimitrov
 */

#ifndef _SCMENU_H
#define _SCMENU_H

#include <stdlib.h>

/**
 * Early prototypes
 */
class SCMenu;
class SCMenuItem;

/**
 * Types
 */
#ifdef nullptr
  #define nullptr_t nullptr
#else
  #define nullptr_t NULL
#endif

#ifndef scmenu_index
  #ifdef byte
    typedef unsigned byte scmenu_index;
  #else
    typedef unsigned short int scmenu_index;
  #endif
#endif

typedef void (*SCMenuCallback)();
typedef void (*SCMenuItemCallback)(SCMenuItem*);


/**
 * Menu item structure
 */
class SCMenuItem {
  friend class SCMenu;
  private:
    SCMenuItem* _parent;
    bool _isFocused;
    bool _isSelected;
    bool _hasChildrens;
    char* _label;
    bool _hidden;
  public:
    SCMenuItem* getParent();
    bool hasParent();
    char* getLabel();
    void setLabel(char*);
    bool hasChildrens();
    bool isFocused();
    bool isSelected();
    bool isHidden();
    void hide();
    void show();
};

/**
 * Main menu class
 */
class SCMenu {
  protected:
    SCMenuItem* _items;
    scmenu_index _itemsLen;
    scmenu_index _focusedItemIndex;
    scmenu_index _selectedItemIndex;
    bool _hasSelection;
    SCMenuItemCallback _selectListener;
    SCMenuCallback _rendererMenuBefore;
    SCMenuCallback _rendererMenuAfter;
    SCMenuItemCallback _rendererMenuItem;
  public:
    SCMenu();
    ~SCMenu();
    void empty();
    void reset();
    void setSelectEventListener(SCMenuItemCallback);
    void setRendererMenuBefore(SCMenuCallback);
    void setRendererMenuItem(SCMenuItemCallback);
    void setRendererMenuAfter(SCMenuCallback);
    SCMenuItem* addItem(char*, SCMenuItem* = nullptr_t);
    bool removeItem(SCMenuItem* = nullptr_t);
    void first();
    void last();
    bool prev(bool = false);
    bool next(bool = false);
    void select(bool = true);
    bool back();
    bool hasSelection();
    SCMenuItem* getSelected();
    void render();
};

#endif
