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
#include <classes/context.h>

zend_object_handlers php_ui_area_handlers;

typedef struct php_ui_event_t {
	void *e;
	zend_object std;
} php_ui_event_t;

#define php_ui_event_fetch(o) ((php_ui_event_t*) ((char*) o - XtOffsetOf(php_ui_event_t, std)))

static int php_ui_area_event(php_ui_area_t *_area, zval *handler, zend_class_entry *aclass, void *a) {
	php_ui_area_t *area = (php_ui_area_t*) _area;
	int ret = 0;

	if (Z_TYPE_P(handler) != IS_UNDEF) {
		zval rv;
		zval arg;
		zval param;

		zend_fcall_info fci = empty_fcall_info;
		zend_fcall_info_cache fcc = empty_fcall_info_cache;
		
		ZVAL_UNDEF(&rv);
		ZVAL_UNDEF(&param);
		ZVAL_OBJ(&arg, &area->std);
		
		if (zend_fcall_info_init(handler, IS_CALLABLE_CHECK_SILENT, &fci, &fcc, NULL, NULL) != SUCCESS) {
			return;
		}

		fci.retval = &rv;
		zend_fcall_info_argn(&fci, 1, &arg);

		if (aclass && a) {
			php_ui_event_t *event;

			object_init_ex(&param, aclass);

			event = php_ui_event_fetch(&param);			
			event->e = a;
			
			zend_fcall_info_argn(&fci, 2, &arg, &param);
		} else {
			zend_fcall_info_argn(&fci, 1, &arg);
		}

		if (zend_call_function(&fci, &fcc) != SUCCESS) {
			return;
		}

		zend_fcall_info_args_clear(&fci, 1);

		if (Z_TYPE(rv) != IS_UNDEF) {
			if (Z_TYPE(rv) == IS_LONG) {
				ret = (int) zval_get_long(&rv);
			}

			zval_ptr_dtor(&rv);
		}
	}
	
	return ret;
}

static void php_ui_area_draw(uiAreaHandler *handler, uiArea *_area, uiAreaDrawParams *p) {
	php_ui_area_t *area = 
		php_ui_area_from_handler(handler);

	int ret = 0;

	if (Z_TYPE(area->draw) != IS_UNDEF) {
		zval rv;
		zval ctrl, context, areaSize, clipPoint, clipSize;
		php_ui_context_t *ctxt;

		zend_fcall_info fci = empty_fcall_info;
		zend_fcall_info_cache fcc = empty_fcall_info_cache;
		
		if (zend_fcall_info_init(&area->draw, IS_CALLABLE_CHECK_SILENT, &fci, &fcc, NULL, NULL) != SUCCESS) {
			return;
		}

		ZVAL_UNDEF(&rv);
		fci.retval = &rv;

		ZVAL_OBJ(&ctrl, &area->std);

		php_ui_context_construct(&context, p->Context);
		php_ui_size_construct(&areaSize, p->AreaWidth, p->AreaHeight);
		php_ui_point_construct(&clipPoint, p->ClipX, p->ClipY);
		php_ui_size_construct(&clipSize, p->ClipWidth, p->ClipHeight);

		zend_fcall_info_argn(&fci, 5, &ctrl, &context, &areaSize, &clipPoint, &clipSize);

		if (zend_call_function(&fci, &fcc) != SUCCESS) {
			return;
		}

		zend_fcall_info_args_clear(&fci, 1);

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}

		zval_ptr_dtor(&context);
		zval_ptr_dtor(&areaSize);
		zval_ptr_dtor(&clipPoint);
		zval_ptr_dtor(&clipSize);
	}
}

static void php_ui_area_mouse(uiAreaHandler *handler, uiArea *_area, uiAreaMouseEvent *e) {
	php_ui_area_t *area = 
		php_ui_area_from_handler(handler);

	int ret = 0;

	if (Z_TYPE(area->mouse) != IS_UNDEF) {
		zval rv;
		zval ctrl, areaPoint, areaSize, flags;
		php_ui_context_t *ctxt;
		zend_long modifiers = e->Modifiers;

		zend_fcall_info fci = empty_fcall_info;
		zend_fcall_info_cache fcc = empty_fcall_info_cache;

		if (zend_fcall_info_init(&area->mouse, IS_CALLABLE_CHECK_SILENT, &fci, &fcc, NULL, NULL) != SUCCESS) {
			return;
		}

		ZVAL_UNDEF(&rv);
		fci.retval = &rv;

		ZVAL_OBJ(&ctrl, &area->std);

		php_ui_point_construct(&areaPoint, e->X, e->Y);
		php_ui_size_construct(&areaSize, e->AreaWidth, e->AreaHeight);

		if (e->Down) {
			modifiers |= PHP_UI_AREA_DOWN;
		}

		if (e->Up) {
			modifiers |= PHP_UI_AREA_UP;
		}

		ZVAL_LONG(&flags, modifiers);

		zend_fcall_info_argn(&fci, 4, &ctrl, &areaPoint, &areaSize, &flags);

		if (zend_call_function(&fci, &fcc) != SUCCESS) {
			return;
		}

		zend_fcall_info_args_clear(&fci, 1);

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

	return php_ui_area_event(area, &area->key, NULL, e);
}

static void php_ui_area_crossed(uiAreaHandler *ah, uiArea *_area, int left) {}
static void php_ui_area_drag(uiAreaHandler *ah, uiArea *a) {}

zend_object* php_ui_area_create(zend_class_entry *ce) {
	php_ui_area_t *area = 
		(php_ui_area_t*) ecalloc(1, sizeof(php_ui_area_t));

	zend_object_std_init(&area->std, ce);

	object_properties_init(&area->std, ce);

	area->std.handlers = &php_ui_area_handlers;

	area->h.Draw = php_ui_area_draw;
	area->h.MouseEvent = php_ui_area_mouse;
	area->h.KeyEvent = php_ui_area_key;

	area->h.MouseCrossed = php_ui_area_crossed;
	area->h.DragBroken = php_ui_area_drag;

	ZVAL_UNDEF(&area->draw);
	ZVAL_UNDEF(&area->mouse);
	ZVAL_UNDEF(&area->key);

	return &area->std;
}

void php_ui_area_free(zend_object *o) {
	php_ui_area_t *area = php_ui_area_from(o);

	if (Z_TYPE(area->draw) != IS_UNDEF) {
		zval_ptr_dtor(&area->draw);
	}

	if (Z_TYPE(area->mouse) != IS_UNDEF) {
		zval_ptr_dtor(&area->mouse);
	}

	if (Z_TYPE(area->key) != IS_UNDEF) {
		zval_ptr_dtor(&area->key);
	}

	zend_object_std_dtor(o);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_area_construct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Area Area::__construct(void) */
PHP_METHOD(Area, __construct) 
{
	php_ui_area_t *area = php_ui_area_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	area->a = uiNewArea(&area->h);
} /* }}} */

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


ZEND_BEGIN_ARG_INFO_EX(php_ui_area_callable_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, handler, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Area::onDraw(callable handler) */
PHP_METHOD(Area, onDraw)
{
	php_ui_area_t *area = php_ui_area_fetch(getThis());
	zval *handler = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &handler) != SUCCESS) {
		return;
	}

	if (Z_TYPE(area->draw) != IS_UNDEF) {
		zval_ptr_dtor(&area->draw);
	}

	ZVAL_COPY(&area->draw, handler);
} /* }}} */

/* {{{ proto void Area::onMouse(callable handler) */
PHP_METHOD(Area, onMouse)
{
	php_ui_area_t *area = php_ui_area_fetch(getThis());
	zval *handler = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &handler) != SUCCESS) {
		return;
	}

	if (Z_TYPE(area->mouse) != IS_UNDEF) {
		zval_ptr_dtor(&area->mouse);
	}

	ZVAL_COPY(&area->mouse, handler);
} /* }}} */

/* {{{ proto void Area::onKey(callable handler) */
PHP_METHOD(Area, onKey)
{
	php_ui_area_t *area = php_ui_area_fetch(getThis());
	zval *handler = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &handler) != SUCCESS) {
		return;
	}

	if (Z_TYPE(area->key) != IS_UNDEF) {
		zval_ptr_dtor(&area->key);
	}

	ZVAL_COPY(&area->key, handler);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_area_methods[] = {
	PHP_ME(Area, __construct,  php_ui_area_construct_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Area, redraw,       php_ui_area_redraw_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Area, setSize,      php_ui_area_set_size_info,   ZEND_ACC_PUBLIC)
	PHP_ME(Area, scrollTo,     php_ui_area_scroll_to_info,  ZEND_ACC_PUBLIC)

	PHP_ME(Area, onDraw,       php_ui_area_callable_info,   ZEND_ACC_PUBLIC)
	PHP_ME(Area, onMouse,      php_ui_area_callable_info,   ZEND_ACC_PUBLIC)
	PHP_ME(Area, onKey,        php_ui_area_callable_info,   ZEND_ACC_PUBLIC)
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
	php_ui_area_handlers.free_obj = php_ui_area_free;
	
	return SUCCESS;
} /* }}} */
#endif
