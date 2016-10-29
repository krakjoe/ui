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

#include <classes/brush.h>
#include <classes/color.h>

zend_object_handlers php_ui_brush_handlers;

zend_class_entry *uiDrawBrush_ce;

static inline uiDrawBrushType php_ui_brush_type(zend_long type) {
	switch (type) {
		case PHP_UI_BRUSH_SOLID: return uiDrawBrushTypeSolid;
		case PHP_UI_BRUSH_LINEAR_GRADIENT: return uiDrawBrushTypeLinearGradient;
		case PHP_UI_BRUSH_RADIAL_GRADIENT: return uiDrawBrushTypeRadialGradient;
		case PHP_UI_BRUSH_IMAGE: return uiDrawBrushTypeImage;
	}
	return uiDrawBrushTypeSolid;
}

static inline void php_ui_brush_color(php_ui_brush_t *brush, php_ui_color_t *color) {
	brush->b.R = color->r;
	brush->b.G = color->g;
	brush->b.B = color->b;
	brush->b.A = color->a;
}

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
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, color, UI\\Draw\\Color, 1)
	ZEND_ARG_TYPE_INFO(0, X0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, Y0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, X1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, Y1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, radius, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto DrawBrush DrawBrush::__construct(int type [, UI\Draw\Color color, double X0, double, Y0, double X1, double Y1, double radius]) */
PHP_METHOD(DrawBrush, __construct) 
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zend_long type = PHP_UI_BRUSH_SOLID;
	zval *color = NULL;
	double X0 = 0, Y0 = 0, X1 = 0, Y1 = 0, radius = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l|O!ddddd", &type, &color, uiDrawColor_ce, &X0, &Y0, &X1, &Y1, &radius) != SUCCESS) {
		return;
	}

	brush->b.Type = php_ui_brush_type(type);

	if (color) {
		php_ui_brush_color(brush, php_ui_color_fetch(color));
	}

	brush->b.X0 = X0;
	brush->b.Y0 = Y0;
	brush->b.X1 = X1;
	brush->b.Y1 = Y1;
	brush->b.OuterRadius = radius;
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_brush_set_color_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, color, UI\\Draw\\Color, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawBrush::setColor(UI\Draw\Color color) */
PHP_METHOD(DrawBrush, setColor)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zval *color = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &color, uiDrawColor_ce) != SUCCESS) {
		return;
	}

	php_ui_brush_color(brush, php_ui_color_fetch(color));	
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_brush_get_type_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int DrawBrush::getType(void) */
PHP_METHOD(DrawBrush, getType) 
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		RETURN_LONG(-1);
	}

	switch (brush->b.Type) {
		case uiDrawBrushTypeSolid:
			RETURN_LONG(PHP_UI_BRUSH_SOLID);
		break;

		case uiDrawBrushTypeLinearGradient:
			RETURN_LONG(PHP_UI_BRUSH_LINEAR_GRADIENT);
		break;

		case uiDrawBrushTypeRadialGradient:
			RETURN_LONG(PHP_UI_BRUSH_RADIAL_GRADIENT);
		break;

		case uiDrawBrushTypeImage:
			RETURN_LONG(PHP_UI_BRUSH_IMAGE);
		break;
	}
	
	RETURN_LONG(PHP_UI_BRUSH_SOLID);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_brush_set_type_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawBrush::setType(int type) */
PHP_METHOD(DrawBrush, setType)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zend_long type = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &type) != SUCCESS) {
		return;
	}

	brush->b.Type = php_ui_brush_type(type);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_brush_get_color_info, 0, 0, IS_OBJECT, "UI\\Draw\\Color", 0)
ZEND_END_ARG_INFO()

/* {{{ proto Color Brush::getColor(void) */
PHP_METHOD(DrawBrush, getColor)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	php_ui_color_t *color;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiDrawColor_ce);
	
	color = php_ui_color_fetch(return_value);

	color->r = brush->b.R;
	color->g = brush->b.G;
	color->b = brush->b.B;
	color->a = brush->b.A;	
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_brush_add_stop_info, 0, 2, IS_LONG, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, position, IS_DOUBLE, 0)
	ZEND_ARG_OBJ_INFO(0, color, UI\\Draw\\Color, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int UI\Draw\Brush::addStop(double position, UI\Draw\Color color) */
PHP_METHOD(DrawBrush, addStop)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	double position = 0;
	zval *color = NULL;
	php_ui_color_t *c;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dO", &position, &color, uiDrawColor_ce) != SUCCESS) {
		return;
	}

	c = php_ui_color_fetch(color);

	if (!brush->b.NumStops) {
		brush->b.Stops = (uiDrawBrushGradientStop*) emalloc(sizeof(uiDrawBrushGradientStop));
	} else {
		brush->b.Stops = (uiDrawBrushGradientStop*) erealloc(
			brush->b.Stops, sizeof(uiDrawBrushGradientStop) * (brush->b.NumStops + 1));
	}

	brush->b.Stops[brush->b.NumStops].Pos = position;
	brush->b.Stops[brush->b.NumStops].R   = c->r;
	brush->b.Stops[brush->b.NumStops].G   = c->g;
	brush->b.Stops[brush->b.NumStops].B   = c->b;
	brush->b.Stops[brush->b.NumStops].A   = c->a;
	brush->b.NumStops++;

	RETURN_LONG(brush->b.NumStops);	
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_brush_del_stop_info, 0, 1, IS_LONG, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int UI\Draw\Brush::delStop(int index) */
PHP_METHOD(DrawBrush, delStop)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zend_long index = 0, stop  = 0;
	uiDrawBrushGradientStop *stops;
	uiDrawBrushGradientStop *next;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &index) != SUCCESS) {
		return;
	}

	if (!brush->b.NumStops || index < 0) {
		RETURN_LONG(-1);
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

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_brush_set_stop_info, 0, 3, _IS_BOOL, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, position, IS_DOUBLE, 0)
	ZEND_ARG_OBJ_INFO(0, color, UI\\Draw\\Color, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool UI\Draw\Brush::setStop(int index, double position, UI\Draw\Color color) */
PHP_METHOD(DrawBrush, setStop)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zend_long index = 0;
	double position = 0;
	zval *color = NULL;
	php_ui_color_t *c;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ldO", &index, &position, &color, uiDrawColor_ce) != SUCCESS) {
		return;
	}

	if (index < 0 || index >= brush->b.NumStops) {
		RETURN_FALSE;
	}

	c = php_ui_color_fetch(color);

	brush->b.Stops[index].Pos = position;
	brush->b.Stops[index].R   = c->r;
	brush->b.Stops[index].G   = c->g;
	brush->b.Stops[index].B   = c->b;
	brush->b.Stops[index].A   = c->a;

	RETURN_TRUE;	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_brush_methods[] = {
	PHP_ME(DrawBrush, __construct, php_ui_brush_construct_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, getType,     php_ui_brush_get_type_info,    ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, setType,     php_ui_brush_set_type_info,    ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, setColor,    php_ui_brush_set_color_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, getColor,    php_ui_brush_get_color_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, addStop,     php_ui_brush_add_stop_info,    ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, delStop,     php_ui_brush_del_stop_info,    ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, setStop,     php_ui_brush_set_stop_info,    ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawBrush) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw", "Brush", php_ui_brush_methods);

	uiDrawBrush_ce = zend_register_internal_class(&ce);
	uiDrawBrush_ce->create_object = php_ui_brush_create;

	zend_declare_class_constant_long(uiDrawBrush_ce, ZEND_STRL("Solid"), PHP_UI_BRUSH_SOLID);
	zend_declare_class_constant_long(uiDrawBrush_ce, ZEND_STRL("LinearGradient"), PHP_UI_BRUSH_LINEAR_GRADIENT);
	zend_declare_class_constant_long(uiDrawBrush_ce, ZEND_STRL("RadialGradient"), PHP_UI_BRUSH_RADIAL_GRADIENT);
	zend_declare_class_constant_long(uiDrawBrush_ce, ZEND_STRL("Image"), PHP_UI_BRUSH_IMAGE);

	memcpy(&php_ui_brush_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_brush_handlers.offset = XtOffsetOf(php_ui_brush_t, std);
	php_ui_brush_handlers.free_obj = php_ui_brush_free;

	return SUCCESS;
} /* }}} */
#endif
