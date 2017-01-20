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

zend_class_entry *uiButton_ce;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);
extern int php_ui_call(zend_fcall_info *fci, zend_fcall_info_cache *fcc);

zend_object* php_ui_button_create(zend_class_entry *ce) {
	php_ui_button_t *button = 
		(php_ui_button_t*) ecalloc(1, sizeof(php_ui_button_t) + zend_object_properties_size(ce));

	zend_object_std_init(&button->std, ce);

	object_properties_init(&button->std, ce);

	button->std.handlers = &php_ui_button_handlers;

	php_ui_set_call(&button->std, ZEND_STRL("onclick"), &button->click.fci, &button->click.fcc);

	return &button->std;
}

void php_ui_button_click_handler(uiButton *u, void *_button) {
	php_ui_button_t *button = (php_ui_button_t*) _button;

	if (button->click.fci.size) {
		zval rv;

		ZVAL_UNDEF(&rv);

		button->click.fci.retval = &rv;

		if (php_ui_call(&button->click.fci, &button->click.fcc) != SUCCESS) {
			return;
		}

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

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_button_get_text_info, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_button_get_text_info, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

/* {{{ proto string Button::getText(void) */
PHP_METHOD(Button, getText)
{
	php_ui_button_t *button = php_ui_button_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(uiButtonText(button->b));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_button_on_click_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Button::onClick() */
PHP_METHOD(Button, onClick)
{

} /* }}} */

/* {{{ */
const zend_function_entry php_ui_button_methods[] = {
	PHP_ME(Button, __construct, php_ui_button_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Button, setText,	    php_ui_button_set_text_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Button, getText,	    php_ui_button_get_text_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Button, onClick,	    php_ui_button_on_click_info,  ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Button) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "Button", php_ui_button_methods);

	uiButton_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiButton_ce->create_object = php_ui_button_create;

	memcpy(&php_ui_button_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_button_handlers.offset = XtOffsetOf(php_ui_button_t, std);

	return SUCCESS;
} /* }}} */
#endif
