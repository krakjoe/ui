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
#ifndef HAVE_PHP_UI_STROKE
#define HAVE_PHP_UI_STROKE
#include <ui.h>

#include "php.h"

#include <classes/stroke.h>

zend_object_handlers php_ui_stroke_handlers;

zend_object* php_ui_stroke_create(zend_class_entry *ce) {
	php_ui_stroke_t *stroke = 
		(php_ui_stroke_t*) ecalloc(1, sizeof(php_ui_stroke_t) + zend_object_properties_size(ce));

	zend_object_std_init(&stroke->std, ce);

	object_properties_init(&stroke->std, ce);

	stroke->std.handlers = &php_ui_stroke_handlers;

	return &stroke->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_stroke_construct_info, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, cap, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, join, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, thickness, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, miterLimit, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto DrawStroke DrawStroke::__construct(int cap, int join, double thickness, double miterLimit) */
PHP_METHOD(DrawStroke, __construct) 
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());
	zend_long cap = 0, join = 0;
	double thickness = 0, miterLimit = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lldd", &cap, &join, &thickness, &miterLimit) != SUCCESS) {
		return;
	}

	stroke->s.Cap = (uiDrawLineCap) cap;
	stroke->s.Join = (uiDrawLineJoin) join;
	stroke->s.Thickness = thickness;
	stroke->s.MiterLimit = miterLimit;
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_stroke_methods[] = {
	PHP_ME(DrawStroke, __construct, php_ui_stroke_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawStroke) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "DrawStroke", php_ui_stroke_methods);

	uiDrawStroke_ce = zend_register_internal_class(&ce);
	uiDrawStroke_ce->create_object = php_ui_stroke_create;

	zend_declare_class_constant_long(uiDrawStroke_ce, ZEND_STRL("CAP_FLAT"), PHP_UI_STROKE_CAP_FLAT);
	zend_declare_class_constant_long(uiDrawStroke_ce, ZEND_STRL("CAP_ROUND"), PHP_UI_STROKE_CAP_ROUND);
	zend_declare_class_constant_long(uiDrawStroke_ce, ZEND_STRL("CAP_SQUARE"), PHP_UI_STROKE_CAP_SQUARE);
	zend_declare_class_constant_long(uiDrawStroke_ce, ZEND_STRL("JOIN_MITER"), PHP_UI_STROKE_JOIN_MITER);
	zend_declare_class_constant_long(uiDrawStroke_ce, ZEND_STRL("JOIN_ROUND"), PHP_UI_STROKE_JOIN_ROUND);
	zend_declare_class_constant_long(uiDrawStroke_ce, ZEND_STRL("JOIN_BEVEL"), PHP_UI_STROKE_JOIN_BEVEL);

	memcpy(&php_ui_stroke_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_stroke_handlers.offset = XtOffsetOf(php_ui_stroke_t, std);

	return SUCCESS;
} /* }}} */
#endif
