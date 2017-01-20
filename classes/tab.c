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
#ifndef HAVE_PHP_UI_TAB
#define HAVE_PHP_UI_TAB
#include <ui.h>

#include "php.h"

#include <classes/exceptions.h>
#include <classes/control.h>
#include <classes/tab.h>

#define PHP_UI_TAB_PAGE_CHECK(tab, page) do { \
	if (page < 0 || page >= zend_hash_num_elements(tab->controls)) { \
		php_ui_exception_ex( \
			InvalidArgumentException, "page %ld does not exist", page); \
		return; \
	} \
} while(0)

zend_object_handlers php_ui_tab_handlers;

zend_class_entry *uiTab_ce;

zend_object* php_ui_tab_create(zend_class_entry *ce) {
	php_ui_tab_t *tab = (php_ui_tab_t*) ecalloc(1, sizeof(php_ui_tab_t) + zend_object_properties_size(ce));

	zend_object_std_init(&tab->std, ce);

	object_properties_init(&tab->std, ce);

	tab->std.handlers = &php_ui_tab_handlers;

	tab->t = uiNewTab();

	ALLOC_HASHTABLE(tab->controls);

	zend_hash_init(tab->controls, 8, NULL, ZVAL_PTR_DTOR, 0);

	php_ui_control_set_controls(&tab->std, tab->controls);

	return &tab->std;
}

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_append_info, 0, 2, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_append_info, 0, 2, IS_LONG, NULL, 0)
#endif
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0,  control, UI\\Control, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Tab::append(string name, Control control) */
PHP_METHOD(Tab, append)
{
	php_ui_tab_t *tab = php_ui_tab_fetch(getThis());
	zend_string *name = NULL;
	zval *control = NULL;
	php_ui_control_t *ctrl = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SO", &name, &control, uiControl_ce) != SUCCESS) {
		return;
	}

	ctrl = php_ui_control_fetch(control);

	uiTabAppend(tab->t, ZSTR_VAL(name), ctrl->control);

	ctrl->parent = &tab->std;

	if (zend_hash_next_index_insert(tab->controls, control)) {
		Z_ADDREF_P(control);
	}

	RETURN_LONG(zend_hash_num_elements(tab->controls) - 1);
}
/* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_delete_info, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_delete_info, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Tab::delete(int index) */
PHP_METHOD(Tab, delete)
{
	php_ui_tab_t *tab = php_ui_tab_fetch(getThis());
	zend_long index = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &index) != SUCCESS) {
		return;
	}

	PHP_UI_TAB_PAGE_CHECK(tab, index);

	if (zend_hash_index_del(tab->controls, index) == SUCCESS) {
		uiTabDelete(tab->t, (int) index);

		RETURN_TRUE;
	}

	RETURN_FALSE;
} /* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_pages_info, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_pages_info, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

/* {{{ proto int Tab::pages(void) */
PHP_METHOD(Tab, pages)
{
	php_ui_tab_t *tab = php_ui_tab_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(zend_hash_num_elements(tab->controls));
} /* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_insert_at_info, 0, 3, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_insert_at_info, 0, 3, IS_LONG, NULL, 0)
#endif
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, before, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0,  control, UI\\Control, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Tab::insertAt(string name, int page, Control control) */
PHP_METHOD(Tab, insertAt)
{
	php_ui_tab_t *tab = php_ui_tab_fetch(getThis());
	zend_string *name = NULL;
	zend_long page = 0;
	zval *control = NULL;
	php_ui_control_t *ctrl = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SlO", &name, &page, &control, uiControl_ce) != SUCCESS) {
		return;
	}

	ctrl = php_ui_control_fetch(control);

	if (!php_ui_control_set_parent(control, getThis())) {
		return;
	}

	uiTabInsertAt(tab->t, ZSTR_VAL(name), (int) page, ctrl->control);

	RETURN_LONG(uiTabNumPages(tab->t));
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_tab_set_margin_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, page, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, margin, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Tab::setMargin(int page, bool margin) */
PHP_METHOD(Tab, setMargin) 
{
	php_ui_tab_t *tab = php_ui_tab_fetch(getThis());
	zend_bool margin = 0;
	zend_long page = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lb", &page, &margin) != SUCCESS) {
		return;
	}

	PHP_UI_TAB_PAGE_CHECK(tab, page);

	uiTabSetMargined(tab->t, (int) page, (int) margin);

	RETURN_TRUE;
} /* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_has_margin_info, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_tab_has_margin_info, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_TYPE_INFO(0, page, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Tab::hasMargin(int page) */
PHP_METHOD(Tab, hasMargin) 
{
	php_ui_tab_t *tab = php_ui_tab_fetch(getThis());
	zend_long page = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &page) != SUCCESS) {
		return;
	}

	PHP_UI_TAB_PAGE_CHECK(tab, page);

	if (uiTabMargined(tab->t, (int) page)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_tab_methods[] = {
	PHP_ME(Tab, append,        php_ui_tab_append_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Tab, delete,        php_ui_tab_delete_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Tab, pages,         php_ui_tab_pages_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Tab, insertAt,      php_ui_tab_insert_at_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Tab, setMargin,     php_ui_tab_set_margin_info, ZEND_ACC_PUBLIC)
	PHP_ME(Tab, hasMargin,     php_ui_tab_has_margin_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Tab) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "Tab", php_ui_tab_methods);

	uiTab_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiTab_ce->create_object = php_ui_tab_create;

	zend_declare_property_null(uiTab_ce, ZEND_STRL("controls"), ZEND_ACC_PROTECTED);

	memcpy(&php_ui_tab_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_tab_handlers.offset = XtOffsetOf(php_ui_tab_t, std);
	
	return SUCCESS;
} /* }}} */
#endif
