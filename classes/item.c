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
#ifndef HAVE_PHP_UI_ITEM
#define HAVE_PHP_UI_ITEM
#include <ui.h>

#include "php.h"

#include <classes/window.h>
#include <classes/item.h>

zend_object_handlers php_ui_item_handlers;

zend_class_entry *uiItem_ce;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);
extern int php_ui_call(zend_fcall_info *fci, zend_fcall_info_cache *fcc);

void php_ui_item_click_handler(uiMenuItem *i, uiWindow *w,  void *_item) {
	php_ui_item_t *item = (php_ui_item_t*) _item;

	if (item->click.fci.size) {
		zval rv;
		zval window;

		php_ui_window_construct(&window, w);
		
		ZVAL_UNDEF(&rv);

		zend_fcall_info_argn(&item->click.fci, 1, &window);
		item->click.fci.retval = &rv;

		if (zend_call_function(&item->click.fci, &item->click.fcc) != SUCCESS) {
			return;
		}

		zend_fcall_info_args_clear(&item->click.fci, 1);

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}

		zval_ptr_dtor(&window);
	}
}

zend_object* php_ui_item_create(zend_class_entry *ce) {
	php_ui_item_t *item = 
		(php_ui_item_t*) ecalloc(1, sizeof(php_ui_item_t) + zend_object_properties_size(ce));

	zend_object_std_init(&item->std, ce);

	object_properties_init(&item->std, ce);

	item->std.handlers = &php_ui_item_handlers;

	php_ui_set_call(&item->std, ZEND_STRL("onclick"), &item->click.fci, &item->click.fcc);

	return &item->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_item_enable_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void MenuItem::enable(void) */
PHP_METHOD(MenuItem, enable) 
{
	php_ui_item_t *item = php_ui_item_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiMenuItemEnable(item->i);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_item_disable_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void MenuItem::disable(void) */
PHP_METHOD(MenuItem, disable) 
{
	php_ui_item_t *item = php_ui_item_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiMenuItemDisable(item->i);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_item_is_checked_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool MenuItem::isChecked(void) */
PHP_METHOD(MenuItem, isChecked) 
{
	php_ui_item_t *item = php_ui_item_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiMenuItemChecked(item->i)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_item_set_checked_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, checked, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void MenuItem::setChecked(bool checked) */
PHP_METHOD(MenuItem, setChecked) 
{
	php_ui_item_t *item = php_ui_item_fetch(getThis());
	zend_bool checked = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &checked) != SUCCESS) {
		return;
	}

	uiMenuItemSetChecked(item->i, (int) checked);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_item_on_click_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void MenuItem::onClick() */
PHP_METHOD(MenuItem, onClick)
{
	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_item_methods[] = {
	PHP_ME(MenuItem, enable, php_ui_item_enable_info,          ZEND_ACC_PUBLIC)
	PHP_ME(MenuItem, disable, php_ui_item_disable_info,        ZEND_ACC_PUBLIC)
	PHP_ME(MenuItem, isChecked, php_ui_item_is_checked_info,   ZEND_ACC_PUBLIC)
	PHP_ME(MenuItem, setChecked, php_ui_item_set_checked_info, ZEND_ACC_PUBLIC)
	PHP_ME(MenuItem, onClick, php_ui_item_on_click_info,       ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_MenuItem) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "MenuItem", php_ui_item_methods);

	uiItem_ce = zend_register_internal_class(&ce);
	uiItem_ce->create_object = php_ui_item_create;

	memcpy(&php_ui_item_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_item_handlers.offset = XtOffsetOf(php_ui_item_t, std);

	return SUCCESS;
} /* }}} */
#endif
