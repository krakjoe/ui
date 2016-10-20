/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe                                                      |
  +----------------------------------------------------------------------+
*/
#ifndef HAVE_PHP_UI_MENU
#define HAVE_PHP_UI_MENU
#include <ui.h>

#include "php.h"

#include <classes/menu.h>
#include <classes/item.h>

zend_object_handlers php_ui_menu_handlers;

zend_object* php_ui_menu_create(zend_class_entry *ce) {
	php_ui_menu_t *menu = 
		(php_ui_menu_t*) ecalloc(1, sizeof(php_ui_menu_t) + zend_object_properties_size(ce));

	zend_object_std_init(&menu->std, ce);

	object_properties_init(&menu->std, ce);

	menu->std.handlers = &php_ui_menu_handlers;

	return &menu->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_menu_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Menu Menu::__construct(string name) */
PHP_METHOD(Menu, __construct) 
{
	php_ui_menu_t *menu = php_ui_menu_fetch(getThis());
	zend_string *name = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &name) != SUCCESS) {
		return;
	}

	menu->m = uiNewMenu(ZSTR_VAL(name));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_menu_append_info, 0, 0, IS_OBJECT, "UI\\MenuItem", 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Menu Menu::append(string name) */
PHP_METHOD(Menu, append) 
{
	php_ui_menu_t *menu = php_ui_menu_fetch(getThis());
	zend_string *name = NULL;
	php_ui_item_t *item = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &name) != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiItem_ce);

	item = php_ui_item_fetch(return_value);

	item->i = uiMenuAppendItem(menu->m, ZSTR_VAL(name));
} /* }}} */

/* {{{ proto Menu Menu::appendCheck(string name) */
PHP_METHOD(Menu, appendCheck) 
{
	php_ui_menu_t *menu = php_ui_menu_fetch(getThis());
	zend_string *name = NULL;
	php_ui_item_t *item = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &name) != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiItem_ce);

	item = php_ui_item_fetch(return_value);

	item->i = uiMenuAppendCheckItem(menu->m, ZSTR_VAL(name));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_menu_append_anon_info, 0, 0, IS_OBJECT, "UI\\MenuItem", 0)
ZEND_END_ARG_INFO()

/* {{{ proto Menu Menu::appendQuit(void) */
PHP_METHOD(Menu, appendQuit) 
{
	php_ui_menu_t *menu = php_ui_menu_fetch(getThis());
	php_ui_item_t *item = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiItem_ce);

	item = php_ui_item_fetch(return_value);

	item->i = uiMenuAppendQuitItem(menu->m);
} /* }}} */

/* {{{ proto Menu Menu::appendPreferences(void) */
PHP_METHOD(Menu, appendPreferences) 
{
	php_ui_menu_t *menu = php_ui_menu_fetch(getThis());
	php_ui_item_t *item = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiItem_ce);

	item = php_ui_item_fetch(return_value);

	item->i = uiMenuAppendPreferencesItem(menu->m);
} /* }}} */

/* {{{ proto Menu Menu::appendAbout(void) */
PHP_METHOD(Menu, appendAbout) 
{
	php_ui_menu_t *menu = php_ui_menu_fetch(getThis());
	php_ui_item_t *item = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiItem_ce);

	item = php_ui_item_fetch(return_value);

	item->i = uiMenuAppendAboutItem(menu->m);
} /* }}} */
ZEND_BEGIN_ARG_INFO_EX(php_ui_menu_append_separator_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Menu::appendSeparator(void) */
PHP_METHOD(Menu, appendSeparator) 
{
	php_ui_menu_t *menu = php_ui_menu_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiMenuAppendSeparator(menu->m);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_menu_methods[] = {
	PHP_ME(Menu, __construct,        php_ui_menu_construct_info,        ZEND_ACC_PUBLIC)
	PHP_ME(Menu, append,             php_ui_menu_append_info,           ZEND_ACC_PUBLIC)
	PHP_ME(Menu, appendCheck,        php_ui_menu_append_info,           ZEND_ACC_PUBLIC)
	PHP_ME(Menu, appendQuit,         php_ui_menu_append_anon_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Menu, appendPreferences,  php_ui_menu_append_anon_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Menu, appendAbout,        php_ui_menu_append_anon_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Menu, appendSeparator,    php_ui_menu_append_separator_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Menu) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Menu", php_ui_menu_methods);

	uiMenu_ce = zend_register_internal_class(&ce);
	uiMenu_ce->create_object = php_ui_menu_create;

	memcpy(&php_ui_menu_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_menu_handlers.offset = XtOffsetOf(php_ui_menu_t, std);

	return SUCCESS;
} /* }}} */
#endif
