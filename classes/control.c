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

#include <classes/control.h>

extern int php_ui_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t blen, zend_unserialize_data *data);
extern int php_ui_serialize(zval *object, unsigned char **buffer, size_t *buflen, zend_serialize_data *data);

ZEND_BEGIN_ARG_INFO_EX(php_ui_control_void_info, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_control_get_parent_info, 0, 0, IS_OBJECT, "UI\\Control", 1)
ZEND_END_ARG_INFO()

/* {{{ proto Control Control::getParent(void) */
PHP_METHOD(Control, getParent)
{
	uiControl *ctrl = php_ui_control_fetch(getThis());
	php_ui_control_t *parent;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}
	
	if (!uiControlParent(ctrl)) {
		return;
	}

	object_init_ex(return_value, uiControl_ce);

	parent = (php_ui_control_t*) php_ui_control_fetch(return_value);

	parent->c = uiControlParent(ctrl);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_control_set_parent_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, parent, UI\\Control, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Control::setParent(Control parent) */
PHP_METHOD(Control, setParent)
{
	uiControl *ctrl = php_ui_control_fetch(getThis()),
					 *pctrl;
	zval *parent = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &parent, uiControl_ce) != SUCCESS) {
		return;
	}

	pctrl = php_ui_control_fetch(parent);

	uiControlSetParent(ctrl, pctrl);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_control_get_top_level_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Control::getTopLevel(void) */
PHP_METHOD(Control, getTopLevel) 
{
	uiControl *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(uiControlTopLevel(ctrl));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_control_is_visible_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Control::isVisible(void) */
PHP_METHOD(Control, isVisible) 
{
	uiControl *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiControlVisible(ctrl)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

/* {{{ proto void Control::show(void) */
PHP_METHOD(Control, show) 
{
	uiControl *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlShow(ctrl);
} /* }}} */

/* {{{ proto void Control::hide(void) */
PHP_METHOD(Control, hide) 
{
	uiControl *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlHide(ctrl);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_control_is_enabled_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Control::isEnabled(void) */
PHP_METHOD(Control, isEnabled) 
{
	uiControl *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiControlEnabled(ctrl)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

/* {{{ proto void Control::enable(void) */
PHP_METHOD(Control, enable) 
{
	uiControl *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlEnable(ctrl);
} /* }}} */

/* {{{ proto void Control::disable(void) */
PHP_METHOD(Control, disable) 
{
	uiControl *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlDisable(ctrl);
} /* }}} */

/* {{{ proto void Control::destroy(void) */
PHP_METHOD(Control, destroy)
{
	uiControl *ctrl = php_ui_control_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiControlDestroy(ctrl);
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

	return SUCCESS;
} /* }}} */
#endif
