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

zend_class_entry *uiSpin_ce;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);
extern int php_ui_call(zend_fcall_info *fci, zend_fcall_info_cache *fcc);

zend_object* php_ui_spin_create(zend_class_entry *ce) {
	php_ui_spin_t *spin = 
		(php_ui_spin_t*) ecalloc(1, sizeof(php_ui_spin_t) + zend_object_properties_size(ce));

	zend_object_std_init(&spin->std, ce);

	object_properties_init(&spin->std, ce);

	spin->std.handlers = &php_ui_spin_handlers;

	php_ui_set_call(&spin->std, ZEND_STRL("onchange"), &spin->change.fci, &spin->change.fcc);

	return &spin->std;
}

void php_ui_spin_change_handler(uiSpinbox *m,  void *_spin) {
	php_ui_spin_t *spin = (php_ui_spin_t*) _spin;

	if (spin->change.fci.size) {
		zval rv;

		ZVAL_UNDEF(&rv);

		spin->change.fci.retval = &rv;

		if (php_ui_call(&spin->change.fci, &spin->change.fcc) != SUCCESS) {
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

	uiSpinboxOnChanged(spin->s, php_ui_spin_change_handler, spin);
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

ZEND_BEGIN_ARG_INFO_EX(php_ui_spin_on_change_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Spin::onChange() */
PHP_METHOD(Spin, onChange)
{
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_spin_methods[] = {
	PHP_ME(Spin, __construct, php_ui_spin_construct_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Spin, setValue,    php_ui_spin_set_value_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Spin, getValue,    php_ui_spin_get_value_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Spin, onChange,    php_ui_spin_on_change_info,  ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Spin) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "Spin", php_ui_spin_methods);

	uiSpin_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiSpin_ce->create_object = php_ui_spin_create;

	memcpy(&php_ui_spin_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_spin_handlers.offset = XtOffsetOf(php_ui_spin_t, std);

	return SUCCESS;
} /* }}} */
#endif
