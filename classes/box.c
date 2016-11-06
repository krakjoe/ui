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

#include <classes/exceptions.h>
#include <classes/control.h>
#include <classes/box.h>

zend_object_handlers php_ui_box_handlers;

zend_class_entry *uiBox_ce;

#define PHP_UI_BOX_CONTROL_CHECK(box, control) do { \
	if (control < 0 || control >= zend_hash_num_elements(box->controls)) { \
		php_ui_exception_ex( \
			InvalidArgumentException, "control %ld does not exist", control); \
		return; \
	} \
} while(0)

zend_object* php_ui_box_create(zend_class_entry *ce) {
	php_ui_box_t *box = 
		(php_ui_box_t*) ecalloc(1, sizeof(php_ui_box_t) + zend_object_properties_size(ce));

	zend_object_std_init(&box->std, ce);

	object_properties_init(&box->std, ce);

	box->std.handlers = &php_ui_box_handlers;

	ALLOC_HASHTABLE(box->controls);

	zend_hash_init(box->controls, 8, NULL, ZVAL_PTR_DTOR, 0);

	php_ui_control_set_controls(&box->std, box->controls);

	return &box->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_box_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, orientation, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Box Box::__construct(int orientation = BOX::HORIZONTAL) */
PHP_METHOD(Box, __construct) 
{
	php_ui_box_t *box = php_ui_box_fetch(getThis());

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

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_box_append_info, 0, 1, IS_LONG, NULL, 0)
	ZEND_ARG_OBJ_INFO(0, control, UI\\Control, 0)
	ZEND_ARG_TYPE_INFO(0, stretchy, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Box::append(Control control [, bool stretchy = false]) */
PHP_METHOD(Box, append)
{
	php_ui_box_t *box = php_ui_box_fetch(getThis());
	zval *control = NULL;
	zend_bool stretchy = 0;
	php_ui_control_t *ctrl;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|b", &control, uiControl_ce, &stretchy) != SUCCESS) {
		return;
	}

	ctrl = php_ui_control_fetch(control);

	if (!php_ui_control_set_parent(control, getThis())) {
		return;
	}

	uiBoxAppend(box->b, ctrl->control, stretchy);

	if (zend_hash_next_index_insert(box->controls, control)) {
		Z_ADDREF_P(control);
	}

	RETURN_LONG(zend_hash_num_elements(box->controls) - 1);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_box_delete_info, 0, 1, _IS_BOOL, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Box::delete(int index) */
PHP_METHOD(Box, delete)
{
	php_ui_box_t *box = php_ui_box_fetch(getThis());
	zend_long index = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &index) != SUCCESS) {
		return;
	}

	PHP_UI_BOX_CONTROL_CHECK(box, index);

	if (zend_hash_index_del(box->controls, index)) {
		uiBoxDelete(box->b, (int) index);

		RETURN_TRUE;
	}

	RETURN_FALSE;
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

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "Box", php_ui_box_methods);

	uiBox_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiBox_ce->create_object = php_ui_box_create;
	zend_declare_property_null(uiBox_ce, ZEND_STRL("controls"), ZEND_ACC_PROTECTED);

	memcpy(&php_ui_box_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_box_handlers.offset = XtOffsetOf(php_ui_box_t, std);

	zend_declare_class_constant_long(uiBox_ce, ZEND_STRL("Vertical"), PHP_UI_BOX_VERTICAL);
	zend_declare_class_constant_long(uiBox_ce, ZEND_STRL("Horizontal"), PHP_UI_BOX_HORIZONTAL);

	return SUCCESS;
} /* }}} */
#endif
