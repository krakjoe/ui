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

#include <classes/control.h>
#include <classes/brush.h>

zend_object_handlers php_ui_brush_handlers;

static inline uiDrawBrushType php_ui_brush_type(zend_long type) {
	switch (type) {
		case PHP_UI_BRUSH_SOLID: return uiDrawBrushTypeSolid;
		case PHP_UI_BRUSH_LINEAR_GRADIENT: return uiDrawBrushTypeLinearGradient;
		case PHP_UI_BRUSH_RADIAL_GRADIENT: uiDrawBrushTypeRadialGradient;
		case PHP_UI_BRUSH_IMAGE: return uiDrawBrushTypeImage;
	}
	return uiDrawBrushTypeSolid;
}

zend_object* php_ui_brush_create(zend_class_entry *ce) {
	php_ui_brush_t *brush = 
		(php_ui_brush_t*) ecalloc(1, sizeof(php_ui_brush_t));

	zend_object_std_init(&brush->std, ce);

	object_properties_init(&brush->std, ce);

	brush->std.handlers = &php_ui_brush_handlers;

	return &brush->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_brush_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, r, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, X0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, Y0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, X1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, radius, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto DrawBrush DrawBrush::__construct(int type, double r, double r, double b, double a [, double X0, double, Y0, double X1, double Y1, double radius]) */
PHP_METHOD(DrawBrush, __construct) 
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zend_long type = PHP_UI_BRUSH_SOLID;
	double r = 0, g = 0, b = 0, a = 0, X0 = 0, Y0 = 0, X1 = 0, Y1 = 0, radius = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l|ddddddddd", &type, &r, &g, &b, &a, &X0, &Y0, &X1, &Y1, &radius) != SUCCESS) {
		return;
	}

	brush->b.Type = php_ui_brush_type(type);
	brush->b.R = r;
	brush->b.G = g;
	brush->b.B = b;
	brush->b.A = a;
	brush->b.X0 = X0;
	brush->b.Y0 = Y0;
	brush->b.X1 = X1;
	brush->b.Y1 = Y1;
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_brush_set_rgb_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, color, IS_LONG, 0)
ZEND_END_ARG_INFO()

static inline void php_ui_brush_rgb(uiDrawBrush *brush, uint32_t color)
{
	uint8_t component;

	component = (uint8_t) ((color >> 16) & 0xFF);
	brush->R = ((double) component) / 255;
	component = (uint8_t) ((color >> 8) & 0xFF);
	brush->G = ((double) component) / 255;
	component = (uint8_t) (color & 0xFF);
	brush->B = ((double) component) / 255;
}

/* {{{ proto void DrawBrush::setRGB(int color) */
PHP_METHOD(DrawBrush, setRGB)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	zend_long color = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &color) != SUCCESS) {
		return;
	}

	php_ui_brush_rgb(&brush->b, (uint32_t) color);	
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_brush_get_alpha_info, 0, 0, IS_DOUBLE, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto double DrawBrush::getAlpha(void) */
PHP_METHOD(DrawBrush, getAlpha)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(brush->b.A);
} /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_brush_set_alpha_info, 0, 0, 1)

ZEND_END_ARG_INFO()

/* {{{ proto void DrawBrush::setAlpha(double alpha) */
PHP_METHOD(DrawBrush, setAlpha)
{
	php_ui_brush_t *brush = php_ui_brush_fetch(getThis());
	double alpha = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &alpha) != SUCCESS) {
		return;
	}

	brush->b.A = alpha;
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

/* {{{ */
const zend_function_entry php_ui_brush_methods[] = {
	PHP_ME(DrawBrush, __construct, php_ui_brush_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, getType,     php_ui_brush_get_type_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, setRGB,      php_ui_brush_set_rgb_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, setAlpha,    php_ui_brush_set_alpha_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawBrush, getAlpha,    php_ui_brush_get_alpha_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawBrush) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "DrawBrush", php_ui_brush_methods);

	uiDrawBrush_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiDrawBrush_ce->create_object = php_ui_brush_create;

	zend_declare_class_constant_long(uiDrawBrush_ce, ZEND_STRL("SOLID"), PHP_UI_BRUSH_SOLID);
	zend_declare_class_constant_long(uiDrawBrush_ce, ZEND_STRL("LGRADIENT"), PHP_UI_BRUSH_LINEAR_GRADIENT);
	zend_declare_class_constant_long(uiDrawBrush_ce, ZEND_STRL("RGRADIENT"), PHP_UI_BRUSH_RADIAL_GRADIENT);
	zend_declare_class_constant_long(uiDrawBrush_ce, ZEND_STRL("IMAGE"), PHP_UI_BRUSH_IMAGE);

	memcpy(&php_ui_brush_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_brush_handlers.offset = XtOffsetOf(php_ui_brush_t, std);

	return SUCCESS;
} /* }}} */
#endif
