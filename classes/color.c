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

#include <classes/exceptions.h>
#include <classes/control.h>
#include <classes/color.h>

zend_object_handlers php_std_handlers;
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
		
		*r = ((double) (uint8_t) ((components >> 24) & 0xFF)) / 255;
		*g = ((double) (uint8_t) ((components >> 16) & 0xFF)) / 255;
		*b = ((double) (uint8_t) ((components >> 8) & 0xFF)) / 255;
		*a = ((double) (uint8_t) (components & 0xFF)) / 255;

		return 1;
	}

	return 0;	
}

/* {{{ */
static zval* php_ui_color_read(zval *object, zval *member, int type, void **cache, zval *rv) {
	php_ui_color_t *color = php_ui_color_fetch(object);

	if (Z_TYPE_P(member) != IS_STRING) {
		goto php_std_read_color;
	}

	if (Z_STRLEN_P(member) != 1) {
		goto php_std_read_color;
	}

#define php_ui_color_guard() do { \
	if (type == BP_VAR_RW || type == BP_VAR_W) { \
		php_ui_exception( \
			"Failed to fetch reference to %s, not allowed", Z_STRVAL_P(member)); \
		return &EG(uninitialized_zval);	 \
	} \
} while(0)

	switch (Z_STRVAL_P(member)[0]) {
		case 'r':
		case 'R':
			php_ui_color_guard();
			ZVAL_DOUBLE(rv, color->r);
			return rv;

		case 'g':
		case 'G':
			php_ui_color_guard();
			ZVAL_DOUBLE(rv, color->g);
			return rv;

		case 'b':
		case 'B':
			php_ui_color_guard();
			ZVAL_DOUBLE(rv, color->b);
			return rv;

		case 'a':
		case 'A':
			php_ui_color_guard();
			ZVAL_DOUBLE(rv, color->a);
			return rv;
	}

#undef php_ui_color_guard

php_std_read_color:
	return php_std_handlers.read_property(object, member, type, cache, rv);
} /* }}} */

/* {{{ */
zval* php_ui_color_noref(zval *object, zval *member, int type, void **cache) {
	return NULL;
} /* }}} */

/* {{{ */
void php_ui_color_write(zval *object, zval *member, zval *value, void **cache) {
	php_ui_color_t *color = php_ui_color_fetch(object);

	if (Z_TYPE_P(member) != IS_STRING) {
		goto php_std_write_color;
	}

	if (Z_STRLEN_P(member) != 1) {
		goto php_std_write_color;
	}

	switch (Z_STRVAL_P(member)[0]) {
		case 'r':
		case 'R':
			color->r = zval_get_double(value);
			return;

		case 'g':
		case 'G':
			color->g = zval_get_double(value);
			return;

		case 'b':
		case 'B':
			color->b = zval_get_double(value);
			return;

		case 'a':
		case 'A':
			color->a = zval_get_double(value);
			return;
	}

php_std_write_color:
	php_std_handlers.write_property(object, member, value, cache);
} /* }}} */

zend_object* php_ui_color_create(zend_class_entry *ce) {
	php_ui_color_t *color = 
		(php_ui_color_t*) ecalloc(1, sizeof(php_ui_color_t) + zend_object_properties_size(ce));

	zend_object_std_init(&color->std, ce);

	object_properties_init(&color->std, ce);

	color->std.handlers = &php_ui_color_handlers;

	color->a = 1;

	return &color->std;
}

HashTable* php_ui_color_debug(zval *object, int *is_temp) {
	php_ui_color_t *color = php_ui_color_fetch(object);
	HashTable *table, *std;
	zval tmp;
	int is_std_temp = 0;

	*is_temp = 1;

	ALLOC_HASHTABLE(table);
	zend_hash_init(table, 4, NULL, ZVAL_PTR_DTOR, 0);
	
	std = php_std_handlers.get_debug_info(object, &is_std_temp);

	if (std) {
		zend_hash_merge(table, std, zval_add_ref, 1);

		if (is_std_temp) {
			zend_hash_destroy(std);
			FREE_HASHTABLE(std);
		}
	}

	ZVAL_DOUBLE(&tmp, color->r);
	zend_hash_str_update(table, "r", sizeof("r")-1, &tmp);

	ZVAL_DOUBLE(&tmp, color->g);
	zend_hash_str_update(table, "g", sizeof("g")-1, &tmp);

	ZVAL_DOUBLE(&tmp, color->b);
	zend_hash_str_update(table, "b", sizeof("b")-1, &tmp);

	ZVAL_DOUBLE(&tmp, color->a);
	zend_hash_str_update(table, "a", sizeof("a")-1, &tmp);

	return table;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_color_construct_info, 0, 0, 0)
	ZEND_ARG_INFO(0, rgba)
ZEND_END_ARG_INFO()

/* {{{ proto DrawColor DrawColor::__construct([int|double rgba = 0]) */
PHP_METHOD(DrawColor, __construct)
{
	php_ui_color_t *color = php_ui_color_fetch(getThis());
	zval *rgba = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|z", &rgba) != SUCCESS) {
		return;
	}

	if (ZEND_NUM_ARGS() > 0) {
		php_ui_color_set(rgba, 
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

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_color_get_channel_info, 0, 0, IS_DOUBLE, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_color_get_channel_info, 0, 0, IS_DOUBLE, NULL, 1)
#endif
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
	zend_declare_property_double(uiDrawColor_ce, ZEND_STRL("r"), 0.0, ZEND_ACC_PUBLIC);
	zend_declare_property_double(uiDrawColor_ce, ZEND_STRL("g"), 0.0, ZEND_ACC_PUBLIC);
	zend_declare_property_double(uiDrawColor_ce, ZEND_STRL("b"), 0.0, ZEND_ACC_PUBLIC);
	zend_declare_property_double(uiDrawColor_ce, ZEND_STRL("a"), 0.0, ZEND_ACC_PUBLIC);

	zend_declare_class_constant_long(uiDrawColor_ce, ZEND_STRL("Red"), PHP_UI_COLOR_RED);
	zend_declare_class_constant_long(uiDrawColor_ce, ZEND_STRL("Green"), PHP_UI_COLOR_GREEN);
	zend_declare_class_constant_long(uiDrawColor_ce, ZEND_STRL("Blue"), PHP_UI_COLOR_BLUE);
	zend_declare_class_constant_long(uiDrawColor_ce, ZEND_STRL("Alpha"), PHP_UI_COLOR_ALPHA);

	memcpy(&php_std_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	memcpy(&php_ui_color_handlers, &php_std_handlers, sizeof(zend_object_handlers));
	
	php_ui_color_handlers.offset = XtOffsetOf(php_ui_color_t, std);
	php_ui_color_handlers.get_debug_info = php_ui_color_debug;
	php_ui_color_handlers.read_property = php_ui_color_read;
	php_ui_color_handlers.get_property_ptr_ptr = php_ui_color_noref;
	php_ui_color_handlers.write_property = php_ui_color_write;

	return SUCCESS;
} /* }}} */
#endif
