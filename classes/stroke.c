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

zend_class_entry *uiDrawStroke_ce;
zend_class_entry *uiDrawLineCap_ce;
zend_class_entry *uiDrawLineJoin_ce;

zend_object* php_ui_stroke_create(zend_class_entry *ce) {
	php_ui_stroke_t *stroke = 
		(php_ui_stroke_t*) ecalloc(1, sizeof(php_ui_stroke_t) + zend_object_properties_size(ce));

	zend_object_std_init(&stroke->std, ce);

	object_properties_init(&stroke->std, ce);

	stroke->std.handlers = &php_ui_stroke_handlers;

	return &stroke->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_stroke_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, cap, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, join, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, thickness, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, miterLimit, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Stroke UI\Draw\Stroke::__construct([int cap = UI\Draw\Line\Cap::Flat, int join = UI\Draw\Line\Join::Miter, double thickness = 1, double miterLimit = 10]) */
PHP_METHOD(DrawStroke, __construct) 
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());
	zend_long cap = uiDrawLineCapFlat, 
					join = uiDrawLineJoinMiter;
	double thickness = 1, miterLimit = 10;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|lldd", &cap, &join, &thickness, &miterLimit) != SUCCESS) {
		return;
	}

	stroke->s.Cap = (uiDrawLineCap) cap;
	stroke->s.Join = (uiDrawLineJoin) join;
	stroke->s.Thickness = thickness;
	stroke->s.MiterLimit = miterLimit;
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_stroke_set_cap_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, cap, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\Draw\Stroke::setCap(int cap) */
PHP_METHOD(DrawStroke, setCap)
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());
	zend_long cap = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &cap) != SUCCESS) {
		return;
	}

	stroke->s.Cap = (uiDrawLineCap) cap;
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_stroke_get_cap_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int UI\Draw\Stroke::getCap(void) */
PHP_METHOD(DrawStroke, getCap) 
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG((zend_long) stroke->s.Cap);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_stroke_set_join_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, join, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\Draw\Stroke::setJoin(int join) */
PHP_METHOD(DrawStroke, setJoin)
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());
	zend_long join = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &join) != SUCCESS) {
		return;
	}

	stroke->s.Join = (uiDrawLineJoin) join;
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_stroke_get_join_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int UI\Draw\Stroke::getJoin(void) */
PHP_METHOD(DrawStroke, getJoin) 
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG((zend_long) stroke->s.Join);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_stroke_set_thickness_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, thickness, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\Draw\Stroke::setThickness(double thickness) */
PHP_METHOD(DrawStroke, setThickness)
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());
	double thickness = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &thickness) != SUCCESS) {
		return;
	}

	stroke->s.Thickness = thickness;
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_stroke_get_thickness_info, 0, 0, IS_DOUBLE, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int UI\Draw\Stroke::getThickness(void) */
PHP_METHOD(DrawStroke, getThickness) 
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(stroke->s.Thickness);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_stroke_set_miter_limit_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, limit, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\Draw\Stroke::setMiterLimit(double limit) */
PHP_METHOD(DrawStroke, setMiterLimit)
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());
	double limit = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &limit) != SUCCESS) {
		return;
	}

	stroke->s.MiterLimit = limit;
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_stroke_get_miter_limit_info, 0, 0, IS_DOUBLE, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int UI\Draw\Stroke::getMiterLimit(void) */
PHP_METHOD(DrawStroke, getMiterLimit) 
{
	php_ui_stroke_t *stroke = php_ui_stroke_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(stroke->s.MiterLimit);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_stroke_methods[] = {
	PHP_ME(DrawStroke, __construct, php_ui_stroke_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawStroke, setCap,      php_ui_stroke_set_cap_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawStroke, getCap,      php_ui_stroke_get_cap_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawStroke, setJoin,     php_ui_stroke_set_join_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawStroke, getJoin,     php_ui_stroke_get_join_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawStroke, setThickness,     php_ui_stroke_set_thickness_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawStroke, getThickness,     php_ui_stroke_get_thickness_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawStroke, getMiterLimit,    php_ui_stroke_get_miter_limit_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawStroke, setMiterLimit,    php_ui_stroke_set_miter_limit_info,   ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawStroke) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw", "Stroke", php_ui_stroke_methods);

	uiDrawStroke_ce = zend_register_internal_class(&ce);
	uiDrawStroke_ce->create_object = php_ui_stroke_create;

	memcpy(&php_ui_stroke_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_stroke_handlers.offset = XtOffsetOf(php_ui_stroke_t, std);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Line", "Cap", NULL);
	
	uiDrawLineCap_ce = zend_register_internal_class(&ce);
	uiDrawLineCap_ce->ce_flags |= ZEND_ACC_FINAL;

#define php_ui_register_cap(c, n) zend_declare_class_constant_long(c, ZEND_STRL(#n), uiDrawLineCap##n)
	php_ui_register_cap(uiDrawLineCap_ce, Flat);
	php_ui_register_cap(uiDrawLineCap_ce, Round);
	php_ui_register_cap(uiDrawLineCap_ce, Square);
#undef php_ui_register_cap

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Line", "Join", NULL);

	uiDrawLineJoin_ce = zend_register_internal_class(&ce);
	uiDrawLineJoin_ce->ce_flags |= ZEND_ACC_FINAL;

#define php_ui_register_join(c, n) zend_declare_class_constant_long(c, ZEND_STRL(#n), uiDrawLineJoin##n)
	php_ui_register_join(uiDrawLineJoin_ce, Miter);
	php_ui_register_join(uiDrawLineJoin_ce, Round);
	php_ui_register_join(uiDrawLineJoin_ce, Bevel);
#undef php_ui_register_join

	return SUCCESS;
} /* }}} */
#endif
