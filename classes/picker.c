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
#ifndef HAVE_PHP_UI_PICKER
#define HAVE_PHP_UI_PICKER
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/picker.h>

zend_object_handlers php_ui_picker_handlers;

zend_object* php_ui_picker_create(zend_class_entry *ce) {
	php_ui_picker_t *picker = 
		(php_ui_picker_t*) ecalloc(1, sizeof(php_ui_picker_t));

	zend_object_std_init(&picker->std, ce);

	object_properties_init(&picker->std, ce);

	picker->std.handlers = &php_ui_picker_handlers;

	return &picker->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_picker_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Picker Picker::__construct(int type == PICKER::DATE) */
PHP_METHOD(Picker, __construct) 
{
	php_ui_picker_t *picker = php_ui_picker_fetch(getThis());
	zend_long type = PHP_UI_PICKER_DATE;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &type) != SUCCESS) {
		return;
	}

	switch (type) {
		case PHP_UI_PICKER_DATE:
			picker->p = uiNewDatePicker();
		break;

		case PHP_UI_PICKER_TIME:
			picker->p = uiNewTimePicker();
		break;

		case PHP_UI_PICKER_DATETIME:
			picker->p = uiNewDateTimePicker();
		break;
		
		default:
			return;
	}

	picker->type = type;
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_picker_methods[] = {
	PHP_ME(Picker, __construct, php_ui_picker_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Picker) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Picker", php_ui_picker_methods);

	uiPicker_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiPicker_ce->create_object = php_ui_picker_create;

	memcpy(&php_ui_picker_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_picker_handlers.offset = XtOffsetOf(php_ui_picker_t, std);

	zend_declare_class_constant_long(uiPicker_ce, ZEND_STRL("DATE"),     PHP_UI_PICKER_DATE);
	zend_declare_class_constant_long(uiPicker_ce, ZEND_STRL("TIME"),     PHP_UI_PICKER_TIME);
	zend_declare_class_constant_long(uiPicker_ce, ZEND_STRL("DATETIME"), PHP_UI_PICKER_DATETIME);

	return SUCCESS;
} /* }}} */
#endif
