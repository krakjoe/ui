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
#ifndef HAVE_PHP_UI_SPIN
#define HAVE_PHP_UI_SPIN
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/spin.h>

zend_object_handlers php_ui_spin_handlers;

zend_object* php_ui_spin_create(zend_class_entry *ce) {
	php_ui_spin_t *spin = 
		(php_ui_spin_t*) ecalloc(1, sizeof(php_ui_spin_t));

	ZVAL_UNDEF(&spin->handler);

	zend_object_std_init(&spin->std, ce);

	object_properties_init(&spin->std, ce);

	spin->std.handlers = &php_ui_spin_handlers;

	return &spin->std;
}

void php_ui_spin_free(zend_object *o) {
	php_ui_spin_t *spin = php_ui_spin_from(o);

	if (Z_TYPE(spin->handler) != IS_UNDEF) {
		zval_ptr_dtor(&spin->handler);
	}

	zend_object_std_dtor(o);
}

void php_ui_spin_on_change(uiSpinbox *u, void *_spin) {
	php_ui_spin_t *spin = (php_ui_spin_t*) _spin;

	if (Z_TYPE(spin->handler) != IS_UNDEF) {
		zval rv;
		zend_fcall_info fci = empty_fcall_info;
		zend_fcall_info_cache fcc = empty_fcall_info_cache;
		zend_string *cn = NULL;
		char *er = NULL;

		if (zend_fcall_info_init(&spin->handler, IS_CALLABLE_CHECK_SILENT, &fci, &fcc, &cn, &er) != SUCCESS) {
			return;
		}

		fci.retval = &rv;

		ZVAL_UNDEF(&rv);

		if (zend_call_function(&fci, &fcc) != SUCCESS) {
			return;
		}

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_spin_construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Spin Spin::__construct(int min, int max) */
PHP_METHOD(Spin, __construct) 
{
	php_ui_spin_t *spin = php_ui_spin_fetch(getThis());
	zend_long min = 0, max = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ll", &min, &max) != SUCCESS) {
		return;
	}

	spin->s = uiNewSpinbox((int) min, (int) max);

	uiSpinboxOnChanged(spin->s, php_ui_spin_on_change, spin);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_spin_set_value_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Spin::setValue(int value) */
PHP_METHOD(Spin, setValue) 
{
	php_ui_spin_t *spin = php_ui_spin_fetch(getThis());
	zend_long value = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &value) != SUCCESS) {
		return;
	}

	uiSpinboxSetValue(spin->s, value);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_spin_get_value_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Spin::getValue(void) */
PHP_METHOD(Spin, getValue) 
{
	php_ui_spin_t *spin = php_ui_spin_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(uiSpinboxValue(spin->s));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_spin_on_change_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, handler, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Spin::onChange(callable handler) */
PHP_METHOD(Spin, onChange)
{
	php_ui_spin_t *spin = php_ui_spin_fetch(getThis());
	zval *handler = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &handler) != SUCCESS) {
		return;
	}

	if (Z_TYPE(spin->handler) != IS_UNDEF) {
		zval_ptr_dtor(&spin->handler);
	}

	ZVAL_COPY(&spin->handler, handler);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_spin_methods[] = {
	PHP_ME(Spin, __construct, php_ui_spin_construct_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Spin, setValue,    php_ui_spin_set_value_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Spin, getValue,    php_ui_spin_get_value_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Spin, onChange,    php_ui_spin_on_change_info,  ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Spin) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Spin", php_ui_spin_methods);

	uiSpin_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiSpin_ce->create_object = php_ui_spin_create;

	memcpy(&php_ui_spin_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_spin_handlers.free_obj = php_ui_spin_free;
	php_ui_spin_handlers.offset = XtOffsetOf(php_ui_spin_t, std);

	return SUCCESS;
} /* }}} */
#endif
