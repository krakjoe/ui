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
#ifndef HAVE_PHP_UI_CBUTTON
#define HAVE_PHP_UI_CBUTTON
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/cbutton.h>

zend_object_handlers php_ui_cbutton_handlers;

extern void php_ui_event_handler(void *, void *);

typedef void (*php_ui_cbutton_on_change_handler)(uiColorButton *, void *);

zend_object* php_ui_cbutton_create(zend_class_entry *ce) {
	php_ui_cbutton_t *cbutton = 
		(php_ui_cbutton_t*) ecalloc(1, sizeof(php_ui_cbutton_t));

	ZVAL_UNDEF(&cbutton->handler);

	zend_object_std_init(&cbutton->std, ce);

	object_properties_init(&cbutton->std, ce);

	cbutton->std.handlers = &php_ui_cbutton_handlers;

	return &cbutton->std;
}

void php_ui_cbutton_free(zend_object *o) {
	php_ui_cbutton_t *cbutton = php_ui_cbutton_from(o);

	if (Z_TYPE(cbutton->handler) != IS_UNDEF) {
		zval_ptr_dtor(&cbutton->handler);
	}

	zend_object_std_dtor(o);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_cbutton_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto ColorButton ColorButton::__construct() */
PHP_METHOD(ColorButton, __construct) 
{
	php_ui_cbutton_t *cbutton = php_ui_cbutton_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	cbutton->b = uiNewColorButton();

	uiColorButtonOnChanged(cbutton->b, (php_ui_cbutton_on_change_handler) php_ui_event_handler, cbutton);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_cbutton_set_color_info, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, r, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void ColorButton::setColor(int r, int g, int b, int a) */
PHP_METHOD(ColorButton, setColor)
{
	php_ui_cbutton_t *cbutton = php_ui_cbutton_fetch(getThis());
	double r = 0, g = 0, b = 0, a = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddd", &r, &g, &b, &a) != SUCCESS) {
		return;
	}

	uiColorButtonSetColor(cbutton->b, (int) r, (int) g, (int) b, (int) a);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_cbutton_get_color_info, 0, 0, IS_ARRAY, NULL, 1)
ZEND_END_ARG_INFO()

/* {{{ proto array ColorButton::getColor(void) */
PHP_METHOD(ColorButton, getColor)
{
	php_ui_cbutton_t *cbutton = php_ui_cbutton_fetch(getThis());
	double r = 0, g = 0, b = 0, a = 0;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiColorButtonColor(cbutton->b, &r, &g, &b, &a);

	array_init(return_value);

	add_next_index_double(return_value, r);
	add_next_index_double(return_value, g);
	add_next_index_double(return_value, b);
	add_next_index_double(return_value, a);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_cbutton_on_changed_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, handler, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void ColorButton::onChanged(callable handler) */
PHP_METHOD(ColorButton, onChanged)
{
	php_ui_cbutton_t *cbutton = php_ui_cbutton_fetch(getThis());
	zval *handler = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &handler) != SUCCESS) {
		return;
	}

	if (Z_TYPE(cbutton->handler) != IS_UNDEF) {
		zval_ptr_dtor(&cbutton->handler);
	}

	ZVAL_COPY(&cbutton->handler, handler);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_cbutton_methods[] = {
	PHP_ME(ColorButton, __construct,    php_ui_cbutton_construct_info,   ZEND_ACC_PUBLIC)
	PHP_ME(ColorButton, setColor,	    php_ui_cbutton_set_color_info,   ZEND_ACC_PUBLIC)
	PHP_ME(ColorButton, getColor,	    php_ui_cbutton_get_color_info,   ZEND_ACC_PUBLIC)
	PHP_ME(ColorButton, onChanged,      php_ui_cbutton_on_changed_info,  ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_ColorButton) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "ColorButton", php_ui_cbutton_methods);

	uiColorButton_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiColorButton_ce->create_object = php_ui_cbutton_create;

	memcpy(&php_ui_cbutton_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_cbutton_handlers.free_obj = php_ui_cbutton_free;
	php_ui_cbutton_handlers.offset = XtOffsetOf(php_ui_cbutton_t, std);

	return SUCCESS;
} /* }}} */
#endif
