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
#ifndef HAVE_PHP_UI_WINDOW
#define HAVE_PHP_UI_WINDOW
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/point.h>
#include <classes/size.h>
#include <classes/window.h>

zend_object_handlers php_ui_window_handlers;

extern void php_ui_set_controls(zend_object *std, const char *name, size_t nlength, HashTable *table);
extern zend_bool php_ui_set_parent(zval *child, zval *control);

zend_class_entry *uiWindow_ce;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);

zval *php_ui_window_construct(zval *object, uiWindow *w) {
	php_ui_window_t *win;

	object_init_ex(object, uiWindow_ce);

	win = php_ui_window_fetch(object);
	win->w = w;

	return object;
}

zend_object* php_ui_window_create(zend_class_entry *ce) {
	php_ui_window_t *w = (php_ui_window_t*) ecalloc(1, sizeof(php_ui_window_t) + zend_object_properties_size(ce));

	zend_object_std_init(&w->std, ce);

	object_properties_init(&w->std, ce);

	w->std.handlers = &php_ui_window_handlers;

	php_ui_set_call(&w->std, ZEND_STRL("onclosing"), &w->closing.fci, &w->closing.fcc);

	ALLOC_HASHTABLE(w->controls);

	zend_hash_init(w->controls, 8, NULL, ZVAL_PTR_DTOR, 0);

	php_ui_set_controls(&w->std, ZEND_STRL("controls"), w->controls);

	return &w->std;
}

int php_ui_window_closing_handler(uiWindow *w, void *arg) {
	php_ui_window_t *window = (php_ui_window_t *) arg;
	zval rv;
	int result = 0;

	if (!window->closing.fci.size) {
		uiControlDestroy(uiControl(window->w));
		uiQuit();

		return 0;
	}

	ZVAL_UNDEF(&rv);

	window->closing.fci.retval = &rv;

	if (zend_call_function(&window->closing.fci, &window->closing.fcc) != SUCCESS) {
		uiControlDestroy(uiControl(window->w));
		uiQuit();

		return 0;
	}

	if (Z_TYPE(rv) != IS_UNDEF) {
		result = 
			(int) zval_get_long(&rv);
		zval_ptr_dtor(&rv);
	}

	return result;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_window_construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, size, UI\\Size, 0)
	ZEND_ARG_TYPE_INFO(0, menu, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Window Window::__construct(string title, Size size, bool menu) */
PHP_METHOD(Window, __construct) 
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	zend_string *title = NULL;
	zval *size = NULL;
	php_ui_size_t *s;
	zend_bool menu = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SO|b", &title, &size, uiSize_ce, &menu) != SUCCESS) {
		return;
	}

	s = php_ui_size_fetch(size);

	win->w = uiNewWindow(ZSTR_VAL(title), 
		(int) s->width, (int) s->height, menu);

	uiWindowOnClosing(win->w, php_ui_window_closing_handler, win);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_window_set_title_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Window::setTitle(string title) */
PHP_METHOD(Window, setTitle)
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	zend_string *title = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &title) != SUCCESS) {
		return;
	}

	uiWindowSetTitle(win->w, ZSTR_VAL(title));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_window_get_title_info, 0, 0, IS_STRING, NULL, 0)	
ZEND_END_ARG_INFO()

/* {{{ proto string Window::getTitle(void) */
PHP_METHOD(Window, getTitle) 
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	char *title = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(uiWindowTitle(win->w));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_window_set_size_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, size, UI\\Size, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Window::setSize(Size size) */
PHP_METHOD(Window, setSize) 
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	zval *size = NULL;
	php_ui_size_t *s;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &size, uiSize_ce) != SUCCESS) {
		return;
	}

	if (!uiControlVisible(uiControl(win->w))) {
		return;
	}

	s = php_ui_size_fetch(size);

	uiWindowSetContentSize(win->w, (int) s->width, (int) s->height);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_window_get_size_info, 0, 0, IS_OBJECT, "UI\\Size", 0)	
ZEND_END_ARG_INFO()

/* {{{ proto Size Window::getSize(void) */
PHP_METHOD(Window, getSize) 
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	int width = 0, height = 0;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiWindowContentSize(win->w, &width, &height);

	php_ui_size_construct(return_value, (double) width, (double) height);	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_window_set_full_screen_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, full, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Window::setFullScreen(bool full) */
PHP_METHOD(Window, setFullScreen)
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	zend_bool full = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &full) != SUCCESS) {
		return;
	}

	uiWindowSetFullscreen(win->w, full);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_window_is_full_screen_info, 0, 0, _IS_BOOL, NULL, 0)	
ZEND_END_ARG_INFO()

/* {{{ proto bool Window::isFullScreen(void) */
PHP_METHOD(Window, isFullScreen) 
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;	
	}

	if (uiWindowFullscreen(win->w)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_window_set_borders_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, borders, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Window::setBorders(bool borders) */
PHP_METHOD(Window, setBorders)
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	zend_bool borders = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &borders) != SUCCESS) {
		return;
	}

	uiWindowSetBorderless(win->w, !borders);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_window_has_borders_info, 0, 0, _IS_BOOL, NULL, 0)	
ZEND_END_ARG_INFO()

/* {{{ proto bool Window::hasBorders(void) */
PHP_METHOD(Window, hasBorders)
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;	
	}

	if (uiWindowBorderless(win->w)) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_window_set_margin_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, margin, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Window::setMargin(bool margin) */
PHP_METHOD(Window, setMargin)
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	zend_bool margin = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &margin) != SUCCESS) {
		return;
	}

	uiWindowSetMargined(win->w, margin);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_window_has_margin_info, 0, 0, _IS_BOOL, NULL, 0)	
ZEND_END_ARG_INFO()

/* {{{ proto bool Window::hasMargin(void) */
PHP_METHOD(Window, hasMargin)
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;	
	}

	if (uiWindowMargined(win->w)) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
} /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_window_add_info, 0, 0, 4)
	ZEND_ARG_OBJ_INFO(0, control, UI\\Control, 0)
ZEND_END_ARG_INFO()

/* {{{ */
PHP_METHOD(Window, add)
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	php_ui_control_t *ctrl;

	zval *control = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &control, uiControl_ce) != SUCCESS) {
		return;
	}

	ctrl = php_ui_control_fetch(control);

	if (!php_ui_set_parent(control, getThis())) {
		return;
	}

	uiWindowSetChild(win->w, ctrl->control);

	if (zend_hash_next_index_insert(win->controls, control)) {
		Z_ADDREF_P(control);
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_window_box_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, title,  IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, msg,    IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Window::msg(string title, string msg) */
PHP_METHOD(Window, msg) 
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	zend_string *title = NULL, *msg = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SS", &title, &msg) != SUCCESS) {
		return;
	}

	uiMsgBox(win->w, ZSTR_VAL(title), ZSTR_VAL(msg));
} /* }}} */

/* {{{ proto void Window::error(string title, string msg) */
PHP_METHOD(Window, error) 
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	zend_string *title = NULL, *msg = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SS", &title, &msg) != SUCCESS) {
		return;
	}

	uiMsgBoxError(win->w, ZSTR_VAL(title), ZSTR_VAL(msg));
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_window_dialog_info, 0, 0, IS_STRING, NULL, 1)	
ZEND_END_ARG_INFO()

/* {{{ proto string Window::open(void) */
PHP_METHOD(Window, open) 
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	char *open = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	open = uiOpenFile(win->w);

	if (!open) {
		return;
	}

	RETURN_STRING(open);
} /* }}} */

/* {{{ proto string Window::save(void) */
PHP_METHOD(Window, save) 
{
	php_ui_window_t *win = php_ui_window_fetch(getThis());
	char *save = NULL;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	save = uiSaveFile(win->w);

	if (!save) {
		return;
	}

	RETURN_STRING(save);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_window_closing_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Window, onClosing) {}

/* {{{ */
const zend_function_entry php_ui_window_methods[] = {
	PHP_ME(Window, __construct,    php_ui_window_construct_info,       ZEND_ACC_PUBLIC)
	PHP_ME(Window, setTitle,       php_ui_window_set_title_info,       ZEND_ACC_PUBLIC)
	PHP_ME(Window, getTitle,       php_ui_window_get_title_info,       ZEND_ACC_PUBLIC)
	PHP_ME(Window, setSize,        php_ui_window_set_size_info,        ZEND_ACC_PUBLIC)
	PHP_ME(Window, getSize,        php_ui_window_get_size_info,        ZEND_ACC_PUBLIC)
	PHP_ME(Window, setFullScreen,  php_ui_window_set_full_screen_info, ZEND_ACC_PUBLIC)
	PHP_ME(Window, isFullScreen,   php_ui_window_is_full_screen_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Window, setBorders,     php_ui_window_set_borders_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Window, hasBorders,     php_ui_window_has_borders_info,     ZEND_ACC_PUBLIC)
	PHP_ME(Window, setMargin,      php_ui_window_set_margin_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Window, hasMargin,      php_ui_window_has_margin_info,      ZEND_ACC_PUBLIC)
	PHP_ME(Window, add,            php_ui_window_add_info,             ZEND_ACC_PUBLIC)
	PHP_ME(Window, msg,            php_ui_window_box_info,             ZEND_ACC_PUBLIC)
	PHP_ME(Window, error,          php_ui_window_box_info,             ZEND_ACC_PUBLIC)
	PHP_ME(Window, open,           php_ui_window_dialog_info,          ZEND_ACC_PUBLIC)
	PHP_ME(Window, save,           php_ui_window_dialog_info,          ZEND_ACC_PUBLIC)
	PHP_ME(Window, onClosing,      php_ui_window_closing_info,         ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Window) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Window", php_ui_window_methods);

	uiWindow_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiWindow_ce->create_object = php_ui_window_create;
	zend_declare_property_null(uiWindow_ce, ZEND_STRL("controls"), ZEND_ACC_PROTECTED);

	memcpy(&php_ui_window_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_window_handlers.offset = XtOffsetOf(php_ui_window_t, std);

	return SUCCESS;
} /* }}} */
#endif
