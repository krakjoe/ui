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
#ifndef HAVE_PHP_UI_BUTTON
#define HAVE_PHP_UI_BUTTON
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/button.h>

zend_object_handlers php_ui_button_handlers;

zend_object* php_ui_button_create(zend_class_entry *ce) {
	php_ui_button_t *button = 
		(php_ui_button_t*) ecalloc(1, sizeof(php_ui_button_t) + zend_object_properties_size(ce));

	zend_object_std_init(&button->std, ce);

	object_properties_init(&button->std, ce);

	button->std.handlers = &php_ui_button_handlers;

	return &button->std;
}

void php_ui_button_free(zend_object *o) {
	php_ui_button_t *button = php_ui_button_from(o);

	if (button->click.fci.size) {
		if (Z_TYPE(button->click.fci.function_name)) {
			zval_ptr_dtor(&button->click.fci.function_name);
		}
	}

	zend_object_std_dtor(o);
}

void php_ui_button_click_handler(uiButton *u, void *_button) {
	php_ui_button_t *button = (php_ui_button_t*) _button;

	if (button->click.fci.size) {
		zval rv;
		zval ctrl;

		ZVAL_UNDEF(&rv);
		ZVAL_OBJ(&ctrl, &button->std);

		button->click.fci.retval = &rv;

		zend_fcall_info_argn(&button->click.fci, 1, &ctrl);

		if (zend_call_function(&button->click.fci, &button->click.fcc) != SUCCESS) {
			return;
		}

		zend_fcall_info_args_clear(&button->click.fci, 1);

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_button_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Button Button::__construct(string text) */
PHP_METHOD(Button, __construct) 
{
	php_ui_button_t *button = php_ui_button_fetch(getThis());
	zend_string *text = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	button->b = uiNewButton(ZSTR_VAL(text));

	uiButtonOnClicked(button->b, php_ui_button_click_handler, button);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_button_set_text_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Button::setText(string text) */
PHP_METHOD(Button, setText)
{
	php_ui_button_t *button = php_ui_button_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiButtonSetText(button->b, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_button_get_text_info, 0, 0, IS_STRING, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Button::getText(void) */
PHP_METHOD(Button, getText)
{
	php_ui_button_t *button = php_ui_button_fetch(getThis());
	char *text = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(uiButtonText(button->b));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_button_on_click_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, handler, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Button::onClick(callable handler) */
PHP_METHOD(Button, onClick)
{
	php_ui_button_t *button = php_ui_button_fetch(getThis());
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "f", &fci, &fcc) != SUCCESS) {
		return;
	}

	if (button->click.fci.size) {
		if (Z_TYPE(button->click.fci.function_name)) {
			zval_ptr_dtor(&button->click.fci.function_name);
		}
	}

	memcpy(&button->click.fci, &fci, sizeof(zend_fcall_info));
	memcpy(&button->click.fcc, &fcc, sizeof(zend_fcall_info_cache));

	if (Z_TYPE(button->click.fci.function_name)) {
		Z_ADDREF(button->click.fci.function_name);
	}	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_button_methods[] = {
	PHP_ME(Button, __construct, php_ui_button_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Button, setText,	    php_ui_button_set_text_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Button, getText,	    php_ui_button_get_text_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Button, onClick,	    php_ui_button_on_click_info,  ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Button) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Control", "Button", php_ui_button_methods);

	uiButton_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiButton_ce->create_object = php_ui_button_create;
	uiButton_ce->ce_flags |= ZEND_ACC_FINAL;

	memcpy(&php_ui_button_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_button_handlers.free_obj = php_ui_button_free;
	php_ui_button_handlers.offset = XtOffsetOf(php_ui_button_t, std);

	return SUCCESS;
} /* }}} */
#endif
