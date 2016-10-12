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
#ifndef HAVE_PHP_UI_PATH
#define HAVE_PHP_UI_PATH
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/path.h>

zend_object_handlers php_ui_path_handlers;

zend_object* php_ui_path_create(zend_class_entry *ce) {
	php_ui_path_t *path = 
		(php_ui_path_t*) ecalloc(1, sizeof(php_ui_path_t));

	zend_object_std_init(&path->std, ce);

	object_properties_init(&path->std, ce);

	path->std.handlers = &php_ui_path_handlers;

	return &path->std;
}

void php_ui_path_free(zend_object *o) {
	php_ui_path_t *path = php_ui_path_from(o);

	uiDrawPathFree(path->p);

	zend_object_std_dtor(o);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_path_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto DrawPath DrawPath::__construct(int mode = DRAWPATH::WINDING) */
PHP_METHOD(DrawPath, __construct) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	zend_long mode = PHP_UI_PATH_WINDING;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &mode) != SUCCESS) {
		return;
	}

	path->p = uiDrawNewPath(mode);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_path_new_figure_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawPath::newFigure(double x, double y) */
PHP_METHOD(DrawPath, newFigure) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	double x = 0, y = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) != SUCCESS) {
		return;
	}

	uiDrawPathNewFigure(path->p, x, y);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_path_draw_to_info, 0, 0, 6)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, radius, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, angle, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, sweep, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, negative, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawPath::newFigureWithArc(double x, double y, double radius, double angle, double sweep, double negative) */
PHP_METHOD(DrawPath, newFigureWithArc) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	double x = 0, y = 0, radius = 0, angle = 0, sweep = 0, negative = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddddd", &x, &y, &radius, &angle, &sweep, &negative) != SUCCESS) {
		return;
	}

	uiDrawPathNewFigureWithArc(path->p, x, y, radius, angle, sweep, negative);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_path_line_to_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawPath::lineTo(double x, double y) */
PHP_METHOD(DrawPath, lineTo) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	double x = 0, y = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) != SUCCESS) {
		return;
	}

	uiDrawPathLineTo(path->p, x, y);
} /* }}} */

/* {{{ proto void DrawPath::arcTo(double x, double y, double radius, double angle, double sweep, double negative) */
PHP_METHOD(DrawPath, arcTo) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	double x = 0, y = 0, radius = 0, angle = 0, sweep = 0, negative = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddddd", &x, &y, &radius, &angle, &sweep, &negative) != SUCCESS) {
		return;
	}

	uiDrawPathArcTo(path->p, x, y, radius, angle, sweep, negative);
} /* }}} */

/* {{{ proto void DrawPath::bezierTo(double x, double y, double radius, double angle, double sweep, double negative) */
PHP_METHOD(DrawPath, bezierTo)
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	double x = 0, y = 0, radius = 0, angle = 0, sweep = 0, negative = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddddd", &x, &y, &radius, &angle, &sweep, &negative) != SUCCESS) {
		return;
	}

	uiDrawPathBezierTo(path->p, x, y, radius, angle, sweep, negative);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_path_close_figure_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawPath::closeFigure(void) */
PHP_METHOD(DrawPath, closeFigure)
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiDrawPathCloseFigure(path->p);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_path_add_rectangle_info, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawPath::addRectangle(double x, double y, double width, double height) */
PHP_METHOD(DrawPath, addRectangle)
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	double x = 0, y = 0, width = 0, height = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddd", &x, &y, &width, &height) != SUCCESS) {
		return;
	}

	uiDrawPathAddRectangle(path->p, x, y, width, height);	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_path_end_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawPath::end(void) */
PHP_METHOD(DrawPath, end)
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiDrawPathEnd(path->p);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_path_methods[] = {
	PHP_ME(DrawPath, __construct, php_ui_path_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, newFigure,   php_ui_path_new_figure_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, newFigureWithArc,   php_ui_path_draw_to_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, lineTo,   php_ui_path_draw_to_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, arcTo,   php_ui_path_draw_to_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, bezierTo, php_ui_path_draw_to_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, closeFigure, php_ui_path_close_figure_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, addRectangle, php_ui_path_add_rectangle_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, end, php_ui_path_end_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawPath) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "DrawPath", php_ui_path_methods);

	uiDrawPath_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiDrawPath_ce->create_object = php_ui_path_create;

	memcpy(&php_ui_path_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_path_handlers.offset = XtOffsetOf(php_ui_path_t, std);
	php_ui_path_handlers.free_obj = php_ui_path_free;

	return SUCCESS;
} /* }}} */
#endif
