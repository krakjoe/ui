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
#ifndef HAVE_PHP_UI_COLOR
#define HAVE_PHP_UI_COLOR
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/color.h>

zend_object_handlers php_ui_color_handlers;

zend_class_entry *uiDrawColor_ce;

zend_bool php_ui_color_set(zval *color, double *r, double *g, double *b, double *a) {

	if (Z_TYPE_P(color) == IS_OBJECT) {
		php_ui_color_t *object;

		if (!instanceof_function(Z_OBJCE_P(color), uiDrawColor_ce)) {
			return 0;
		}

		object  = php_ui_color_fetch(color);

		*r = object->r;
		*g = object->g;
		*b = object->b;
		*a = object->a;

		return 1;
	}

	if (Z_TYPE_P(color) == IS_LONG || Z_TYPE_P(color) == IS_DOUBLE) {
		uint32_t components = zval_get_double(color);
		
		*r = ((double) (uint8_t) ((components >> 16) & 0xFF)) / 255;
		*g = ((double) (uint8_t) ((components >> 8) & 0xFF)) / 255;
		*b = ((double) (uint8_t) (components & 0xFF)) / 255;
		*a = 1.0;

		return 1;
	}

	return 0;	
}

zend_object* php_ui_color_create(zend_class_entry *ce) {
	php_ui_color_t *color = 
		(php_ui_color_t*) ecalloc(1, sizeof(php_ui_color_t) + zend_object_properties_size(ce));

	zend_object_std_init(&color->std, ce);

	object_properties_init(&color->std, ce);

	color->std.handlers = &php_ui_color_handlers;

	color->a = 1;

	return &color->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_color_construct_info, 0, 0, 0)
	ZEND_ARG_INFO(0, rgb)
ZEND_END_ARG_INFO()

/* {{{ proto DrawColor DrawColor::__construct([int|double rgb = 0]) */
PHP_METHOD(DrawColor, __construct)
{
	php_ui_color_t *color = php_ui_color_fetch(getThis());
	zval *rgb = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|z", &rgb) != SUCCESS) {
		return;
	}

	if (ZEND_NUM_ARGS() > 0) {
		php_ui_color_set(rgb, 
			&color->r, &color->g, &color->b, &color->a);
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_color_set_channel_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, channel, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DrawColor::setChannel(int channel, double value) */
PHP_METHOD(DrawColor, setChannel) 
{
	php_ui_color_t *color = php_ui_color_fetch(getThis());
	zend_long channel = 0;
	double value = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ld", &channel, &value) != SUCCESS) {
		return;
	}

	switch (channel) {
		case PHP_UI_COLOR_RED: 
			color->r = value;
		break;

		case PHP_UI_COLOR_GREEN:
			color->g = value;
		break;

		case PHP_UI_COLOR_BLUE:
			color->b = value;
		break;

		case PHP_UI_COLOR_ALPHA:
			color->a = value;
		break;
	}
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_color_get_channel_info, 0, 0, IS_DOUBLE, NULL, 1)
	ZEND_ARG_TYPE_INFO(0, channel, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto double DrawColor::getChannel(int channel) */
PHP_METHOD(DrawColor, getChannel) 
{
	php_ui_color_t *color = php_ui_color_fetch(getThis());
	zend_long channel = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &channel) != SUCCESS) {
		return;
	}

	switch (channel) {
		case PHP_UI_COLOR_RED: 
			RETURN_DOUBLE(color->r);
		case PHP_UI_COLOR_GREEN:
			RETURN_DOUBLE(color->g);
		case PHP_UI_COLOR_BLUE:
			RETURN_DOUBLE(color->b);
		case PHP_UI_COLOR_ALPHA:
			RETURN_DOUBLE(color->a);
	}

	RETURN_DOUBLE(-1);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_color_methods[] = {
	PHP_ME(DrawColor, __construct,  php_ui_color_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawColor, setChannel,   php_ui_color_set_channel_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawColor, getChannel,   php_ui_color_get_channel_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawColor) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw", "Color", php_ui_color_methods);

	uiDrawColor_ce = zend_register_internal_class(&ce);
	uiDrawColor_ce->create_object = php_ui_color_create;

	zend_declare_class_constant_long(uiDrawColor_ce, ZEND_STRL("Red"), PHP_UI_COLOR_RED);
	zend_declare_class_constant_long(uiDrawColor_ce, ZEND_STRL("Green"), PHP_UI_COLOR_GREEN);
	zend_declare_class_constant_long(uiDrawColor_ce, ZEND_STRL("Blue"), PHP_UI_COLOR_BLUE);
	zend_declare_class_constant_long(uiDrawColor_ce, ZEND_STRL("Alpha"), PHP_UI_COLOR_ALPHA);

	memcpy(&php_ui_color_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_color_handlers.offset = XtOffsetOf(php_ui_color_t, std);

	return SUCCESS;
} /* }}} */
#endif
