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
#ifndef HAVE_PHP_UI_CONTEXT
#define HAVE_PHP_UI_CONTEXT
#include <ui.h>

#include "php.h"

#include <classes/context.h>

zend_object_handlers php_ui_context_handlers;

zend_object* php_ui_context_create(zend_class_entry *ce) {
	php_ui_context_t *context = 
		(php_ui_context_t*) ecalloc(1, sizeof(php_ui_context_t));

	zend_object_std_init(&context->std, ce);

	object_properties_init(&context->std, ce);

	context->std.handlers = &php_ui_context_handlers;

	return &context->std;
}

zval* php_ui_context_construct(zval *context, uiDrawContext *c) {
	php_ui_context_t *ctxt;

	object_init_ex(context, uiDrawContext_ce);
	
	ctxt = php_ui_context_fetch(context);
	ctxt->c = c;

	return context;
}

/* {{{ */
const zend_function_entry php_ui_context_methods[] = {
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawContext) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "DrawContext", php_ui_context_methods);

	uiDrawContext_ce = zend_register_internal_class(&ce);
	uiDrawContext_ce->create_object = php_ui_context_create;

	memcpy(&php_ui_context_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_context_handlers.offset = XtOffsetOf(php_ui_context_t, std);

	return SUCCESS;
} /* }}} */
#endif
