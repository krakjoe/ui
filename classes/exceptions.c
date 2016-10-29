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
#ifndef HAVE_PHP_UI_EXCEPTIONS
#define HAVE_PHP_UI_EXCEPTIONS
#include <ui.h>

#include "php.h"

#include "ext/spl/spl_exceptions.h"

zend_class_entry *uiInvalidArgumentException_ce;
zend_class_entry *uiRuntimeException_ce;

PHP_MINIT_FUNCTION(UI_Exceptions)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\Exception", "InvalidArgumentException", NULL);

	uiInvalidArgumentException_ce = zend_register_internal_class_ex(&ce, spl_ce_InvalidArgumentException);

	INIT_NS_CLASS_ENTRY(ce, "UI\Exception", "RuntimeException", NULL);

	uiRuntimeException_ce = zend_register_internal_class_ex(&ce, spl_ce_RuntimeException);
}
#endif
