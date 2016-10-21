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

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);

zend_object* php_ui_entry_create(zend_class_entry *ce) {
	php_ui_entry_t *entry = 
		(php_ui_entry_t*) ecalloc(1, sizeof(php_ui_entry_t) + zend_object_properties_size(ce));

	zend_object_std_init(&entry->std, ce);

	object_properties_init(&entry->std, ce);

	entry->std.handlers = &php_ui_entry_handlers;

	php_ui_set_call(&entry->std, ZEND_STRL("onchange"), &entry->change.fci, &entry->change.fcc);

	return &entry->std;
}

void php_ui_entry_change_handler(uiEntry *u, void *_entry) {
	php_ui_entry_t *entry = (php_ui_entry_t*) _entry;

	if (entry->change.fci.size) {
		zval rv;

		ZVAL_UNDEF(&rv);

		entry->change.fci.retval = &rv;

		if (zend_call_function(&entry->change.fci, &entry->change.fcc) != SUCCESS) {
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

	uiEntryOnChanged(entry->e, php_ui_entry_change_handler, entry);

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

ZEND_BEGIN_ARG_INFO_EX(php_ui_entry_on_change_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Entry::onChange() */
PHP_METHOD(Entry, onChange)
{
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

	if (uiEntryReadOnly(entry->e)) {
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
	PHP_ME(Entry, setReadOnly, php_ui_entry_set_read_only_info, ZEND_ACC_PUBLIC)
	PHP_ME(Entry, isReadOnly,  NULL,                            ZEND_ACC_PUBLIC)
	PHP_ME(Entry, onChange,    php_ui_entry_on_change_info,     ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Entry) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Control", "Entry", php_ui_entry_methods);

	uiEntry_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiEntry_ce->create_object = php_ui_entry_create;

	memcpy(&php_ui_entry_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_entry_handlers.offset = XtOffsetOf(php_ui_entry_t, std);

	zend_declare_class_constant_long(uiEntry_ce, ZEND_STRL("NORMAL"), PHP_UI_ENTRY_NORMAL);
	zend_declare_class_constant_long(uiEntry_ce, ZEND_STRL("PASSWORD"), PHP_UI_ENTRY_PASSWORD);
	zend_declare_class_constant_long(uiEntry_ce, ZEND_STRL("SEARCH"), PHP_UI_ENTRY_SEARCH);

	return SUCCESS;
} /* }}} */
#endif
