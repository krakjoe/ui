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
#ifndef HAVE_PHP_UI_COMBO
#define HAVE_PHP_UI_COMBO
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/combo.h>

zend_object_handlers php_ui_combo_handlers;

zend_class_entry *uiCombo_ce;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);
extern int php_ui_call(zend_fcall_info *fci, zend_fcall_info_cache *fcc);

void php_ui_combo_select_handler(uiCombobox *u, void *_combo) {
	php_ui_combo_t *combo = (php_ui_combo_t*) _combo;

	if (combo->select.fci.size) {
		zval rv;

		ZVAL_UNDEF(&rv);

		combo->select.fci.retval = &rv;

		if (php_ui_call(&combo->select.fci, &combo->select.fcc) != SUCCESS) {
			return;
		}

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

zend_object* php_ui_combo_create(zend_class_entry *ce) {
	php_ui_combo_t *combo = 
		(php_ui_combo_t*) ecalloc(1, sizeof(php_ui_combo_t) + zend_object_properties_size(ce));

	zend_object_std_init(&combo->std, ce);

	object_properties_init(&combo->std, ce);

	combo->std.handlers = &php_ui_combo_handlers;

	combo->c = uiNewCombobox();

	uiComboboxOnSelected(combo->c, php_ui_combo_select_handler, combo);

	php_ui_set_call(&combo->std, ZEND_STRL("onselected"), &combo->select.fci, &combo->select.fcc);

	return &combo->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_combo_set_selected_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Combo::setSelected(int index) */
PHP_METHOD(Combo, setSelected) 
{
	php_ui_combo_t *combo = php_ui_combo_fetch(getThis());
	zend_long selected = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &selected) != SUCCESS) {
		return;
	}

	uiComboboxSetSelected(combo->c, selected);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_combo_get_selected_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Combo::getSelected(void) */
PHP_METHOD(Combo, getSelected) 
{
	php_ui_combo_t *combo = php_ui_combo_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(uiComboboxSelected(combo->c));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_combo_append_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Combo::append(string text) */
PHP_METHOD(Combo, append) 
{
	php_ui_combo_t *combo = php_ui_combo_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiComboboxAppend(combo->c, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_combo_on_selected_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Combo::onSelected() */
PHP_METHOD(Combo, onSelected)
{
	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_combo_methods[] = {
	PHP_ME(Combo, setSelected, php_ui_combo_set_selected_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Combo, getSelected, php_ui_combo_get_selected_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Combo, append,      php_ui_combo_append_info,          ZEND_ACC_PUBLIC)
	PHP_ME(Combo, onSelected,  php_ui_combo_on_selected_info,     ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Combo) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "Combo", php_ui_combo_methods);

	uiCombo_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiCombo_ce->create_object = php_ui_combo_create;

	memcpy(&php_ui_combo_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_combo_handlers.offset = XtOffsetOf(php_ui_combo_t, std);

	return SUCCESS;
} /* }}} */
#endif
