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
#ifndef HAVE_PHP_UI_COMMON_H
#define HAVE_PHP_UI_COMMON_H

#include <classes/point.h>
#include <classes/size.h>

static inline void php_ui_point_size_add(php_ui_point_t *result, php_ui_point_t *point, double x, double y) {
	result->x = point->x + x;
	result->y = point->y + y;
}

static inline void php_ui_point_size_assign_add(php_ui_point_t*result, double x, double y) {
	result->x += x;
	result->y += y;
}

static inline void php_ui_point_size_mul(php_ui_point_t *result, php_ui_point_t *point, double x, double y) {
	result->x = point->x * x;
	result->y = point->y * y;
}

static inline void php_ui_point_size_assign_mul(php_ui_point_t*result, double x, double y) {
	result->x *= x;
	result->y *= y;
}

static inline void php_ui_point_size_div(php_ui_point_t *result, php_ui_point_t *point, double x, double y) {
	result->x = point->x / x;
	result->y = point->y / y;
}

static inline void php_ui_point_size_assign_div(php_ui_point_t*result, double x, double y) {
	result->x /= x;
	result->y /= y;
}

static inline void php_ui_point_size_sub(php_ui_point_t *result, php_ui_point_t *point, double x, double y) {
	result->x = point->x - x;
	result->y = point->y - y;
}

static inline void php_ui_point_size_assign_sub(php_ui_point_t*result, double x, double y) {
	result->x -= x;
	result->y -= y;
}

/* {{{ */
static inline int php_ui_point_size_operation(zend_uchar opcode, zval *retval, zval *op1, zval *op2, zend_class_entry *target) {
	zval *l, *r;
	double x = 0, y = 0;
	php_ui_point_t *point, *result;
	zend_bool assign = 0;

	if (retval == op1) {
		assign = 1;
	} else {
		object_init_ex(retval, target);
	}

	if (Z_TYPE_P(op1) == IS_OBJECT && Z_TYPE_P(op2) == IS_OBJECT) {
		if ((instanceof_function(Z_OBJCE_P(op1), uiSize_ce) || instanceof_function(Z_OBJCE_P(op1), uiPoint_ce)) &&
			(instanceof_function(Z_OBJCE_P(op2), uiSize_ce) || instanceof_function(Z_OBJCE_P(op2), uiPoint_ce))) {
			l = op1;
			r = op2;
			goto operate;
		}
	}

	if ((Z_TYPE_P(op1) == IS_OBJECT) &&
		(instanceof_function(Z_OBJCE_P(op1), uiPoint_ce) || instanceof_function(Z_OBJCE_P(op1), uiSize_ce))) {
		l = op1;
		r = op2;
	} else if ((Z_TYPE_P(op2) == IS_OBJECT) &&
				(instanceof_function(Z_OBJCE_P(op2), uiPoint_ce) || instanceof_function(Z_OBJCE_P(op2), uiSize_ce))) {
		l = op2;
		r = op1;
	} else {
		return FAILURE;
	}

operate:
	point = php_ui_point_fetch(l);

	if (Z_TYPE_P(r) != IS_OBJECT) {
		x = zval_get_double(r);
		y = zval_get_double(r);
	} else {
		php_ui_point_t *value = php_ui_point_fetch(r);

		x = value->x;
		y = value->y;
	}

	result = php_ui_point_fetch(retval);

	switch (opcode) {
		case ZEND_ADD: assign ? php_ui_point_size_assign_add(result, x, y) : php_ui_point_size_add(result, point, x, y); break;
		case ZEND_MUL: assign ? php_ui_point_size_assign_mul(result, x, y) : php_ui_point_size_mul(result, point, x, y); break;
		case ZEND_DIV: assign ? php_ui_point_size_assign_div(result, x, y) : php_ui_point_size_div(result, point, x, y); break;
		case ZEND_SUB: assign ? php_ui_point_size_assign_sub(result, x, y) : php_ui_point_size_sub(result, point, x, y); break;
	}

	return SUCCESS;
} /* }}} */

#endif
