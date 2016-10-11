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
#ifndef HAVE_PHP_UI_ENTRY
#define HAVE_PHP_UI_ENTRY
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/entry.h>

zend_object_handlers php_ui_entry_handlers;

zend_object* php_ui_entry_create(zend_class_entry *ce) {
	php_ui_entry_t *entry = 
		(php_ui_entry_t*) ecalloc(1, sizeof(php_ui_entry_t));

	ZVAL_UNDEF(&entry->handler);

	zend_object_std_init(&entry->std, ce);

	object_properties_init(&entry->std, ce);

	entry->std.handlers = &php_ui_entry_handlers;

	return &entry->std;
}

void php_ui_entry_free(zend_object *o) {
	php_ui_entry_t *entry = php_ui_entry_from(o);

	if (Z_TYPE(entry->handler) != IS_UNDEF) {
		zval_ptr_dtor(&entry->handler);
	}

	zend_object_std_dtor(o);
}

void php_ui_entry_on_changed(uiEntry *e, void *_entry) {
	php_ui_entry_t *entry = (php_ui_entry_t*) _entry;

	if (Z_TYPE(entry->handler) != IS_UNDEF) {
		zval rv;
		zend_fcall_info fci = empty_fcall_info;
		zend_fcall_info_cache fcc = empty_fcall_info_cache;
		zend_string *cn = NULL;
		char *er = NULL;

		if (zend_fcall_info_init(&entry->handler, IS_CALLABLE_CHECK_SILENT, &fci, &fcc, &cn, &er) != SUCCESS) {
			return;
		}

		fci.retval = &rv;

		ZVAL_UNDEF(&rv);

		if (zend_call_function(&fci, &fcc) != SUCCESS) {
			return;
		}

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_entry_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Entry Entry::__construct(int type = ENTRY::NORMAL) */
PHP_METHOD(Entry, __construct) 
{
	php_ui_entry_t *entry = php_ui_entry_fetch(getThis());
	zend_long type = PHP_UI_ENTRY_NORMAL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &type) != SUCCESS) {
		return;
	}

	switch (type) {
		case PHP_UI_ENTRY_NORMAL:
			entry->e = uiNewEntry();
		break;

		case PHP_UI_ENTRY_PASSWORD:
			entry->e = uiNewPasswordEntry();
		break;

		case PHP_UI_ENTRY_SEARCH:
			entry->e = uiNewSearchEntry();
		break;

		default:
			/* throw */
			return;
	}

	uiEntryOnChanged(entry->e, php_ui_entry_on_changed, entry);

	entry->type = type;
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_entry_set_text_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Entry::setText(string text) */
PHP_METHOD(Entry, setText) 
{
	php_ui_entry_t *entry = php_ui_entry_fetch(getThis());
	zend_string *text = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	uiEntrySetText(entry->e, ZSTR_VAL(text));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_entry_get_text_info, 0, 0, IS_STRING, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Entry::getText(void) */
PHP_METHOD(Entry, getText) 
{
	php_ui_entry_t *entry = php_ui_entry_fetch(getThis());
	char *text = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(uiEntryText(entry->e));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_entry_on_change_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, handler, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Entry::onChange(callable handler) */
PHP_METHOD(Entry, onChange)
{
	php_ui_entry_t *entry = php_ui_entry_fetch(getThis());
	zval *handler = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &handler) != SUCCESS) {
		return;
	}

	if (Z_TYPE(entry->handler) != IS_UNDEF) {
		zval_ptr_dtor(&entry->handler);
	}

	ZVAL_COPY(&entry->handler, handler);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_entry_set_read_only_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, readOnly, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Entry::setReadOnly(bool readOnly) */
PHP_METHOD(Entry, setReadOnly) 
{
	php_ui_entry_t *entry = php_ui_entry_fetch(getThis());
	zend_bool readOnly = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &readOnly) != SUCCESS) {
		return;
	}

	uiEntrySetReadOnly(entry->e, readOnly);
} /* }}} */

/* {{{ proto void Entry::isReadOnly(void) */
PHP_METHOD(Entry, isReadOnly) 
{
	php_ui_entry_t *entry = php_ui_entry_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;	
	}

	if (uiEntryReadonly(entry->e)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_entry_methods[] = {
	PHP_ME(Entry, __construct, php_ui_entry_construct_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Entry, setText,     php_ui_entry_set_text_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Entry, getText,     php_ui_entry_get_text_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Entry, onChange,    php_ui_entry_on_change_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Entry, setReadOnly, php_ui_entry_set_read_only_info, ZEND_ACC_PUBLIC)
	PHP_ME(Entry, isReadOnly,  NULL,                            ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Entry) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Entry", php_ui_entry_methods);

	uiEntry_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiEntry_ce->create_object = php_ui_entry_create;

	memcpy(&php_ui_entry_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_entry_handlers.free_obj = php_ui_entry_free;
	php_ui_entry_handlers.offset = XtOffsetOf(php_ui_entry_t, std);

	zend_declare_class_constant_long(uiEntry_ce, ZEND_STRL("NORMAL"), PHP_UI_ENTRY_NORMAL);
	zend_declare_class_constant_long(uiEntry_ce, ZEND_STRL("PASSWORD"), PHP_UI_ENTRY_PASSWORD);
	zend_declare_class_constant_long(uiEntry_ce, ZEND_STRL("SEARCH"), PHP_UI_ENTRY_SEARCH);

	return SUCCESS;
} /* }}} */
#endif
