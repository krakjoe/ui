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
#ifndef HAVE_PHP_UI_FORM
#define HAVE_PHP_UI_FORM
#include <ui.h>

#include "php.h"

#include <classes/exceptions.h>
#include <classes/control.h>
#include <classes/form.h>

zend_object_handlers php_ui_form_handlers;

extern void php_ui_set_controls(zend_object *std, const char *name, size_t nlength, HashTable *table);

#define PHP_UI_FORM_CONTROL_CHECK(form, control) do { \
	if (control < 0 || control >= zend_hash_num_elements(form->controls)) { \
		php_ui_exception_ex( \
			InvalidArgumentException, "control %ld does not exist", control); \
		return; \
	} \
} while(0)

zend_class_entry *uiForm_ce;

zend_object* php_ui_form_create(zend_class_entry *ce) {
	php_ui_form_t *form = 
		(php_ui_form_t*) ecalloc(1, sizeof(php_ui_form_t) + zend_object_properties_size(ce));

	zend_object_std_init(&form->std, ce);

	object_properties_init(&form->std, ce);

	form->std.handlers = &php_ui_form_handlers;

	ALLOC_HASHTABLE(form->controls);

	zend_hash_init(form->controls, 8, NULL, ZVAL_PTR_DTOR, 0);

	php_ui_set_controls(&form->std, ZEND_STRL("controls"), form->controls);

	form->f = uiNewForm();

	return &form->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_form_set_padded_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, padded, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Form::setPadded(bool padded) */
PHP_METHOD(Form, setPadded) 
{
	php_ui_form_t *form = php_ui_form_fetch(getThis());
	zend_bool padded = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &padded) != SUCCESS) {
		return;
	}

	uiFormSetPadded(form->f, padded);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_form_is_padded_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Form::isPadded(void) */
PHP_METHOD(Form, isPadded) 
{
	php_ui_form_t *form = php_ui_form_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}
	
	if (uiFormPadded(form->f)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_form_append_info, 0, 2, IS_LONG, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, label, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, control, UI\\Control, 0)
	ZEND_ARG_TYPE_INFO(0, stretchy, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Form::append(string label, Control control [, bool stretchy = false]) */
PHP_METHOD(Form, append)
{
	php_ui_form_t *form = php_ui_form_fetch(getThis());
	zend_string *label = NULL;
	zval *control = NULL;
	zend_bool stretchy = 0;
	uiControl *ctrl;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SO|b", &label, &control, uiControl_ce, &stretchy) != SUCCESS) {
		return;
	}

	ctrl = php_ui_control_fetch(control);

	uiFormAppend(form->f, ZSTR_VAL(label), ctrl, stretchy);

	if (zend_hash_next_index_insert(form->controls, control)) {
		Z_ADDREF_P(control);
	}

	RETURN_LONG(zend_hash_num_elements(form->controls) - 1);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_form_delete_info, 0, 1, _IS_BOOL, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Form::delete(int index) */
PHP_METHOD(Form, delete)
{
	php_ui_form_t *form = php_ui_form_fetch(getThis());
	zend_long index = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &index) != SUCCESS) {
		return;
	}

	PHP_UI_FORM_CONTROL_CHECK(form, index);

	if (zend_hash_index_del(form->controls, index)) {
		uiFormDelete(form->f, (int) index);

		RETURN_TRUE;
	}

	RETURN_FALSE;
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_form_methods[] = {
	PHP_ME(Form, setPadded,   php_ui_form_set_padded_info, ZEND_ACC_PUBLIC)
	PHP_ME(Form, isPadded,    php_ui_form_is_padded_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Form, append,      php_ui_form_append_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Form, delete,      php_ui_form_delete_info,     ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Form) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "Form", php_ui_form_methods);

	uiForm_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiForm_ce->create_object = php_ui_form_create;
	zend_declare_property_null(uiForm_ce, ZEND_STRL("controls"), ZEND_ACC_PROTECTED);

	memcpy(&php_ui_form_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_form_handlers.offset = XtOffsetOf(php_ui_form_t, std);

	return SUCCESS;
} /* }}} */
#endif
