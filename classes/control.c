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

zend_object_handlers php_ui_control_handlers;

/* {{{ */
const zend_function_entry php_ui_control_methods[] = {

	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Control) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Control", php_ui_control_methods);

	uiControl_ce = zend_register_internal_class(&ce);

	memcpy(&php_ui_control_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_control_handlers.offset = XtOffsetOf(php_ui_control_t, std);

	return SUCCESS;
} /* }}} */
#endif
