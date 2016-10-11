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
#ifndef HAVE_PHP_UI_CHECK
#define HAVE_PHP_UI_CHECK
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/check.h>

zend_object_handlers php_ui_check_handlers;

extern void php_ui_event_handler(void *, void *);

typedef void (*php_ui_check_on_toggle_handler)(uiCheckbox *, void *);

zend_object* php_ui_check_create(zend_class_entry *ce) {
	php_ui_check_t *check = 
		(php_ui_check_t*) ecalloc(1, sizeof(php_ui_check_t));

	ZVAL_UNDEF(&check->handler);

	zend_object_std_init(&check->std, ce);

	object_properties_init(&check->std, ce);

	check->std.handlers = &php_ui_check_handlers;

	return &check->std;
}

void php_ui_check_free(zend_object *o) {
	php_ui_check_t *check = php_ui_check_from(o);

	if (Z_TYPE(check->handler) != IS_UNDEF) {
		zval_ptr_dtor(&check->handler);
	}

	zend_object_std_dtor(o);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_check_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Check Check::__construct(string text) */
PHP_METHOD(Check, __construct) 
{
	php_ui_check_t *check = php_ui_check_fetch(getThis());
	zend_string *text = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	check->c = uiNewCheckbox(ZSTR_VAL(text));

	uiCheckboxOnToggled(check->c, (php_ui_check_on_toggle_handler) php_ui_event_handler, check);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_check_set_text_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, checked, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Check::setText(string text) */
PHP_METHOD(Check, setText) 
{
	php_ui_check_t *check = php_ui_check_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiCheckboxSetText(check->c, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_check_get_text_info, 0, 0, IS_STRING, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Check::getText(void) */
PHP_METHOD(Check, getText) 
{
	php_ui_check_t *check = php_ui_check_fetch(getThis());
	char *text = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(uiCheckboxText(check->c));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_check_set_checked_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, checked, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Check::setChecked(bool checked) */
PHP_METHOD(Check, setChecked) 
{
	php_ui_check_t *check = php_ui_check_fetch(getThis());
	zend_bool checked = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &checked) != SUCCESS) {
		return;
	}

	uiCheckboxSetChecked(check->c, checked);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_check_is_checked_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Check::isChecked(void) */
PHP_METHOD(Check, isChecked) 
{
	php_ui_check_t *check = php_ui_check_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiCheckboxChecked(check->c)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_check_on_toggle_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, handler, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Check::onToggle(callable handler) */
PHP_METHOD(Check, onToggle)
{
	php_ui_check_t *check = php_ui_check_fetch(getThis());
	zval *handler = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &handler) != SUCCESS) {
		return;
	}

	if (Z_TYPE(check->handler) != IS_UNDEF) {
		zval_ptr_dtor(&check->handler);
	}

	ZVAL_COPY(&check->handler, handler);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_check_methods[] = {
	PHP_ME(Check, __construct, php_ui_check_construct_info,   ZEND_ACC_PUBLIC)
	PHP_ME(Check, setText,     php_ui_check_set_text_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Check, getText,     php_ui_check_get_text_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Check, setChecked,  php_ui_check_set_checked_info, ZEND_ACC_PUBLIC)
	PHP_ME(Check, isChecked,   php_ui_check_is_checked_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Check, onToggle,    php_ui_check_on_toggle_info,   ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Check) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Check", php_ui_check_methods);

	uiCheck_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiCheck_ce->create_object = php_ui_check_create;

	memcpy(&php_ui_check_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_check_handlers.free_obj = php_ui_check_free;
	php_ui_check_handlers.offset = XtOffsetOf(php_ui_check_t, std);

	return SUCCESS;
} /* }}} */
#endif
