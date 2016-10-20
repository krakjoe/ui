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
#ifndef HAVE_PHP_UI_SEPARATOR
#define HAVE_PHP_UI_SEPARATOR
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/separator.h>

zend_object_handlers php_ui_separator_handlers;

zend_object* php_ui_separator_create(zend_class_entry *ce) {
	php_ui_separator_t *separator = 
		(php_ui_separator_t*) ecalloc(1, sizeof(php_ui_separator_t) + zend_object_properties_size(ce));

	zend_object_std_init(&separator->std, ce);

	object_properties_init(&separator->std, ce);

	separator->std.handlers = &php_ui_separator_handlers;

	return &separator->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_separator_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Separator Separator::__construct(int type == SEPARATOR::HORIZONTAL) */
PHP_METHOD(Separator, __construct) 
{
	php_ui_separator_t *separator = php_ui_separator_fetch(getThis());
	zend_long type = PHP_UI_SEPARATOR_HORIZONTAL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &type) != SUCCESS) {
		return;
	}

	switch (type) {
		case PHP_UI_SEPARATOR_HORIZONTAL:
			separator->s = uiNewHorizontalSeparator();
		break;

		case PHP_UI_SEPARATOR_VERTICAL:
			separator->s = uiNewVerticalSeparator();
		break;
		
		default:
			return;
	}

	separator->type = type;
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_separator_methods[] = {
	PHP_ME(Separator, __construct, php_ui_separator_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Separator) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Separator", php_ui_separator_methods);

	uiSeparator_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiSeparator_ce->create_object = php_ui_separator_create;

	memcpy(&php_ui_separator_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_separator_handlers.offset = XtOffsetOf(php_ui_separator_t, std);

	zend_declare_class_constant_long(uiSeparator_ce, ZEND_STRL("HORIZONTAL"), PHP_UI_SEPARATOR_HORIZONTAL);
	zend_declare_class_constant_long(uiSeparator_ce, ZEND_STRL("VERTICAL"), PHP_UI_SEPARATOR_VERTICAL);

	return SUCCESS;
} /* }}} */
#endif
