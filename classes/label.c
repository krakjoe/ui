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
#ifndef HAVE_PHP_UI_LABEL
#define HAVE_PHP_UI_LABEL
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/label.h>

zend_object_handlers php_ui_label_handlers;

zend_object* php_ui_label_create(zend_class_entry *ce) {
	php_ui_label_t *label = 
		(php_ui_label_t*) ecalloc(1, sizeof(php_ui_label_t) + zend_object_properties_size(ce));

	zend_object_std_init(&label->std, ce);

	object_properties_init(&label->std, ce);

	label->std.handlers = &php_ui_label_handlers;

	return &label->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_label_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Label Label::__construct(string text) */
PHP_METHOD(Label, __construct) 
{
	php_ui_label_t *label = php_ui_label_fetch(getThis());
	zend_string *text = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	label->l = uiNewLabel(ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_label_set_text_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Label::setText(string text) */
PHP_METHOD(Label, setText) 
{
	php_ui_label_t *label = php_ui_label_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiLabelSetText(label->l, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_label_get_text_info, 0, 0, IS_STRING, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Label::getText(void) */
PHP_METHOD(Label, getText) 
{
	php_ui_label_t *label = php_ui_label_fetch(getThis());
	char *text = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(uiLabelText(label->l));	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_label_methods[] = {
	PHP_ME(Label, __construct, php_ui_label_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Label, setText,     php_ui_label_set_text_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Label, getText,     php_ui_label_get_text_info,  ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Label) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Control", "Label", php_ui_label_methods);

	uiLabel_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiLabel_ce->create_object = php_ui_label_create;
	uiLabel_ce->ce_flags |= ZEND_ACC_FINAL;

	memcpy(&php_ui_label_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_label_handlers.offset = XtOffsetOf(php_ui_label_t, std);

	return SUCCESS;
} /* }}} */
#endif
