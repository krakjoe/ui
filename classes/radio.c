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
#ifndef HAVE_PHP_UI_RADIO
#define HAVE_PHP_UI_RADIO
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/radio.h>

zend_object_handlers php_ui_radio_handlers;

zend_object* php_ui_radio_create(zend_class_entry *ce) {
	php_ui_radio_t *radio = 
		(php_ui_radio_t*) ecalloc(1, sizeof(php_ui_radio_t) + zend_object_properties_size(ce));

	zend_object_std_init(&radio->std, ce);

	object_properties_init(&radio->std, ce);

	radio->std.handlers = &php_ui_radio_handlers;

	return &radio->std;
}

void php_ui_radio_free(zend_object *o) {
	php_ui_radio_t *radio = php_ui_radio_from(o);

	if (radio->selected.fci.size) {
		if (Z_TYPE(radio->selected.fci.function_name)) {
			zval_ptr_dtor(&radio->selected.fci.function_name);
		}
	}

	zend_object_std_dtor(o);
}

void php_ui_radio_selected_handler(uiRadioButtons *m,  void *_radio) {
	php_ui_radio_t *radio = (php_ui_radio_t*) _radio;

	if (radio->selected.fci.size) {
		zval rv;
		zval ctrl;

		ZVAL_UNDEF(&rv);
		ZVAL_OBJ(&ctrl, &radio->std);

		radio->selected.fci.retval = &rv;

		zend_fcall_info_argn(&radio->selected.fci, 1, &ctrl);

		if (zend_call_function(&radio->selected.fci, &radio->selected.fcc) != SUCCESS) {
			return;
		}

		zend_fcall_info_args_clear(&radio->selected.fci, 1);

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_radio_construct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Radio Radio::__construct(string text) */
PHP_METHOD(Radio, __construct) 
{
	php_ui_radio_t *radio = php_ui_radio_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	radio->r = uiNewRadioButtons();

	uiRadioButtonsOnSelected(radio->r, php_ui_radio_selected_handler, radio);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_radio_set_selected_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Radio::setSelected(int index) */
PHP_METHOD(Radio, setSelected) 
{
	php_ui_radio_t *radio = php_ui_radio_fetch(getThis());
	zend_long selected = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &selected) != SUCCESS) {
		return;
	}

	uiRadioButtonsSetSelected(radio->r, selected);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_radio_get_selected_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Radio::getSelected(void) */
PHP_METHOD(Radio, getSelected) 
{
	php_ui_radio_t *radio = php_ui_radio_fetch(getThis());
	char *text = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(uiRadioButtonsSelected(radio->r));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_radio_append_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Radio::append(string text) */
PHP_METHOD(Radio, append) 
{
	php_ui_radio_t *radio = php_ui_radio_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiRadioButtonsAppend(radio->r, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_radio_on_selected_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, handler, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Radio::onSelected(callable handler) */
PHP_METHOD(Radio, onSelected)
{
	php_ui_radio_t *radio = php_ui_radio_fetch(getThis());
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "f", &fci, &fcc) != SUCCESS) {
		return;
	}

	if (radio->selected.fci.size) {
		if (Z_TYPE(radio->selected.fci.function_name)) {
			zval_ptr_dtor(&radio->selected.fci.function_name);
		}
	}

	memcpy(&radio->selected.fci, &fci, sizeof(zend_fcall_info));
	memcpy(&radio->selected.fcc, &fcc, sizeof(zend_fcall_info_cache));

	if (Z_TYPE(radio->selected.fci.function_name)) {
		Z_ADDREF(radio->selected.fci.function_name);
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_radio_methods[] = {
	PHP_ME(Radio, __construct, php_ui_radio_construct_info,       ZEND_ACC_PUBLIC)
	PHP_ME(Radio, setSelected, php_ui_radio_set_selected_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Radio, getSelected, php_ui_radio_get_selected_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Radio, append,      php_ui_radio_append_info,          ZEND_ACC_PUBLIC)
	PHP_ME(Radio, onSelected,  php_ui_radio_on_selected_info,     ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Radio) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Control", "Radio", php_ui_radio_methods);

	uiRadio_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiRadio_ce->create_object = php_ui_radio_create;
	uiRadio_ce->ce_flags |= ZEND_ACC_FINAL;

	memcpy(&php_ui_radio_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_radio_handlers.free_obj = php_ui_radio_free;
	php_ui_radio_handlers.offset = XtOffsetOf(php_ui_radio_t, std);

	return SUCCESS;
} /* }}} */
#endif
