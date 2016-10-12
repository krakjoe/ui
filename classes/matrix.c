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

#include <classes/control.h>
#include <classes/matrix.h>
#include <classes/context.h>

zend_object_handlers php_ui_matrix_handlers;

zend_object* php_ui_matrix_create(zend_class_entry *ce) {
	php_ui_matrix_t *matrix = 
		(php_ui_matrix_t*) ecalloc(1, sizeof(php_ui_matrix_t));

	zend_object_std_init(&matrix->std, ce);

	object_properties_init(&matrix->std, ce);

	matrix->std.handlers = &php_ui_matrix_handlers;

	return &matrix->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_construct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto DrawMatrix DrawMatrix::__construct() */
PHP_METHOD(DrawMatrix, __construct) 
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiDrawMatrixSetIdentity(&matrix->m);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_translate_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawMatrix::translate(double x, double y) */
PHP_METHOD(DrawMatrix, translate)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	double x = 0, y = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) != SUCCESS) {
		return;
	}

	uiDrawMatrixTranslate(&matrix->m, x, y);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_scale_info, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, xCenter, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, yCenter, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawMatrix::scale(double xCenter, double yCenter, double x, double y) */
PHP_METHOD(DrawMatrix, scale)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	double xCenter = 0, yCenter = 0, x = 0, y = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddd", &xCenter, &yCenter, &x, &y) != SUCCESS) {
		return;
	}

	uiDrawMatrixScale(&matrix->m, xCenter, yCenter, x, y);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_rotate_info, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, amount, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawMatrix::rotate(double xCenter, double yCenter, double x, double y) */
PHP_METHOD(DrawMatrix, rotate)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	double x = 0, y = 0, amount = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ddd", &x, &y, &amount) != SUCCESS) {
		return;
	}

	uiDrawMatrixRotate(&matrix->m, x, y, amount);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_matrix_skew_info, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, xAmount, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, yAmount, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawMatrix::skew(double x, double y, double xAmount, double yAmount) */
PHP_METHOD(DrawMatrix, skew)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	double xAmount = 0, yAmount = 0, x = 0, y = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddd", &x, &y, &xAmount, &yAmount) != SUCCESS) {
		return;
	}

	uiDrawMatrixSkew(&matrix->m, x, y, xAmount, yAmount);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_matrix_multiply_info, 0, 0, IS_OBJECT, "UI\\DrawMatrix", 1)
ZEND_END_ARG_INFO()

/* {{{ proto DrawMatrix DrawMatrix::multiply() */
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

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_matrix_is_invertible_info, 0, 0, _IS_BOOL, NULL, 1)
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
	PHP_ME(DrawMatrix, __construct,     php_ui_matrix_construct_info,       ZEND_ACC_PUBLIC)
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

	INIT_NS_CLASS_ENTRY(ce, "UI", "DrawMatrix", php_ui_matrix_methods);

	uiDrawMatrix_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiDrawMatrix_ce->create_object = php_ui_matrix_create;

	memcpy(&php_ui_matrix_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_matrix_handlers.offset = XtOffsetOf(php_ui_matrix_t, std);

	return SUCCESS;
} /* }}} */
#endif
