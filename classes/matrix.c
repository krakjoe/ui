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
#ifndef HAVE_PHP_UI_MATRIX
#define HAVE_PHP_UI_MATRIX
#include <ui.h>

#include "php.h"

#include <classes/point.h>
#include <classes/matrix.h>

zend_object_handlers php_ui_matrix_handlers;

zend_class_entry *uiDrawMatrix_ce;

zend_object* php_ui_matrix_create(zend_class_entry *ce) {
	php_ui_matrix_t *matrix = 
		(php_ui_matrix_t*) ecalloc(1, sizeof(php_ui_matrix_t) + zend_object_properties_size(ce));

	zend_object_std_init(&matrix->std, ce);

	object_properties_init(&matrix->std, ce);

	matrix->std.handlers = &php_ui_matrix_handlers;

	uiDrawMatrixSetIdentity(&matrix->m);

	return &matrix->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_translate_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawMatrix::translate(Point point) */
PHP_METHOD(DrawMatrix, translate)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &point, uiPoint_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	uiDrawMatrixTranslate(&matrix->m, p->x, p->y);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_scale_info, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, center, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawMatrix::scale(Point center, Point point) */
PHP_METHOD(DrawMatrix, scale)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *center = NULL, *point = NULL;
	php_ui_point_t *p, *c;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &center, uiPoint_ce, &point, uiPoint_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);
	c = php_ui_point_fetch(center);

	uiDrawMatrixScale(&matrix->m, c->x, c->y, p->x, p->y);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_rotate_info, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
	ZEND_ARG_TYPE_INFO(0, amount, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawMatrix::rotate(Point point, double amount) */
PHP_METHOD(DrawMatrix, rotate)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p;
	double amount = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Od", &point, uiPoint_ce, &amount) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	uiDrawMatrixRotate(&matrix->m, p->x, p->y, amount);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_skew_info, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, amount, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawMatrix::skew(Point point, Point amount) */
PHP_METHOD(DrawMatrix, skew)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *point = NULL, *amount = NULL;
	php_ui_point_t *p, *a;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &point, uiPoint_ce, &amount, uiPoint_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);
	a = php_ui_point_fetch(amount);

	uiDrawMatrixSkew(&matrix->m, p->x, p->y, a->x, a->y);
} /* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(php_ui_matrix_multiply_info, 0, 1, "UI\\DrawMatrix", 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_matrix_multiply_info, 0, 1, IS_OBJECT, "UI\\DrawMatrix", 0)
#endif
	ZEND_ARG_OBJ_INFO(0, matrix, UI\\Draw\\Matrix, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Matrix UI\Draw\Matrix::multiply(UI\Draw\Matrix multiply) */
PHP_METHOD(DrawMatrix, multiply)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	php_ui_matrix_t *dest;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiDrawMatrix_ce);

	dest = php_ui_matrix_fetch(return_value);

	uiDrawMatrixMultiply(&dest->m, &matrix->m);
} /* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_matrix_is_invertible_info, 0, 0, _IS_BOOL, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_matrix_is_invertible_info, 0, 0, _IS_BOOL, NULL, 1)
#endif
ZEND_END_ARG_INFO()

/* {{{ proto bool DrawMatrix::isInvertible(void) */
PHP_METHOD(DrawMatrix, isInvertible)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiDrawMatrixInvertible(&matrix->m)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_invert_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawMatrix::invert(void) */
PHP_METHOD(DrawMatrix, invert)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiDrawMatrixInvert(&matrix->m);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_matrix_methods[] = {
	PHP_ME(DrawMatrix, translate,       php_ui_matrix_translate_info,       ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, scale,           php_ui_matrix_scale_info,           ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, rotate,          php_ui_matrix_rotate_info,          ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, skew,            php_ui_matrix_skew_info,            ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, multiply,        php_ui_matrix_multiply_info,        ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, isInvertible,    php_ui_matrix_is_invertible_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, invert,          php_ui_matrix_invert_info,          ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawMatrix) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw", "Matrix", php_ui_matrix_methods);

	uiDrawMatrix_ce = zend_register_internal_class(&ce);
	uiDrawMatrix_ce->create_object = php_ui_matrix_create;

	memcpy(&php_ui_matrix_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_matrix_handlers.offset = XtOffsetOf(php_ui_matrix_t, std);

	return SUCCESS;
} /* }}} */
#endif
