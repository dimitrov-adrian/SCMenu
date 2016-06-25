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

#ifdef nullptr
  #define nullptr_t nullptr
#else
  #define nullptr_t NULL
#endif

/**
 * Early prototypes
 */
class SCMenu;
class SCMenuItem;

/**
 * Typedefs
 */
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
    int _itemsLen;
    int _focusedItemIndex;
    int _selectedItemIndex;
    bool _hasSelection;
    SCMenuItemCallback _selectListener;
    SCMenuCallback _rendererBefore;
    SCMenuCallback _rendererAfter;
    SCMenuItemCallback _rendererMenuItem;
  public:
    SCMenu();
    ~SCMenu();
    void empty();
    void reset();
    void setSelectListener(SCMenuItemCallback);
    void setRendererBefore(SCMenuCallback);
    void setRendererMenuItem(SCMenuItemCallback);
    void setRendererAfter(SCMenuCallback);
    SCMenuItem* addItem(char*, SCMenuItem* = nullptr_t);
    bool prev(bool = false);
    bool next(bool = false);
    void select();
    bool back();
    void render();
};

#endif
