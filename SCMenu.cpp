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
  std::cout<<"\n"<<sizeof(SCMenuItem)<<"\n";
  if (_items == nullptr_t) {
    //free(_items);
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
  for (int i = 0; i < _itemsLen; i++) {
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
SCMenuItem* SCMenu::addItem(char* label, SCMenuItem* parentItem = nullptr_t) {
  SCMenuItem *itemsNew;
  itemsNew = (SCMenuItem*)realloc(_items, sizeof(SCMenuItem)*(_itemsLen+1));
  if (itemsNew == nullptr_t) {
    free(itemsNew);
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
 * Move focus to previous item
 *
 * @param weither to go in loop not to stop on first element
 *
 * @return true if focus is changed
 */
bool SCMenu::prev(bool loop = false) {
  bool founded_next = false;
  int old_focus = _focusedItemIndex;
  int loop_start_index = 0;
  bool loop_start_index_found = false;
  for (int i = _itemsLen-1; i >= 0; i--) {
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
bool SCMenu::next(bool loop = false) {
  bool founded_next = false;
  int old_focus = _focusedItemIndex;
  int loop_start_index = 0;
  bool loop_start_index_found = false;
  for (int i = 0; i < _itemsLen; i++) {
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
 */
void SCMenu::select() {
  _hasSelection = true;
  _selectedItemIndex = _focusedItemIndex;
  for (int i = 0; i < _itemsLen; i++) {
    _items[i]._isSelected = (_selectedItemIndex == i);
  }
  if (_selectListener != nullptr_t) {
    (_selectListener)(&_items[_selectedItemIndex]);
  }
  if (_items[_selectedItemIndex].hasChildrens()) {
    SCMenu::render();
    next();
  }
  else {
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
    for (int i = 0; i < _itemsLen; i++) {
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
 * Render the menu
 */
void SCMenu::render() {
  if (_rendererBefore != nullptr_t) {
    (_rendererBefore)();
  }

  if (_rendererMenuItem != nullptr_t) {
    for (int i = 0; i < _itemsLen; i++) {
      if (!_items[i].isHidden()) {
        if (!_hasSelection && !_items[i]._parent) {
          (_rendererMenuItem)(&_items[i]);
        }
        else if (_hasSelection && _items[i]._parent == &_items[_selectedItemIndex]) {
          (_rendererMenuItem)(&_items[i]);
        }
      }
    }
  }

  if (_rendererAfter != nullptr_t) {
    (_rendererAfter)();
  }
}

/**
 * Set event listener callback triggered when menu item is selected
 */
void SCMenu::setSelectListener(SCMenuItemCallback callback) {
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