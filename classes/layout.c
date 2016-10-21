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
#ifndef HAVE_PHP_UI_LAYOUT
#define HAVE_PHP_UI_LAYOUT
#include <ui.h>

#include "php.h"

#include <classes/descriptor.h>
#include <classes/layout.h>
#include <classes/font.h>
#include <classes/color.h>

zend_object_handlers php_ui_layout_handlers;

zend_object* php_ui_layout_create(zend_class_entry *ce) {
	php_ui_layout_t *layout = 
		(php_ui_layout_t*) ecalloc(1, sizeof(php_ui_layout_t) + zend_object_properties_size(ce));

	zend_object_std_init(&layout->std, ce);

	object_properties_init(&layout->std, ce);

	layout->std.handlers = &php_ui_layout_handlers;

	return &layout->std;
}

void php_ui_layout_free(zend_object *o) {
	php_ui_layout_t *layout = php_ui_layout_from(o);

	if (layout->l) {
		uiDrawFreeTextLayout(layout->l);
	}

	zend_object_std_dtor(o);
}
ZEND_BEGIN_ARG_INFO_EX(php_ui_layout_construct_info, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0,  font, UI\\Draw\\Text\\Font, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Layout UI\Draw\Text\Layout::__construct(string text, UI\Draw\Text\Font font, double width) */
PHP_METHOD(DrawTextLayout, __construct) 
{
	php_ui_layout_t *layout = php_ui_layout_fetch(getThis());
	zend_string *text = NULL;
	zval *font = NULL;
	double width = 0;
	php_ui_font_t *f;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SOd", &text, &font, uiDrawTextFont_ce, &width) != SUCCESS) {
		return;
	}

	f = php_ui_font_fetch(font);

	layout->l = uiDrawNewTextLayout(ZSTR_VAL(text), f->f, width);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_layout_set_width_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\Draw\Text\Layout::setWidth(double width) */
PHP_METHOD(DrawTextLayout, setWidth)
{
	php_ui_layout_t *layout = php_ui_layout_fetch(getThis());
	double width = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &width) != SUCCESS) {
		return;
	}

	uiDrawTextLayoutSetWidth(layout->l, width);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_layout_set_color_info, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, start, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, end, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, color, UI\\Draw\\Color, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\Draw\Text\Layout::setColor(int start, int end, UI\Draw\Color color) */
PHP_METHOD(DrawTextLayout, setColor)
{
	php_ui_layout_t *layout = php_ui_layout_fetch(getThis());
	zend_long start = 0, end = 0;
	zval *color = NULL;
	php_ui_color_t *c;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "llO", &start, &end, &color, uiDrawColor_ce) != SUCCESS) {
		return;
	}
	
	c = php_ui_color_fetch(color);

	uiDrawTextLayoutSetColor(layout->l, start, end, c->r, c->g, c->b, c->a);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_layout_methods[] = {
	PHP_ME(DrawTextLayout, __construct, php_ui_layout_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextLayout, setWidth, php_ui_layout_set_width_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextLayout, setColor, php_ui_layout_set_color_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawTextLayout) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text", "Layout", php_ui_layout_methods);

	uiDrawTextLayout_ce = zend_register_internal_class(&ce);
	uiDrawTextLayout_ce->create_object = php_ui_layout_create;

	memcpy(&php_ui_layout_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_layout_handlers.offset = XtOffsetOf(php_ui_layout_t, std);
	php_ui_layout_handlers.free_obj = php_ui_layout_free;
	
	return SUCCESS;
} /* }}} */
#endif
