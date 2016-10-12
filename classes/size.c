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

#include <classes/size.h>

zend_object_handlers php_ui_size_handlers;

zval *php_ui_size_construct(zval *size, double width, double height)
{
	php_ui_size_t *s;

	object_init_ex(size, uiSize_ce);

	s = php_ui_size_fetch(size);

	s->width = width;
	s->height = height;

	return size;
}

zend_object* php_ui_size_create(zend_class_entry *ce) {
	php_ui_size_t *size = 
		(php_ui_size_t*) ecalloc(1, sizeof(php_ui_size_t));

	zend_object_std_init(&size->std, ce);

	object_properties_init(&size->std, ce);

	size->std.handlers = &php_ui_size_handlers;

	return &size->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_size_construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Size Size::__construct(double width, double height) */
PHP_METHOD(Size, __construct) 
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());
	double width = 0, height = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &width, &height) != SUCCESS) {
		return;
	}

	size->width = width;
	size->height = height;
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_size_get_size_info, 0, 0, IS_DOUBLE, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto double Size::getWidth(void) */
PHP_METHOD(Size, getWidth) 
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(size->width);	
} /* }}} */

/* {{{ proto double Size::getHeight(void) */
PHP_METHOD(Size, getHeight) 
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(size->height);	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_size_set_size_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, size, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Size::setWidth(double size) */
PHP_METHOD(Size, setWidth)
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());
	double width = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &width) != SUCCESS) {
		return;
	}

	size->width = width;
} /* }}} */

/* {{{ proto void Size::setHeight(double size) */
PHP_METHOD(Size, setHeight)
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());
	double height = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &height) != SUCCESS) {
		return;
	}

	size->height = height;
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_size_methods[] = {
	PHP_ME(Size, __construct,     php_ui_size_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Size, getWidth,        php_ui_size_get_size_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Size, getHeight,       php_ui_size_get_size_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Size, setWidth,        php_ui_size_set_size_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Size, setHeight,       php_ui_size_set_size_info,  ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Size) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Size", php_ui_size_methods);

	uiSize_ce = zend_register_internal_class(&ce);
	uiSize_ce->create_object = php_ui_size_create;

	memcpy(&php_ui_size_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_size_handlers.offset = XtOffsetOf(php_ui_size_t, std);

	return SUCCESS;
} /* }}} */
#endif
