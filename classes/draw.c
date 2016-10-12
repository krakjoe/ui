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
#ifndef HAVE_PHP_UI_DRAW
#define HAVE_PHP_UI_DRAW
#include <ui.h>

#include "php.h"

#include <classes/draw.h>
#include <classes/context.h>
#include <classes/path.h>
#include <classes/brush.h>
#include <classes/stroke.h>
#include <classes/matrix.h>

ZEND_BEGIN_ARG_INFO_EX(php_ui_draw_fill_info, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0, context, UI\\DrawContext, 0)
	ZEND_ARG_OBJ_INFO(0, path, UI\\DrawPath, 0)
	ZEND_ARG_OBJ_INFO(0, brush, UI\\DrawBrush, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Draw::fill(UI\DrawContext context, UI\DrawPath path, UI\DrawBrush brush) */
PHP_METHOD(Draw, fill) 
{
	zval *context = NULL, *path = NULL, *brush = NULL;
	php_ui_context_t *c;
	php_ui_path_t *p;
	php_ui_brush_t *b;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OOO", &context, uiDrawContext_ce, &path, uiDrawPath_ce, &brush, uiDrawBrush_ce) != SUCCESS) {
		return;
	}

	c = php_ui_context_fetch(context);
	p = php_ui_path_fetch(path);
	b = php_ui_brush_fetch(brush);

	uiDrawFill(c->c, p->p, &b->b);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_draw_stroke_info, 0, 0, 4)
	ZEND_ARG_OBJ_INFO(0, context, UI\\DrawContext, 0)
	ZEND_ARG_OBJ_INFO(0, path, UI\\DrawPath, 0)
	ZEND_ARG_OBJ_INFO(0, brush, UI\\DrawBrush, 0)
	ZEND_ARG_OBJ_INFO(0, stroke, UI\\DrawStroke, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Draw::stroke(UI\DrawContext context, UI\DrawPath path, UI\DrawBrush brush, UI\DrawStroke stroke) */
PHP_METHOD(Draw, stroke)
{
	zval *context = NULL, *path = NULL, *brush = NULL, *stroke = NULL;
	php_ui_context_t *c;
	php_ui_path_t *p;
	php_ui_brush_t *b;
	php_ui_stroke_t *s;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OOOO", &context, uiDrawContext_ce, &path, uiDrawPath_ce, &brush, uiDrawBrush_ce, &stroke, uiDrawStroke_ce) != SUCCESS) {
		return;
	}

	c = php_ui_context_fetch(context);
	p = php_ui_path_fetch(path);
	b = php_ui_brush_fetch(brush);
	s = php_ui_stroke_fetch(stroke);

	uiDrawStroke(c->c, p->p, &b->b, &s->s);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_draw_transform_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, context, UI\\DrawContext, 0)
	ZEND_ARG_OBJ_INFO(0, matrix, UI\\DrawMatrix, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Draw::transform(UI\DrawContext context, UI\DrawMatrix matrix) */
PHP_METHOD(Draw, transform)
{
	zval *context = NULL, *matrix = NULL;
	php_ui_context_t *c;
	php_ui_matrix_t *m;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &context, uiDrawContext_ce, &matrix, uiDrawMatrix_ce) != SUCCESS) {
		return;
	}

	c = php_ui_context_fetch(context);	
	m = php_ui_matrix_fetch(matrix);

	uiDrawTransform(c->c, &m->m);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_draw_methods[] = {
	PHP_ME(Draw, fill,     php_ui_draw_fill_info,  ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Draw, stroke,   php_ui_draw_stroke_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Draw, transform, php_ui_draw_transform_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Draw) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Draw", php_ui_draw_methods);

	uiDraw_ce = zend_register_internal_class(&ce);

	return SUCCESS;
} /* }}} */
#endif
