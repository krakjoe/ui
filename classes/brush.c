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
#ifndef HAVE_PHP_UI_BRUSH
#define HAVE_PHP_UI_BRUSH
#include <ui.h>

#include "php.h"

#include <classes/exceptions.h>
#include <classes/brush.h>
#include <classes/point.h>
#include <classes/color.h>

zend_object_handlers php_ui_brush_handlers;

zend_class_entry *uiDrawBrush_ce;
zend_class_entry *uiDrawBrushGradient_ce;
zend_class_entry *uiDrawBrushLinearGradient_ce;
zend_class_entry *uiDrawBrushRadialGradient_ce;

zend_object* php_ui_brush_create(zend_class_entry *ce) {
	php_ui_brush_t *brush = 
		(php_ui_brush_t*) ecalloc(1, sizeof(php_ui_brush_t) + zend_object_properties_size(ce));

	zend_object_std_init(&brush->std, ce);

	object_properties_init(&brush->std, ce);

	brush->std.handlers = &php_ui_brush_handlers;

	return &brush->std;
}

void php_ui_brush_free(zend_object *o) {
	php_ui_brush_t *brush = php_ui_brush_from(o);

	if (brush->b.NumStops) {
		efree(brush->b.Stops);
	}

	zend_object_std_dtor(o);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_brush_construct_info, 0, 0, 1)
	ZEND_ARG_INFO(0, color)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Brush::__construct(UI\Draw\Color color)
       proto UI\Draw\Brush::__construct(int rgb) */
PHP_METHOD(DrawBrush, __construct) 
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zval *color = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &color) != SUCCESS) {
		return;
	}

	brush->b.Type = uiDrawBrushTypeSolid;

	if (!php_ui_color_set(color, 
			&brush->b.R, &brush->b.G, &brush->b.B, &brush->b.A)) {
		php_ui_exception("could not set the color of the brush");
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_brush_set_color_info, 0, 0, 1)
	ZEND_ARG_INFO(0, color)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawBrush::setColor(UI\Draw\Color color) */
PHP_METHOD(DrawBrush, setColor)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zval *color = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &color) != SUCCESS) {
		return;
	}

	if (brush->b.Type != uiDrawBrushTypeSolid) {
		php_ui_exception("this brush does not have a color");
		return;
	}

	if (!php_ui_color_set(color, 
			&brush->b.R, &brush->b.G, &brush->b.B, &brush->b.A)) {
		php_ui_exception("could not set the color of the brush");
	}
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_brush_get_color_info, 0, 0, IS_OBJECT, "UI\\Draw\\Color", 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Color Brush::getColor(void) */
PHP_METHOD(DrawBrush, getColor)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	php_ui_color_t *color;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (brush->b.Type != uiDrawBrushTypeSolid) {
		php_ui_exception("this brush does not have a color");
		return;
	}

	object_init_ex(return_value, uiDrawColor_ce);
	
	color = php_ui_color_fetch(return_value);

	color->r = brush->b.R;
	color->g = brush->b.G;
	color->b = brush->b.B;
	color->a = brush->b.A;	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_brush_methods[] = {
	PHP_ME(DrawBrush, __construct, php_ui_brush_construct_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, setColor,    php_ui_brush_set_color_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, getColor,    php_ui_brush_get_color_info,   ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_gradient_add_stop_info, 0, 2, IS_LONG, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, position, IS_DOUBLE, 0)
	ZEND_ARG_INFO(0, color)
ZEND_END_ARG_INFO()

/* {{{ proto int UI\Draw\Brush\Gradient::addStop(double position, UI\Draw\Color color) */
PHP_METHOD(DrawBrushGradient, addStop)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	double position = 0;
	zval *color = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dz", &position, &color) != SUCCESS) {
		return;
	}

	if (!brush->b.NumStops) {
		brush->b.Stops = (uiDrawBrushGradientStop*) emalloc(sizeof(uiDrawBrushGradientStop));
	} else {
		brush->b.Stops = (uiDrawBrushGradientStop*) erealloc(
			brush->b.Stops, sizeof(uiDrawBrushGradientStop) * (brush->b.NumStops + 1));
	}

	if (!php_ui_color_set(color, 
		&brush->b.Stops[brush->b.NumStops].R, 
		&brush->b.Stops[brush->b.NumStops].G, 
		&brush->b.Stops[brush->b.NumStops].B, 
		&brush->b.Stops[brush->b.NumStops].A)) {
		php_ui_exception("could not set the color of the stop");
		return;
	}

	brush->b.Stops[brush->b.NumStops].Pos = position;
	brush->b.NumStops++;

	RETURN_LONG(brush->b.NumStops);	
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_gradient_del_stop_info, 0, 1, IS_LONG, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int UI\Draw\Brush\Gradient::delStop(int index) */
PHP_METHOD(DrawBrushGradient, delStop)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zend_long index = 0, stop  = 0;
	uiDrawBrushGradientStop *stops;
	uiDrawBrushGradientStop *next;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &index) != SUCCESS) {
		return;
	}

	if (!brush->b.NumStops || index < 0) {
		php_ui_exception_ex(InvalidArgumentException,
			"the stop %ld is not valid", index);
		return;
	}

	if (brush->b.NumStops == 1) {
		efree(brush->b.Stops);

		brush->b.NumStops = 0;
		brush->b.Stops = NULL;
		RETURN_LONG(0);
	}

	stops = next = (uiDrawBrushGradientStop*) ecalloc(brush->b.NumStops - 1, sizeof(uiDrawBrushGradientStop));

	for (stop = 0; stop < brush->b.NumStops; stop++) {
		if (stop != index) {
			memcpy(next, &brush->b.Stops[stop], sizeof(uiDrawBrushGradientStop));
			next++;
		}
	}

	brush->b.Stops = stops;
	brush->b.NumStops--;

	RETURN_LONG(brush->b.NumStops);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_gradient_set_stop_info, 0, 3, _IS_BOOL, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, position, IS_DOUBLE, 0)
	ZEND_ARG_OBJ_INFO(0, color, UI\\Draw\\Color, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool UI\Draw\Draw\Brush\Gradient::setStop(int index, double position, UI\Draw\Color color) */
PHP_METHOD(DrawBrushGradient, setStop)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zend_long index = 0;
	double position = 0;
	zval *color = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ldz", &index, &position, &color) != SUCCESS) {
		return;
	}

	if (index < 0 || index >= brush->b.NumStops) {
		php_ui_exception_ex(InvalidArgumentException,
			"the stop %ld is not valid", index);
		return;
	}

	if (php_ui_color_set(color, 
		&brush->b.Stops[index].R, 
		&brush->b.Stops[index].G, 
		&brush->b.Stops[index].B, 
		&brush->b.Stops[index].A)) {
		php_ui_exception(
			"could not set the color of stop %ld", index);
		return;
	}

	brush->b.Stops[index].Pos = position;
	RETURN_TRUE;	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_gradient_methods[] = {
	PHP_ME(DrawBrushGradient, addStop,       php_ui_gradient_add_stop_info,       ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrushGradient, delStop,       php_ui_gradient_del_stop_info,       ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrushGradient, setStop,       php_ui_gradient_set_stop_info,       ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_linear_gradient_construct_info, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, start, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, end, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto LinearGradient UI\Draw\Brush\LinearGradient::__construct(UI\Point start, UI\Point end) */
PHP_METHOD(DrawBrushLinearGradient, __construct) 
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zval *start = NULL, *end = NULL;
	php_ui_point_t *s, *e;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &start, uiPoint_ce, &end, uiPoint_ce) != SUCCESS) {
		return;
	}

	brush->b.Type = uiDrawBrushTypeLinearGradient;

	s = php_ui_point_fetch(start);
	e = php_ui_point_fetch(end);

	brush->b.X0 = s->x;
	brush->b.Y0 = s->y;

	brush->b.X1 = e->x;
	brush->b.Y1 = e->y;
} /* }}} */

const zend_function_entry php_ui_linear_gradient_methods[] = {
	PHP_ME(DrawBrushLinearGradient, __construct,   php_ui_linear_gradient_construct_info,      ZEND_ACC_PUBLIC)
	PHP_FE_END
};

ZEND_BEGIN_ARG_INFO_EX(php_ui_radial_gradient_construct_info, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0, start, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, outer, UI\\Point, 0)
	ZEND_ARG_TYPE_INFO(0, radius, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto RadialGradient UI\Draw\Brush\RadialGradient::__construct(UI\Point start, UI\Point outer, double radius) */
PHP_METHOD(DrawBrushRadialGradient, __construct) 
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zval *start = NULL, *outer = NULL;
	double radius = 0;
	php_ui_point_t *s, *o;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OOd", &start, uiPoint_ce, &outer, uiPoint_ce, &radius) != SUCCESS) {
		return;
	}

	brush->b.Type = uiDrawBrushTypeRadialGradient;

	s = php_ui_point_fetch(start);
	o = php_ui_point_fetch(outer);

	brush->b.X0 = s->x;
	brush->b.Y0 = s->y;

	brush->b.X1 = o->x;
	brush->b.Y1 = o->y;

	brush->b.OuterRadius = radius;
} /* }}} */

const zend_function_entry php_ui_radial_gradient_methods[] = {
	PHP_ME(DrawBrushRadialGradient, __construct,   php_ui_radial_gradient_construct_info,      ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawBrush) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw", "Brush", php_ui_brush_methods);

	uiDrawBrush_ce = zend_register_internal_class(&ce);
	uiDrawBrush_ce->create_object = php_ui_brush_create;

	memcpy(&php_ui_brush_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_brush_handlers.offset = XtOffsetOf(php_ui_brush_t, std);
	php_ui_brush_handlers.free_obj = php_ui_brush_free;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Brush", "Gradient", php_ui_gradient_methods);

	uiDrawBrushGradient_ce = zend_register_internal_class_ex(&ce, uiDrawBrush_ce);
	uiDrawBrushGradient_ce->ce_flags |= ZEND_ACC_ABSTRACT;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Brush", "LinearGradient", php_ui_linear_gradient_methods);

	uiDrawBrushLinearGradient_ce = zend_register_internal_class_ex(&ce, uiDrawBrushGradient_ce);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Brush", "RadialGradient", php_ui_radial_gradient_methods);

	uiDrawBrushRadialGradient_ce = zend_register_internal_class_ex(&ce, uiDrawBrushGradient_ce);

	return SUCCESS;
} /* }}} */
#endif
