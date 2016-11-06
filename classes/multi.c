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
#ifndef HAVE_PHP_UI_MULTI
#define HAVE_PHP_UI_MULTI
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/multi.h>

zend_object_handlers php_ui_multi_handlers;

zend_class_entry *uiMulti_ce;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);
extern int php_ui_call(zend_fcall_info *fci, zend_fcall_info_cache *fcc);

zend_object* php_ui_multi_create(zend_class_entry *ce) {
	php_ui_multi_t *multi = 
		(php_ui_multi_t*) ecalloc(1, sizeof(php_ui_multi_t) + zend_object_properties_size(ce));

	zend_object_std_init(&multi->std, ce);

	object_properties_init(&multi->std, ce);

	multi->std.handlers = &php_ui_multi_handlers;

	php_ui_set_call(&multi->std, ZEND_STRL("onchange"), &multi->change.fci, &multi->change.fcc);

	return &multi->std;
}

void php_ui_multi_change_handler(uiMultilineEntry *m,  void *_multi) {
	php_ui_multi_t *multi = (php_ui_multi_t*) _multi;

	if (multi->change.fci.size) {
		zval rv;

		ZVAL_UNDEF(&rv);

		multi->change.fci.retval = &rv;

		if (php_ui_call(&multi->change.fci, &multi->change.fcc) != SUCCESS) {
			return;
		}

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_multi_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Multi Multi::__construct(int type = MULTI::WRAP) */
PHP_METHOD(Multi, __construct) 
{
	php_ui_multi_t *multi = php_ui_multi_fetch(getThis());
	zend_long type = PHP_UI_MULTI_WRAP;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &type) != SUCCESS) {
		return;
	}

	switch (type) {
		case PHP_UI_MULTI_WRAP:
			multi->e = uiNewMultilineEntry();
		break;

		case PHP_UI_MULTI_NOWRAP:
			multi->e = uiNewNonWrappingMultilineEntry();
		break;

		default:
			/* throw */
			return;
	}

	uiMultilineEntryOnChanged(multi->e, php_ui_multi_change_handler, multi);

	multi->type = type;
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_multi_set_text_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Multi::setText(string text) */
PHP_METHOD(Multi, setText) 
{
	php_ui_multi_t *multi = php_ui_multi_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiMultilineEntrySetText(multi->e, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_multi_get_text_info, 0, 0, IS_STRING, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Multi::getText(void) */
PHP_METHOD(Multi, getText) 
{
	php_ui_multi_t *multi = php_ui_multi_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(uiMultilineEntryText(multi->e));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_multi_append_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Multi::append(string text) */
PHP_METHOD(Multi, append) 
{
	php_ui_multi_t *multi = php_ui_multi_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiMultilineEntryAppend(multi->e, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_multi_on_change_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Multi::onChange(callable handler) */
PHP_METHOD(Multi, onChange)
{
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_multi_set_read_only_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, readOnly, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Multi::setReadOnly(bool readOnly) */
PHP_METHOD(Multi, setReadOnly) 
{
	php_ui_multi_t *multi = php_ui_multi_fetch(getThis());
	zend_bool readOnly = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &readOnly) != SUCCESS) {
		return;
	}

	uiMultilineEntrySetReadOnly(multi->e, readOnly);
} /* }}} */

/* {{{ proto void Multi::isReadOnly(void) */
PHP_METHOD(Multi, isReadOnly) 
{
	php_ui_multi_t *multi = php_ui_multi_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;	
	}

	if (uiMultilineEntryReadOnly(multi->e)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_multi_methods[] = {
	PHP_ME(Multi, __construct, php_ui_multi_construct_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Multi, setText,     php_ui_multi_set_text_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Multi, getText,     php_ui_multi_get_text_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Multi, append,      php_ui_multi_append_info,        ZEND_ACC_PUBLIC)
	PHP_ME(Multi, setReadOnly, php_ui_multi_set_read_only_info, ZEND_ACC_PUBLIC)
	PHP_ME(Multi, isReadOnly,  NULL,                            ZEND_ACC_PUBLIC)
	PHP_ME(Multi, onChange,    php_ui_multi_on_change_info,     ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Multi) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "MultilineEntry", php_ui_multi_methods);

	uiMulti_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiMulti_ce->create_object = php_ui_multi_create;

	memcpy(&php_ui_multi_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_multi_handlers.offset = XtOffsetOf(php_ui_multi_t, std);

	zend_declare_class_constant_long(uiMulti_ce, ZEND_STRL("Wrap"), PHP_UI_MULTI_WRAP);
	zend_declare_class_constant_long(uiMulti_ce, ZEND_STRL("NoWrap"), PHP_UI_MULTI_NOWRAP);

	return SUCCESS;
} /* }}} */
#endif
