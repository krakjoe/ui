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
#ifndef HAVE_PHP_UI_BOX
#define HAVE_PHP_UI_BOX
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/box.h>

zend_object_handlers php_ui_box_handlers;

zend_object* php_ui_box_create(zend_class_entry *ce) {
	php_ui_box_t *box = 
		(php_ui_box_t*) ecalloc(1, sizeof(php_ui_box_t) + zend_object_properties_size(ce));

	zend_object_std_init(&box->std, ce);

	object_properties_init(&box->std, ce);

	box->std.handlers = &php_ui_box_handlers;

	return &box->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_box_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, orientation, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Box Box::__construct(int orientation = BOX::HORIZONTAL) */
PHP_METHOD(Box, __construct) 
{
	php_ui_box_t *box = php_ui_box_fetch(getThis());
	zend_long orientation = PHP_UI_BOX_HORIZONTAL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &box->orientation) != SUCCESS) {
		return;
	}

	switch (box->orientation) {
		case PHP_UI_BOX_HORIZONTAL:
			box->b = uiNewHorizontalBox();
		break;

		case PHP_UI_BOX_VERTICAL:
			box->b = uiNewVerticalBox();
		break;

		default:
			{ /* throw */ }
	}
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_box_get_orientation_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Box::getOrientation(void) */
PHP_METHOD(Box, getOrientation)
{
	php_ui_box_t *box = php_ui_box_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}
	
	RETURN_LONG(box->orientation);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_box_append_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, control, UI\\Control, 0)
	ZEND_ARG_TYPE_INFO(0, stretchy, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Box::append(Control control [, bool stretchy = 0]) */
PHP_METHOD(Box, append)
{
	php_ui_box_t *box = php_ui_box_fetch(getThis());
	zval *control = NULL;
	zend_bool stretchy = 0;
	uiControl *ctrl;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|b", &control, uiControl_ce, &stretchy) != SUCCESS) {
		return;
	}

	ctrl = php_ui_control_fetch(control);

	uiBoxAppend(box->b, ctrl, stretchy);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_box_delete_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Box::delete(int index) */
PHP_METHOD(Box, delete)
{
	php_ui_box_t *box = php_ui_box_fetch(getThis());
	zend_long index = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &index) != SUCCESS) {
		return;
	}

	uiBoxDelete(box->b, (int) index);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_box_set_padded_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, padded, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Box::setPadded(bool padded) */
PHP_METHOD(Box, setPadded) 
{
	php_ui_box_t *box = php_ui_box_fetch(getThis());
	zend_bool padded = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &padded) != SUCCESS) {
		return;
	}

	uiBoxSetPadded(box->b, padded);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_box_is_padded_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Box::isPadded(void) */
PHP_METHOD(Box, isPadded) 
{
	php_ui_box_t *box = php_ui_box_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiBoxPadded(box->b)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_box_methods[] = {
	PHP_ME(Box, __construct,     php_ui_box_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Box, getOrientation,  php_ui_box_get_orientation_info, ZEND_ACC_PUBLIC)
	PHP_ME(Box, append,          php_ui_box_append_info, ZEND_ACC_PUBLIC)
	PHP_ME(Box, delete,          php_ui_box_delete_info, ZEND_ACC_PUBLIC)
	PHP_ME(Box, setPadded,       php_ui_box_set_padded_info, ZEND_ACC_PUBLIC)
	PHP_ME(Box, isPadded,        php_ui_box_is_padded_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Box) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Box", php_ui_box_methods);

	uiBox_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiBox_ce->create_object = php_ui_box_create;

	memcpy(&php_ui_box_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_box_handlers.offset = XtOffsetOf(php_ui_box_t, std);

	zend_declare_class_constant_long(uiBox_ce, ZEND_STRL("VERTICAL"), PHP_UI_BOX_VERTICAL);
	zend_declare_class_constant_long(uiBox_ce, ZEND_STRL("HORIZONTAL"), PHP_UI_BOX_HORIZONTAL);

	return SUCCESS;
} /* }}} */
#endif
