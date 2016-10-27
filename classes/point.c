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
#ifndef HAVE_PHP_UI_POINT
#define HAVE_PHP_UI_POINT
#include <ui.h>

#include "php.h"

#include <classes/common.h>

zend_object_handlers php_ui_point_handlers;

zend_class_entry *uiPoint_ce;

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
		(php_ui_point_t*) ecalloc(1, sizeof(php_ui_point_t) + zend_object_properties_size(ce));

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

/* {{{ proto Point Point::at(double value)
		     Point Point::at(UI\Size size) */
PHP_METHOD(Point, at)
{
	php_ui_point_t *point;
	zval *location = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &location) != SUCCESS) {
		return;
	}

	if (Z_TYPE_P(location) == IS_OBJECT && instanceof_function(Z_OBJCE_P(location), uiPoint_ce)) {
		RETURN_ZVAL(location, 1, 0);
	}

	if (Z_TYPE_P(location) == IS_OBJECT && instanceof_function(Z_OBJCE_P(location), uiSize_ce)) {
		php_ui_size_t *size = php_ui_size_fetch(location);
		
		object_init_ex(return_value, uiPoint_ce);
		
		point = php_ui_point_fetch(return_value);

		point->x = size->width;
		point->y = size->height;
		return;
	}

	object_init_ex(return_value, uiPoint_ce);

	point = php_ui_point_fetch(return_value);
	point->x = zval_get_double(location);
	point->y = zval_get_double(location);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_point_methods[] = {
	PHP_ME(Point, __construct, php_ui_point_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Point, getX,        php_ui_point_get_point_info, ZEND_ACC_PUBLIC)
	PHP_ME(Point, getY,        php_ui_point_get_point_info, ZEND_ACC_PUBLIC)
	PHP_ME(Point, setX,        php_ui_point_set_point_info, ZEND_ACC_PUBLIC)
	PHP_ME(Point, setY,        php_ui_point_set_point_info, ZEND_ACC_PUBLIC)
	PHP_ME(Point, at,          NULL,                        ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
static int php_ui_point_operate(zend_uchar opcode, zval *result, zval *op1, zval *op2) {
	return php_ui_point_size_operation(opcode, result, op1, op2, uiPoint_ce);
} /* }}} */

/* {{{ */
static zend_object* php_ui_point_clone(zval *o) {
	php_ui_point_t *object = php_ui_point_fetch(o);

	zend_object *cloned = 
		php_ui_point_create(object->std.ce);

	php_ui_point_t *clone = php_ui_point_from(cloned);

	clone->x = object->x;
	clone->y = object->y;

	return cloned;	
} /* }}} */

/* {{{ */
static int php_ui_point_compare(zval *op1, zval *op2) {
	php_ui_point_t *l = php_ui_point_fetch(op1);
	php_ui_point_t *r = php_ui_point_fetch(op2);

	return l->x == r->x && l->y == r->y ? SUCCESS : FAILURE;
} /* }}} */

/* {{{ */
static zval* php_ui_point_read(zval *object, zval *member, int type, void **cache, zval *rv) {
	php_ui_point_t *point = php_ui_point_fetch(object);

	if (Z_TYPE_P(member) != IS_STRING) {
		return &EG(uninitialized_zval);
	}

	if (Z_STRLEN_P(member) != 1) {
		return &EG(uninitialized_zval);
	}

	if (type == BP_VAR_RW || type == BP_VAR_W) {
		zend_throw_exception_ex(NULL, 0, 
			"Failed to fetch reference to %s, not allowed", Z_STRVAL_P(member));
		return &EG(uninitialized_zval);	
	}

	switch (Z_STRVAL_P(member)[0]) {
		case 'x':
		case 'X':
			ZVAL_DOUBLE(rv, point->x);
			return rv;

		case 'y':
		case 'Y':
			ZVAL_DOUBLE(rv, point->y);
			return rv;
	}

	zend_throw_exception_ex(NULL, 0,
		"Failed to fetch %s, does not exist", Z_STRVAL_P(member));

	return &EG(uninitialized_zval);
} /* }}} */

/* {{{ */
zval* php_ui_point_noref(zval *object, zval *member, int type, void **cache) {
	return NULL;
} /* }}} */

/* {{{ */
void php_ui_point_write(zval *object, zval *member, zval *value, void **cache) {
	php_ui_point_t *point = php_ui_point_fetch(object);

	if (Z_TYPE_P(member) != IS_STRING) {
		return;
	}

	if (Z_STRLEN_P(member) != 1) {
		return;
	}

	switch (Z_STRVAL_P(member)[0]) {
		case 'x':
		case 'X':
			point->x = zval_get_double(value);
			return;

		case 'y':
		case 'Y':
			point->y = zval_get_double(value);
			return;
	}

	zend_throw_exception_ex(NULL, 0,
		"Failed to write %s, does not exist", Z_STRVAL_P(member));
} /* }}} */

/* {{{ */
HashTable* php_ui_point_debug(zval *object, int *is_temp) {
	php_ui_point_t *point = php_ui_point_fetch(object);
	zval debug;

	array_init(&debug);

	add_assoc_double(&debug, "x", point->x);
	add_assoc_double(&debug, "y", point->y);

	*is_temp = 1;

	return Z_ARRVAL(debug);
} /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Point) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Point", php_ui_point_methods);

	uiPoint_ce = zend_register_internal_class(&ce);
	uiPoint_ce->create_object = php_ui_point_create;
	uiPoint_ce->ce_flags |= ZEND_ACC_FINAL;

	zend_declare_property_double(uiPoint_ce, ZEND_STRL("x"), 0, ZEND_ACC_PUBLIC);
	zend_declare_property_double(uiPoint_ce, ZEND_STRL("y"), 0, ZEND_ACC_PUBLIC);

	memcpy(&php_ui_point_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_point_handlers.offset = XtOffsetOf(php_ui_point_t, std);
	php_ui_point_handlers.do_operation = php_ui_point_operate;
	php_ui_point_handlers.clone_obj = php_ui_point_clone;
	php_ui_point_handlers.compare_objects = php_ui_point_compare;
	php_ui_point_handlers.read_property = php_ui_point_read;
	php_ui_point_handlers.get_property_ptr_ptr = php_ui_point_noref;
	php_ui_point_handlers.write_property = php_ui_point_write;
	php_ui_point_handlers.get_debug_info = php_ui_point_debug;

	return SUCCESS;
} /* }}} */
#endif
