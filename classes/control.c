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
#ifndef HAVE_PHP_UI_CONTROL
#define HAVE_PHP_UI_CONTROL
#include <ui.h>

#include "php.h"

#include <classes/exceptions.h>
#include <classes/control.h>

zend_class_entry *uiControl_ce;

extern int php_ui_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t blen, zend_unserialize_data *data);
extern int php_ui_serialize(zval *object, unsigned char **buffer, size_t *buflen, zend_serialize_data *data);

void php_ui_control_set_controls(zend_object *std, HashTable *table) {
	zval obj;
	zval *controls, stacked;

	ZVAL_OBJ(&obj, std);
	
	controls = zend_read_property(std->ce, &obj, ZEND_STRL("controls"), 1, &stacked);

	if (!controls) {
		return;
	}

	ZVAL_ARR(controls, table);
}

zend_bool php_ui_control_set_parent(zval *child, zval *control) {
	zval *parent, stacked;
	php_ui_control_t *set;

	parent = zend_read_property(Z_OBJCE_P(child), child, ZEND_STRL("parent"), 1, &stacked);

	if (!parent) {
		return;
	}

	set = php_ui_control_fetch(child);

	if (set->parent) {
		php_ui_exception("cannot set parent on control which already has a parent");
		return 0;
	}

	ZVAL_COPY(parent, control);

	set->parent = Z_OBJ_P(control);

	return 1;
}

void php_ui_control_finalize(void) {
	uiControl_ce->ce_flags |= ZEND_ACC_FINAL;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_control_void_info, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_control_get_parent_info, 0, 0, IS_OBJECT, "UI\\Control", 1)
ZEND_END_ARG_INFO()

/* {{{ proto Control Control::getParent(void) */
PHP_METHOD(Control, getParent)
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}
	
	if (!ctrl->parent) {
		return;
	}

	ZVAL_OBJ(return_value, ctrl->parent);
	Z_ADDREF_P(return_value);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_control_set_parent_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, parent, UI\\Control, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Control::setParent(Control parent) */
PHP_METHOD(Control, setParent)
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis()),
					 *pctrl;
	zval *parent = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &parent, uiControl_ce) != SUCCESS) {
		return;
	}

	pctrl = php_ui_control_fetch(parent);

	if (!php_ui_control_set_parent(getThis(), parent)) {
		return;
	}

	uiControlSetParent(ctrl->control, pctrl->control);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_control_get_top_level_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Control::getTopLevel(void) */
PHP_METHOD(Control, getTopLevel) 
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(uiControlToplevel(ctrl->control));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_control_is_visible_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Control::isVisible(void) */
PHP_METHOD(Control, isVisible) 
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiControlVisible(ctrl->control)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

/* {{{ proto void Control::show(void) */
PHP_METHOD(Control, show) 
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlShow(ctrl->control);
} /* }}} */

/* {{{ proto void Control::hide(void) */
PHP_METHOD(Control, hide) 
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlHide(ctrl->control);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_control_is_enabled_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Control::isEnabled(void) */
PHP_METHOD(Control, isEnabled) 
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiControlEnabled(ctrl->control)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

/* {{{ proto void Control::enable(void) */
PHP_METHOD(Control, enable) 
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlEnable(ctrl->control);
} /* }}} */

/* {{{ proto void Control::disable(void) */
PHP_METHOD(Control, disable) 
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlDisable(ctrl->control);
} /* }}} */

/* {{{ proto void Control::destroy(void) */
PHP_METHOD(Control, destroy)
{
	php_ui_control_t *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlDestroy(ctrl->control);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_control_methods[] = {
	PHP_ME(Control, getParent,   php_ui_control_get_parent_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Control, setParent,   php_ui_control_set_parent_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Control, getTopLevel, php_ui_control_get_top_level_info,   ZEND_ACC_PUBLIC)
	PHP_ME(Control, isVisible,   php_ui_control_is_visible_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Control, show,        php_ui_control_void_info,            ZEND_ACC_PUBLIC)
	PHP_ME(Control, hide,        php_ui_control_void_info,            ZEND_ACC_PUBLIC)
	PHP_ME(Control, isEnabled,   php_ui_control_is_enabled_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Control, enable,      php_ui_control_void_info,            ZEND_ACC_PUBLIC)
	PHP_ME(Control, disable,     php_ui_control_void_info,            ZEND_ACC_PUBLIC)
	PHP_ME(Control, destroy,     php_ui_control_void_info,            ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Control) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Control", php_ui_control_methods);

	uiControl_ce = zend_register_internal_class(&ce);
	uiControl_ce->serialize = php_ui_serialize;
	uiControl_ce->unserialize = php_ui_unserialize;
	uiControl_ce->ce_flags |= ZEND_ACC_ABSTRACT;

	zend_declare_property_null(uiControl_ce, ZEND_STRL("parent"), ZEND_ACC_PROTECTED);

	return SUCCESS;
} /* }}} */


#endif
