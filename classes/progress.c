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
#ifndef HAVE_PHP_UI_PROGRESS
#define HAVE_PHP_UI_PROGRESS
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/progress.h>

zend_object_handlers php_ui_progress_handlers;

zend_class_entry *uiProgress_ce;

zend_object* php_ui_progress_create(zend_class_entry *ce) {
	php_ui_progress_t *progress = 
		(php_ui_progress_t*) ecalloc(1, sizeof(php_ui_progress_t) + zend_object_properties_size(ce));

	zend_object_std_init(&progress->std, ce);

	object_properties_init(&progress->std, ce);

	progress->std.handlers = &php_ui_progress_handlers;

	progress->p = uiNewProgressBar();

	return &progress->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_progress_set_value_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Progress::setValue(int value) */
PHP_METHOD(Progress, setValue) 
{
	php_ui_progress_t *progress = php_ui_progress_fetch(getThis());
	zend_long value = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &value) != SUCCESS) {
		return;
	}

	uiProgressBarSetValue(progress->p, value);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_progress_get_value_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Progress::getValue(void) */
PHP_METHOD(Progress, getValue) 
{
	php_ui_progress_t *progress = php_ui_progress_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(uiProgressBarValue(progress->p));	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_progress_methods[] = {
	PHP_ME(Progress, setValue,    php_ui_progress_set_value_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Progress, getValue,    php_ui_progress_get_value_info,  ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Progress) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "Progress", php_ui_progress_methods);

	uiProgress_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiProgress_ce->create_object = php_ui_progress_create;

	memcpy(&php_ui_progress_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_progress_handlers.offset = XtOffsetOf(php_ui_progress_t, std);

	return SUCCESS;
} /* }}} */
#endif
