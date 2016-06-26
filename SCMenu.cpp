/*
 * SCMenu - Simple C Menu
 * ======================
 * https://github.com/dimitrov-adrian/scmenu
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Adrian Dimitrov
 */

#include "SCMenu.h"

/**
 * Get the parent element
 *
 * @return parent element
 */
SCMenuItem* SCMenuItem::getParent() {
  return _parent;
}

/**
 * Check if current menu item has parent element
 *
 * @return true if has parent
 */
bool SCMenuItem::hasParent() {
  return _parent != nullptr_t;
}

/**
 * Get menu item label
 *
 * @return label
 */
char* SCMenuItem::getLabel() {
  return _label;
}

/**
 * Set label for the menu item
 *
 * @param label
 */
void SCMenuItem::setLabel(char* label) {
  _label = label;
}

/**
 * Check if current menu item has childrens
 *
 * @return true if has childrens
 */
bool SCMenuItem::hasChildrens() {
  return _hasChildrens;
}

/**
 * Check if current menu item is on focus
 *
 * @return true if focused
 */
bool SCMenuItem::isFocused() {
  return _isFocused;
}

/**
 * Check if current menu item is selected
 *
 * @return true if selected
 */
bool SCMenuItem::isSelected() {
  return _isSelected;
}

/**
 * Check if the menu item is hidden
 *
 * @return true if is hidden
 */
bool SCMenuItem::isHidden() {
  return _hidden;
}

/**
 * Hide menu item
 */
void SCMenuItem::hide() {
  _hidden = true;
}

/**
 * Show menu item
 */
void SCMenuItem::show() {
  _hidden = false;
}

/**
 * SCMenu constructor
 */
SCMenu::SCMenu() {
  _itemsLen = 0;
  _rendererBefore = nullptr_t;
  _rendererAfter = nullptr_t;
  _rendererMenuItem = nullptr_t;
  _selectListener = nullptr_t;
  _items = (SCMenuItem*)(malloc(sizeof(SCMenuItem)));
  if (_items == nullptr_t) {
    free(_items);
  }
  SCMenu::reset();
}

/**
 * SCMenu destructor
 */
SCMenu::~SCMenu() {
  free(_items);
}

/**
 * Reset focus and selections
 */
void SCMenu::reset() {
  _focusedItemIndex = 0;
  _selectedItemIndex = 0;
  _hasSelection = false;
  for (scmenu_index i = 0; i < _itemsLen; i++) {
    _items[i]._isFocused = false;
  }
}

/**
 * Add item to menu
 *
 * @param label
 * @param reference to parent menu item
 *
 * @return pointer to newly created menu item
 */
SCMenuItem* SCMenu::addItem(char* label, SCMenuItem* parentItem) {
  SCMenuItem *items_new;
  items_new = (SCMenuItem*)realloc(_items, sizeof(SCMenuItem)*(_itemsLen+1));
  if (items_new == nullptr_t) {
    free(items_new);
    return nullptr_t;
  }
  else {
    _items[_itemsLen]._label = label;
    _items[_itemsLen]._parent = parentItem;
    _items[_itemsLen]._isFocused = _itemsLen == 0 ? true : false;
    _items[_itemsLen]._isSelected = false;
    _items[_itemsLen]._hasChildrens = false;
    _items[_itemsLen]._hidden = false;
    if (parentItem != nullptr_t) {
      parentItem->_hasChildrens = true;
    }
    _itemsLen++;
    return &_items[_itemsLen-1];
  }
}

/**
 * Remove menu item from menu
 *
 * @param menu item reference
 *
 * @return true if item is removed
 */
bool SCMenu::removeItem(SCMenuItem *item) {
  if (item->hasChildrens()) {
    SCMenuItem *items_new;
    items_new = (SCMenuItem*)realloc(_items, sizeof(_items));
    if (items_new == nullptr_t) {
      free(items_new);
      return false;
    }
    else {
      for (scmenu_index i = 0; i < _itemsLen; i++) {
        if (item == &_items[i]) {
          delete &_items[i];
        }
      }
      _items = items_new;
    }
    return true;
  }
  else {
    return false;
  }
}

/**
 * Move focus to first item in menu for a current level
 */
void SCMenu::first() {
  bool loop_start_index_found = false;
  for (scmenu_index i = 0; i < _itemsLen; i++) {
    _items[i]._isFocused = false;
    if (!loop_start_index_found) {
      if ((_hasSelection && _items[i]._parent == &_items[_selectedItemIndex]) || (!_hasSelection && _items[i]._parent == nullptr_t)) {
        loop_start_index_found = true;
        _items[i]._isFocused = false;
        _focusedItemIndex = i;
      }
    }
  }
}

/**
 * Move focus to last item in menu for a current level
 */
void SCMenu::last() {
  bool loop_start_index_found = false;
  for (scmenu_index i = _itemsLen-1; i >= 0; i--) {
    _items[i]._isFocused = false;
    if (!loop_start_index_found) {
      if ((_hasSelection && _items[i]._parent == &_items[_selectedItemIndex]) || (!_hasSelection && _items[i]._parent == nullptr_t)) {
        loop_start_index_found = true;
        _items[i]._isFocused = false;
        _focusedItemIndex = i;
      }
    }
  }
}

/**
 * Move focus to previous item
 *
 * @param weither to go in loop not to stop on first element
 *
 * @return true if focus is changed
 */
bool SCMenu::prev(bool loop) {
  bool founded_next = false;
  scmenu_index old_focus = _focusedItemIndex;
  scmenu_index loop_start_index = 0;
  bool loop_start_index_found = false;
  for (scmenu_index i = _itemsLen-1; i >= 0; i--) {
    _items[i]._isFocused = false;
    if ((_hasSelection && _items[i]._parent == &_items[_selectedItemIndex]) || (!_hasSelection && _items[i]._parent == nullptr_t)) {
      if (!loop_start_index_found) {
        loop_start_index = i;
        loop_start_index_found = true;
      }
      if (i < _focusedItemIndex && !founded_next) {
        _items[i]._isFocused = true;
        founded_next = true;
        _focusedItemIndex = i;
      }
    }
  }
  if (!founded_next) {
    if (loop) {
      _focusedItemIndex = loop_start_index;
      _items[_focusedItemIndex]._isFocused = true;
      return true;
    }
    else {
      _focusedItemIndex = old_focus;
      _items[_focusedItemIndex]._isFocused = true;
      return false;
    }
  }
  return true;
}

/**
 * Move focus to next item
 *
 * @param weither to go in loop not to stop on last element
 *
 * @return true if focus is changed
 */
bool SCMenu::next(bool loop) {
  bool founded_next = false;
  scmenu_index old_focus = _focusedItemIndex;
  scmenu_index loop_start_index = 0;
  bool loop_start_index_found = false;
  for (scmenu_index i = 0; i < _itemsLen; i++) {
    _items[i]._isFocused = false;
    if ((_hasSelection && _items[i]._parent == &_items[_selectedItemIndex]) || (!_hasSelection && _items[i]._parent == nullptr_t)) {
      if (!loop_start_index_found) {
        loop_start_index = i;
        loop_start_index_found = true;
      }
      if (i > _focusedItemIndex && !founded_next) {
        _items[i]._isFocused = true;
        founded_next = true;
        _focusedItemIndex = i;
      }
    }
  }
  if (!founded_next) {
    if (loop) {
      _focusedItemIndex = loop_start_index;
      _items[_focusedItemIndex]._isFocused = true;
      return true;
    }
    else {
      _focusedItemIndex = old_focus;
      _items[_focusedItemIndex]._isFocused = true;
      return false;
    }
  }
  return true;
}

/**
 * Select current focused menu item
 *
 * @param menu item will be selected until back action is emitted.
 */
void SCMenu::select(bool until_back) {
  _hasSelection = true;
  _selectedItemIndex = _focusedItemIndex;
  for (scmenu_index i = 0; i < _itemsLen; i++) {
    _items[i]._isSelected = (_selectedItemIndex == i);
  }
  if (_selectListener != nullptr_t) {
    (_selectListener)(&_items[_selectedItemIndex]);
  }
  if (_items[_selectedItemIndex].hasChildrens()) {
    SCMenu::render();
    SCMenu::first();
  }
  else if (until_back) {
    SCMenu::back();
  }
}

/**
 * Go back to parent element
 *
 * @return if has parent element and is selected
 */
bool SCMenu::back() {
  if (_hasSelection) {
    _focusedItemIndex = _selectedItemIndex;
    _hasSelection = _items[_selectedItemIndex]._parent ? true : false;
    for (scmenu_index i = 0; i < _itemsLen; i++) {
      if (_hasSelection && _items[_selectedItemIndex]._parent == &_items[i]) {
        _selectedItemIndex = i;
      }
      _items[i]._isSelected = (_selectedItemIndex == i);
      _items[i]._isFocused = (_focusedItemIndex == i);
    }
    return true;
  }
  return false;
}

/**
 * Check if in menu has selected menu item
 *
 * @return bool
 */
bool SCMenu::hasSelection() {
  return _hasSelection;
}

/**
 * Get current selected menu item
 *
 * @return reference to menu item
 */
SCMenuItem* SCMenu::getSelected() {
  if (_hasSelection) {
    return &_items[_selectedItemIndex];
  }
  return nullptr_t;
}

/**
 * Render the menu
 */
void SCMenu::render() {

  // Execute callback before menu items are rendered.
  if (_rendererBefore != nullptr_t) {
    (_rendererBefore)();
  }

  // Actual menu item rendering, skip hidden menu items.
  if (_rendererMenuItem != nullptr_t) {
    for (scmenu_index i = 0; i < _itemsLen; i++) {
      if (!_hasSelection && !_items[i].isHidden() && !_items[i]._parent && _items[i]._parent == &_items[_selectedItemIndex]) {
        (_rendererMenuItem)(&_items[i]);
      }
    }
  }

  // Execute callback after menu items are rendered.
  if (_rendererAfter != nullptr_t) {
    (_rendererAfter)();
  }
}

/**
 * Set event listener callback triggered when menu item is selected
 */
void SCMenu::setSelectEventListener(SCMenuItemCallback callback) {
  _selectListener = callback;
}

/**
 * Set before menu render renderer callback
 *
 * @param render callback function
 */
void SCMenu::setRendererBefore(SCMenuCallback callback) {
  _rendererBefore = callback;
}

/**
 * Set after menu render renderer callback
 *
 * @param render callback function
 */
void SCMenu::setRendererAfter(SCMenuCallback callback) {
  _rendererAfter = callback;
}

/**
 * Set menu item renderer callback
 *
 * @param render callback function
 */
void SCMenu::setRendererMenuItem(SCMenuItemCallback callback) {
  _rendererMenuItem = callback;
}
