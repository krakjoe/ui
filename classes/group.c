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
#ifndef HAVE_PHP_UI_GROUP
#define HAVE_PHP_UI_GROUP
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/group.h>

zend_object_handlers php_ui_group_handlers;

zend_object* php_ui_group_create(zend_class_entry *ce) {
	php_ui_group_t *group = 
		(php_ui_group_t*) ecalloc(1, sizeof(php_ui_group_t));

	zend_object_std_init(&group->std, ce);

	object_properties_init(&group->std, ce);

	group->std.handlers = &php_ui_group_handlers;

	return &group->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_group_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Group Group::__construct(string title) */
PHP_METHOD(Group, __construct) 
{
	php_ui_group_t *group = php_ui_group_fetch(getThis());
	zend_string *title = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &title) != SUCCESS) {
		return;
	}

	group->g = uiNewGroup(ZSTR_VAL(title));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_group_set_title_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Group::setTitle(string title) */
PHP_METHOD(Group, setTitle) 
{
	php_ui_group_t *group = php_ui_group_fetch(getThis());
	zend_string *title = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &title) != SUCCESS) {
		return;
	}

	uiGroupSetTitle(group->g, ZSTR_VAL(title));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_group_get_title_info, 0, 0, IS_STRING, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Group::getTitle(void) */
PHP_METHOD(Group, getTitle) 
{
	php_ui_group_t *group = php_ui_group_fetch(getThis());
	char *text = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(uiGroupTitle(group->g));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_group_set_margins_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, margins, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Group::setMargins(bool margins) */
PHP_METHOD(Group, setMargins) 
{
	php_ui_group_t *group = php_ui_group_fetch(getThis());
	zend_bool margins = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &margins) != SUCCESS) {
		return;
	}

	uiGroupSetMargined(group->g, (int) margins);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_group_has_margins_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Group::hasMargins(void) */
PHP_METHOD(Group, hasMargins) 
{
	php_ui_group_t *group = php_ui_group_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiGroupMargined(group->g)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_group_add_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, control, UI\\Control, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Group::add(Control control) */
PHP_METHOD(Group, add)
{
	php_ui_group_t *group = php_ui_group_fetch(getThis());
	zval *control = NULL;
	php_ui_control_t *ctrl;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &control, uiControl_ce) != SUCCESS) {
		return;
	}

	ctrl = php_ui_control_fetch(control);

	uiGroupSetChild(group->g, uiControl(ctrl->c));
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_group_methods[] = {
	PHP_ME(Group, __construct,  php_ui_group_construct_info,   ZEND_ACC_PUBLIC)
	PHP_ME(Group, setTitle,     php_ui_group_set_title_info,   ZEND_ACC_PUBLIC)
	PHP_ME(Group, getTitle,     php_ui_group_get_title_info,   ZEND_ACC_PUBLIC)
	PHP_ME(Group, setMargins,   php_ui_group_set_margins_info, ZEND_ACC_PUBLIC)
	PHP_ME(Group, hasMargins,   php_ui_group_has_margins_info, ZEND_ACC_PUBLIC)
	PHP_ME(Group, add,          php_ui_group_add_info,         ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Group) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Group", php_ui_group_methods);

	uiGroup_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiGroup_ce->create_object = php_ui_group_create;

	memcpy(&php_ui_group_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_group_handlers.offset = XtOffsetOf(php_ui_group_t, std);

	return SUCCESS;
} /* }}} */
#endif
