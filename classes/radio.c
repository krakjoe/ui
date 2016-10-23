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

zend_class_entry *uiRadio_ce;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);

void php_ui_radio_selected_handler(uiRadioButtons *m,  void *_radio) {
	php_ui_radio_t *radio = (php_ui_radio_t*) _radio;

	if (radio->selected.fci.size) {
		zval rv;

		ZVAL_UNDEF(&rv);

		radio->selected.fci.retval = &rv;

		if (zend_call_function(&radio->selected.fci, &radio->selected.fcc) != SUCCESS) {
			return;
		}

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

zend_object* php_ui_radio_create(zend_class_entry *ce) {
	php_ui_radio_t *radio = 
		(php_ui_radio_t*) ecalloc(1, sizeof(php_ui_radio_t) + zend_object_properties_size(ce));

	zend_object_std_init(&radio->std, ce);

	object_properties_init(&radio->std, ce);

	radio->std.handlers = &php_ui_radio_handlers;

	php_ui_set_call(&radio->std, ZEND_STRL("onselected"), &radio->selected.fci, &radio->selected.fcc);

	radio->r = uiNewRadioButtons();

	uiRadioButtonsOnSelected(radio->r, php_ui_radio_selected_handler, radio);

	return &radio->std;
}

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

ZEND_BEGIN_ARG_INFO_EX(php_ui_radio_on_selected_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Radio::onSelected(callable handler) */
PHP_METHOD(Radio, onSelected)
{
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_radio_methods[] = {
	PHP_ME(Radio, setSelected, php_ui_radio_set_selected_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Radio, getSelected, php_ui_radio_get_selected_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Radio, append,      php_ui_radio_append_info,          ZEND_ACC_PUBLIC)
	PHP_ME(Radio, onSelected,  php_ui_radio_on_selected_info,     ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Radio) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "Radio", php_ui_radio_methods);

	uiRadio_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiRadio_ce->create_object = php_ui_radio_create;

	memcpy(&php_ui_radio_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_radio_handlers.offset = XtOffsetOf(php_ui_radio_t, std);

	return SUCCESS;
} /* }}} */
#endif
