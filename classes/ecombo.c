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
#ifndef HAVE_PHP_UI_EECOMBO
#define HAVE_PHP_UI_EECOMBO
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/ecombo.h>

zend_object_handlers php_ui_ecombo_handlers;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);

void php_ui_ecombo_change_handler(uiEditableCombobox *u, void *_ecombo) {
	php_ui_ecombo_t *ecombo = (php_ui_ecombo_t*) _ecombo;

	if (ecombo->change.fci.size) {
		zval rv;

		ZVAL_UNDEF(&rv);

		ecombo->change.fci.retval = &rv;

		if (zend_call_function(&ecombo->change.fci, &ecombo->change.fcc) != SUCCESS) {
			return;
		}

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

zend_object* php_ui_ecombo_create(zend_class_entry *ce) {
	php_ui_ecombo_t *ecombo = 
		(php_ui_ecombo_t*) ecalloc(1, sizeof(php_ui_ecombo_t) + zend_object_properties_size(ce));

	zend_object_std_init(&ecombo->std, ce);

	object_properties_init(&ecombo->std, ce);

	ecombo->std.handlers = &php_ui_ecombo_handlers;

	ecombo->c = uiNewEditableCombobox();

	uiEditableComboboxOnChanged(ecombo->c, php_ui_ecombo_change_handler, ecombo);

	php_ui_set_call(&ecombo->std, ZEND_STRL("onchange"), &ecombo->change.fci, &ecombo->change.fcc);

	return &ecombo->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_ecombo_set_text_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void EditableCombo::setText(string text) */
PHP_METHOD(EditableCombo, setText) 
{
	php_ui_ecombo_t *ecombo = php_ui_ecombo_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiEditableComboboxSetText(ecombo->c, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_ecombo_get_text_info, 0, 0, IS_STRING, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string EditableCombo::getText(void) */
PHP_METHOD(EditableCombo, getText) 
{
	php_ui_ecombo_t *ecombo = php_ui_ecombo_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(uiEditableComboboxText(ecombo->c));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_ecombo_append_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void EditableCombo::append(string text) */
PHP_METHOD(EditableCombo, append) 
{
	php_ui_ecombo_t *ecombo = php_ui_ecombo_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiEditableComboboxAppend(ecombo->c, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_ecombo_on_change_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void EditableCombo::onChange() */
PHP_METHOD(EditableCombo, onChange)
{
	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_ecombo_methods[] = {
	PHP_ME(EditableCombo, setText,     php_ui_ecombo_set_text_info,    ZEND_ACC_PUBLIC)
	PHP_ME(EditableCombo, getText,     php_ui_ecombo_get_text_info,    ZEND_ACC_PUBLIC)
	PHP_ME(EditableCombo, append,      php_ui_ecombo_append_info,      ZEND_ACC_PUBLIC)
	PHP_ME(EditableCombo, onChange,    php_ui_ecombo_on_change_info,   ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_EditableCombo) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "EditableCombo", php_ui_ecombo_methods);

	uiEditableCombo_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiEditableCombo_ce->create_object = php_ui_ecombo_create;

	memcpy(&php_ui_ecombo_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_ecombo_handlers.offset = XtOffsetOf(php_ui_ecombo_t, std);

	return SUCCESS;
} /* }}} */
#endif
