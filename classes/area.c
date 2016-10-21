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
#ifndef HAVE_PHP_UI_AREA
#define HAVE_PHP_UI_AREA
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/point.h>
#include <classes/size.h>
#include <classes/area.h>
#include <classes/pen.h>

zend_object_handlers php_ui_area_handlers;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);

static void php_ui_area_draw(uiAreaHandler *handler, uiArea *_area, uiAreaDrawParams *p) {
	php_ui_area_t *area = 
		php_ui_area_from_handler(handler);
	
	int ret = 0;

	if (area->draw.fci.size) {
		zval rv;
		zval pen, areaSize, clipPoint, clipSize;

		ZVAL_UNDEF(&rv);
		area->draw.fci.retval = &rv;

		php_ui_pen_construct(&pen, p->Context);
		php_ui_size_construct(&areaSize, p->AreaWidth, p->AreaHeight);
		php_ui_point_construct(&clipPoint, p->ClipX, p->ClipY);
		php_ui_size_construct(&clipSize, p->ClipWidth, p->ClipHeight);

		zend_fcall_info_argn(&area->draw.fci, 4, &pen, &areaSize, &clipPoint, &clipSize);

		if (zend_call_function(&area->draw.fci, &area->draw.fcc) != SUCCESS) {
			return;
		}

		zend_fcall_info_args_clear(&area->draw.fci, 1);

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}

		zval_ptr_dtor(&pen);
		zval_ptr_dtor(&areaSize);
		zval_ptr_dtor(&clipPoint);
		zval_ptr_dtor(&clipSize);
	}
}

static void php_ui_area_mouse(uiAreaHandler *handler, uiArea *_area, uiAreaMouseEvent *e) {
	php_ui_area_t *area = 
		php_ui_area_from_handler(handler);

	int ret = 0;

	if (area->mouse.fci.size) {
		zval rv;
		zval areaPoint, areaSize, flags;
		zend_long modifiers = e->Modifiers;

		ZVAL_UNDEF(&rv);
		area->mouse.fci.retval = &rv;

		php_ui_point_construct(&areaPoint, e->X, e->Y);
		php_ui_size_construct(&areaSize, e->AreaWidth, e->AreaHeight);

		if (e->Down) {
			modifiers |= PHP_UI_AREA_DOWN;
		}

		if (e->Up) {
			modifiers |= PHP_UI_AREA_UP;
		}

		ZVAL_LONG(&flags, modifiers);

		zend_fcall_info_argn(&area->mouse.fci, 3, &areaPoint, &areaSize, &flags);

		if (zend_call_function(&area->mouse.fci, &area->mouse.fcc) != SUCCESS) {
			return;
		}

		zend_fcall_info_args_clear(&area->mouse.fci, 1);

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}

		zval_ptr_dtor(&areaPoint);
		zval_ptr_dtor(&areaSize);
	}
}

static int php_ui_area_key(uiAreaHandler *handler, uiArea *_area, uiAreaKeyEvent *e) {
	php_ui_area_t *area = 
		php_ui_area_from_handler(handler);

	int ret = 0;

	if (area->key.fci.size) {
		zval rv;
		zval key, ext, flags;
		zend_long modifiers = e->Modifiers;

		ZVAL_UNDEF(&rv);
		area->mouse.fci.retval = &rv;

		ZVAL_NEW_STR(&key, zend_string_init(&e->Key, sizeof(char), 0));
		ZVAL_LONG(&ext, e->ExtKey);

		if (e->Up) {
			modifiers |= PHP_UI_AREA_UP;
		} else {
			modifiers |= PHP_UI_AREA_DOWN;
		}

		ZVAL_LONG(&flags, modifiers);

		zend_fcall_info_argn(&area->key.fci, 3, &key, &ext, &flags);

		if (zend_call_function(&area->key.fci, &area->key.fcc) != SUCCESS) {
			return ret;
		}

		zend_fcall_info_args_clear(&area->key.fci, 1);

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}

	return ret;
}

static void php_ui_area_crossed(uiAreaHandler *ah, uiArea *_area, int left) {}
static void php_ui_area_drag(uiAreaHandler *ah, uiArea *a) {}

zend_object* php_ui_area_create(zend_class_entry *ce) {
	php_ui_area_t *area = 
		(php_ui_area_t*) ecalloc(1, sizeof(php_ui_area_t) + zend_object_properties_size(ce));

	zend_object_std_init(&area->std, ce);

	object_properties_init(&area->std, ce);

	area->std.handlers = &php_ui_area_handlers;

	area->h.Draw = php_ui_area_draw;
	area->h.MouseEvent = php_ui_area_mouse;
	area->h.KeyEvent = php_ui_area_key;

	area->h.MouseCrossed = php_ui_area_crossed;
	area->h.DragBroken = php_ui_area_drag;

	area->a = uiNewArea(&area->h);

	php_ui_set_call(&area->std, ZEND_STRL("ondraw"), &area->draw.fci, &area->draw.fcc);
	php_ui_set_call(&area->std, ZEND_STRL("onmouse"), &area->mouse.fci, &area->mouse.fcc);
	php_ui_set_call(&area->std, ZEND_STRL("onkey"), &area->key.fci, &area->key.fcc);

	return &area->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_area_redraw_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Area::redraw(void) */
PHP_METHOD(Area, redraw)
{
	php_ui_area_t *area = php_ui_area_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiAreaQueueRedrawAll(area->a);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_area_set_size_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, size, UI\\Size, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Area::setSize(Size size) */
PHP_METHOD(Area, setSize)
{
	php_ui_area_t *area = php_ui_area_fetch(getThis());
	zval *size = NULL;
	php_ui_size_t *s;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &size, uiSize_ce) != SUCCESS) {
		return;
	}
	
	s = php_ui_size_fetch(size);

	uiAreaSetSize(area->a, (int) s->width, (int) s->height);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_area_scroll_to_info, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, size, UI\\Size, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Area::scrollTo(Point point, Size size) */
PHP_METHOD(Area, scrollTo)
{
	php_ui_area_t *area = php_ui_area_fetch(getThis());
	zval *point = NULL, *size = NULL;
	php_ui_point_t *p;
	php_ui_size_t *s;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &point, uiPoint_ce, &size, uiSize_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);
	s = php_ui_size_fetch(size);

	uiAreaScrollTo(area->a, p->x, p->y, s->width, s->height);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_area_on_draw_info, 0, 0, 4)
	ZEND_ARG_OBJ_INFO(0, pen, UI\\Draw\\Pen, 0)
	ZEND_ARG_OBJ_INFO(0, areaSize, UI\\Size, 0)
	ZEND_ARG_OBJ_INFO(0, clipPoint, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, clipSize, UI\\Size, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Area::onDraw(UI\Draw\Pen pen, UI\Size areaSize, UI\Point clipPoint, UI\Size clipSize) */
PHP_METHOD(Area, onDraw)
{
	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_area_on_mouse_info, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0, areaPoint, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, areaSize, UI\\Size, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Area::onMouse(UI\Point areaPoint, UI\Size areaSize, int flags) */
PHP_METHOD(Area, onMouse)
{

} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_area_on_key_info, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, ext, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Area::onKey(string key, int ext, int flags) */
PHP_METHOD(Area, onKey)
{
	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_area_methods[] = {
	PHP_ME(Area, redraw,       php_ui_area_redraw_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Area, setSize,      php_ui_area_set_size_info,   ZEND_ACC_PUBLIC)
	PHP_ME(Area, scrollTo,     php_ui_area_scroll_to_info,  ZEND_ACC_PUBLIC)

	PHP_ME(Area, onDraw,       php_ui_area_on_draw_info,   ZEND_ACC_PROTECTED)
	PHP_ME(Area, onMouse,      php_ui_area_on_mouse_info,   ZEND_ACC_PROTECTED)
	PHP_ME(Area, onKey,        php_ui_area_on_key_info,   ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Area) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Area", php_ui_area_methods);

	uiArea_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiArea_ce->create_object = php_ui_area_create;

	zend_declare_class_constant_long(uiArea_ce, ZEND_STRL("CTRL"), PHP_UI_AREA_CTRL);
	zend_declare_class_constant_long(uiArea_ce, ZEND_STRL("ALT"), PHP_UI_AREA_ALT);
	zend_declare_class_constant_long(uiArea_ce, ZEND_STRL("SHIFT"), PHP_UI_AREA_SHIFT);
	zend_declare_class_constant_long(uiArea_ce, ZEND_STRL("SUPER"), PHP_UI_AREA_SUPER);
	zend_declare_class_constant_long(uiArea_ce, ZEND_STRL("DOWN"), PHP_UI_AREA_DOWN);
	zend_declare_class_constant_long(uiArea_ce, ZEND_STRL("UP"), PHP_UI_AREA_UP);

	memcpy(&php_ui_area_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_area_handlers.offset = XtOffsetOf(php_ui_area_t, std);
	
	return SUCCESS;
} /* }}} */
#endif
