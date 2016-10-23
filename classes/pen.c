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
#ifndef HAVE_PHP_UI_PEN
#define HAVE_PHP_UI_PEN
#include <ui.h>

#include "php.h"

#include <classes/point.h>
#include <classes/size.h>
#include <classes/path.h>
#include <classes/brush.h>
#include <classes/matrix.h>
#include <classes/stroke.h>
#include <classes/pen.h>
#include <classes/layout.h>

zend_object_handlers php_ui_pen_handlers;

zend_class_entry *uiDrawPen_ce;

zend_object* php_ui_pen_create(zend_class_entry *ce) {
	php_ui_pen_t *pen = 
		(php_ui_pen_t*) ecalloc(1, sizeof(php_ui_pen_t) + zend_object_properties_size(ce));

	zend_object_std_init(&pen->std, ce);

	object_properties_init(&pen->std, ce);

	pen->std.handlers = &php_ui_pen_handlers;

	return &pen->std;
}

zval* php_ui_pen_construct(zval *object, uiDrawContext *c) {
	php_ui_pen_t *pen;

	object_init_ex(object, uiDrawPen_ce);
	
	pen = php_ui_pen_fetch(object);
	pen->c = c;

	return object;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_pen_fill_info, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, path, UI\\Draw\\Path, 0)
	ZEND_ARG_OBJ_INFO(0, brush, UI\\Draw\\Brush, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Pen::fill(UI\Draw\Path path, UI\Draw\Brush brush) */
PHP_METHOD(DrawPen, fill) 
{
	php_ui_pen_t *c = php_ui_pen_fetch(getThis());
	zval *path = NULL, *brush = NULL;
	php_ui_path_t *p;
	php_ui_brush_t *b;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &path, uiDrawPath_ce, &brush, uiDrawBrush_ce) != SUCCESS) {
		return;
	}

	p = php_ui_path_fetch(path);
	b = php_ui_brush_fetch(brush);

	uiDrawFill(c->c, p->p, &b->b);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_pen_stroke_info, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0, path, UI\\Draw\\Path, 0)
	ZEND_ARG_OBJ_INFO(0, brush, UI\\Draw\\Brush, 0)
	ZEND_ARG_OBJ_INFO(0, stroke, UI\\Draw\\Stroke, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Pen::stroke(UI\Draw\Path path, UI\Draw\Brush brush, UI\Draw\Stroke stroke) */
PHP_METHOD(DrawPen, stroke)
{
	php_ui_pen_t *c = php_ui_pen_fetch(getThis());
	zval *path = NULL, *brush = NULL, *stroke = NULL;
	php_ui_path_t *p;
	php_ui_brush_t *b;
	php_ui_stroke_t *s;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OOO", &path, uiDrawPath_ce, &brush, uiDrawBrush_ce, &stroke, uiDrawStroke_ce) != SUCCESS) {
		return;
	}

	p = php_ui_path_fetch(path);
	b = php_ui_brush_fetch(brush);
	s = php_ui_stroke_fetch(stroke);

	uiDrawStroke(c->c, p->p, &b->b, &s->s);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_pen_transform_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, matrix, UI\\Draw\\Matrix, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Pen::transform(UI\Draw\Matrix matrix) */
PHP_METHOD(DrawPen, transform)
{
	php_ui_pen_t *c = php_ui_pen_fetch(getThis());
	zval *matrix = NULL;
	php_ui_matrix_t *m;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O",&matrix, uiDrawMatrix_ce) != SUCCESS) {
		return;
	}

	m = php_ui_matrix_fetch(matrix);

	uiDrawTransform(c->c, &m->m);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_pen_clip_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, path, UI\\Draw\\Path, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Pen::clip(UI\Draw\Path path) */
PHP_METHOD(DrawPen, clip)
{
	php_ui_pen_t *c = php_ui_pen_fetch(getThis());
	zval *path = NULL;
	php_ui_path_t *p;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &path, uiDrawPath_ce) != SUCCESS) {
		return;
	}

	p = php_ui_path_fetch(path);

	uiDrawClip(c->c, p->p);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_pen_save_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Pen::save() */
PHP_METHOD(DrawPen, save)
{
	php_ui_pen_t *c = php_ui_pen_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiDrawSave(c->c);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_pen_restore_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Pen::restore() */
PHP_METHOD(DrawPen, restore)
{
	php_ui_pen_t *c = php_ui_pen_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiDrawRestore(c->c);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_pen_write_info, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, layout, UI\\Draw\\Text\\Layout, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\Draw\Pen::write(UI\Point point, UI\Draw\Text\Layout layout) */
PHP_METHOD(DrawPen, write)
{
	php_ui_pen_t *pen = php_ui_pen_fetch(getThis());
	zval *_point = NULL, *_layout = NULL;
	php_ui_point_t *point;
	php_ui_layout_t *layout;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &_point, uiPoint_ce, &_layout, uiDrawTextLayout_ce) != SUCCESS) {
		return;
	}
		
	layout = php_ui_layout_fetch(_layout);
	point = php_ui_point_fetch(_point);

	uiDrawText(pen->c, point->x, point->y, layout->l);	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_pen_methods[] = {
	PHP_ME(DrawPen, fill, php_ui_pen_fill_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPen, stroke, php_ui_pen_stroke_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPen, transform, php_ui_pen_transform_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPen, clip, php_ui_pen_clip_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPen, save, php_ui_pen_save_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPen, restore, php_ui_pen_restore_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPen, write, php_ui_pen_write_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawPen) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw", "Pen", php_ui_pen_methods);

	uiDrawPen_ce = zend_register_internal_class(&ce);
	uiDrawPen_ce->create_object = php_ui_pen_create;
	uiDrawPen_ce->ce_flags |= ZEND_ACC_FINAL;

	memcpy(&php_ui_pen_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_pen_handlers.offset = XtOffsetOf(php_ui_pen_t, std);

	return SUCCESS;
} /* }}} */
#endif
