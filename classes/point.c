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

#include <classes/point.h>

zend_object_handlers php_ui_point_handlers;

zval *php_ui_point_construct(zval *point, double x, double y)
{
	php_ui_point_t *p;

	object_init_ex(point, uiPoint_ce);

	p = php_ui_point_fetch(point);

	p->x = x;
	p->y = y;

	return point;
}

zend_object* php_ui_point_create(zend_class_entry *ce) {
	php_ui_point_t *point = 
		(php_ui_point_t*) ecalloc(1, sizeof(php_ui_point_t));

	zend_object_std_init(&point->std, ce);

	object_properties_init(&point->std, ce);

	point->std.handlers = &php_ui_point_handlers;

	return &point->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_point_construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Point Point::__construct(string text) */
PHP_METHOD(Point, __construct) 
{
	php_ui_point_t *point = php_ui_point_fetch(getThis());
	double x = 0, y = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) != SUCCESS) {
		return;
	}

	point->x = x;
	point->y = y;
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_point_get_point_info, 0, 0, IS_DOUBLE, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto double Point::getX(void) */
PHP_METHOD(Point, getX) 
{
	php_ui_point_t *point = php_ui_point_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(point->x);	
} /* }}} */

/* {{{ proto double Point::getY(void) */
PHP_METHOD(Point, getY) 
{
	php_ui_point_t *point = php_ui_point_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(point->y);	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_point_set_point_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, point, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Point::setX(double point) */
PHP_METHOD(Point, setX)
{
	php_ui_point_t *point = php_ui_point_fetch(getThis());
	double x = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &x) != SUCCESS) {
		return;
	}

	point->x = x;
} /* }}} */

/* {{{ proto void Point::setY(double point) */
PHP_METHOD(Point, setY)
{
	php_ui_point_t *point = php_ui_point_fetch(getThis());
	double y = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &y) != SUCCESS) {
		return;
	}

	point->y = y;
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_point_methods[] = {
	PHP_ME(Point, __construct, php_ui_point_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Point, getX,        php_ui_point_get_point_info, ZEND_ACC_PUBLIC)
	PHP_ME(Point, getY,        php_ui_point_get_point_info, ZEND_ACC_PUBLIC)
	PHP_ME(Point, setX,        php_ui_point_set_point_info, ZEND_ACC_PUBLIC)
	PHP_ME(Point, setY,        php_ui_point_set_point_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Point) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Point", php_ui_point_methods);

	uiPoint_ce = zend_register_internal_class(&ce);
	uiPoint_ce->create_object = php_ui_point_create;

	memcpy(&php_ui_point_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_point_handlers.offset = XtOffsetOf(php_ui_point_t, std);

	return SUCCESS;
} /* }}} */
#endif
