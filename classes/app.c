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
#ifndef HAVE_PHP_UI_APP
#define HAVE_PHP_UI_APP
#include <ui.h>

#include "php.h"

#include <classes/app.h>
#include <classes/window.h>

zend_class_entry *uiApp_ce;

zend_object_handlers php_ui_app_handlers;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);

int php_ui_app_should_quit_handler(void *arg) {
	php_ui_app_t *app = (php_ui_app_t*) arg;
	zval rv;
	zval windows;
	int result = 1;

	if (!app->quit.fci.size) {
		zval *_window = NULL;

		ZEND_HASH_FOREACH_VAL(&app->windows, _window) {
			php_ui_window_t *window = 
				php_ui_window_fetch(_window);
			uiControlDestroy(uiControl(window->w));
		} ZEND_HASH_FOREACH_END();

		zend_hash_clean(&app->windows);

		return result;
	}

	ZVAL_UNDEF(&rv);
	ZVAL_ARR(&windows, &app->windows);

	app->quit.fci.retval = &rv;

	zend_fcall_info_argn(&app->quit.fci, 1, &windows);

	if (zend_call_function(&app->quit.fci, &app->quit.fcc) != SUCCESS) {
		return 1;
	}

	zend_fcall_info_args_clear(&app->quit.fci, 1);

	if (Z_TYPE(rv) != IS_UNDEF) {
		result = 
			(int) zval_get_long(&rv);
		zval_ptr_dtor(&rv);
	}

	return result;
}

void php_ui_app_tick_handler(void *o) {
	php_ui_app_t *app = (php_ui_app_t *) o;
	zval rv;

	if (!app->tick.fci.size) {
		return;
	}

	ZVAL_UNDEF(&rv);
	app->tick.fci.retval = &rv;

	if (zend_call_function(&app->tick.fci, &app->tick.fcc) != SUCCESS) {
		return;
	}

	if (Z_TYPE(rv) != IS_UNDEF) {
		zval_ptr_dtor(&rv);
	}

	app->ticks--;
}

zend_object* php_ui_app_create(zend_class_entry *ce) {
	php_ui_app_t *app = 
		(php_ui_app_t*) ecalloc(1, sizeof(php_ui_app_t) + zend_object_properties_size(ce));

	zend_object_std_init(&app->std, ce);

	object_properties_init(&app->std, ce);

	app->std.handlers = &php_ui_app_handlers;

	zend_hash_init(&app->windows, 8, NULL, ZVAL_PTR_DTOR, 0);

	uiOnShouldQuit(php_ui_app_should_quit_handler, app);

	php_ui_set_call(&app->std, ZEND_STRL("onshouldquit"), &app->quit.fci, &app->quit.fcc);
	php_ui_set_call(&app->std, ZEND_STRL("ontick"), &app->tick.fci, &app->tick.fcc);

	return &app->std;
}

void php_ui_app_window(zval *_app, zval *window) {
	php_ui_app_t *app = php_ui_app_fetch(_app);

	if (zend_hash_next_index_insert(&app->windows, window)) {
		zval_copy_ctor(window);
	}
}

void php_ui_app_free(zend_object *o) {
	php_ui_app_t *app = php_ui_app_from(o);

	zend_hash_destroy(&app->windows);	

	zend_object_std_dtor(o);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_app_run_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void App::run([int flags = 0])*/
PHP_METHOD(App, run)
{
	php_ui_app_t *app = php_ui_app_fetch(getThis());
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &flags) != SUCCESS) {
		return;
	}
	
	if (!(flags & PHP_UI_APP_LOOP)) {
		uiMain();
		return;
	}

	if (app->tick.fci.size && !app->ticks) {
		uiQueueMain(
			php_ui_app_tick_handler, app);
		app->ticks++;
	}
	
	uiMainStep((flags & PHP_UI_APP_WAIT) == PHP_UI_APP_WAIT);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_app_quit_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ */
PHP_METHOD(App, quit)
{
	php_ui_app_t *app = php_ui_app_fetch(getThis());

	if (app->ticks) {
		uiMainStep(0);
	}

	uiQuit();
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_app_should_quit_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, windows, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void App::onShouldQuit(array windows) */
PHP_METHOD(App, onShouldQuit) 
{
	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_app_tick_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void App::onTick() */
PHP_METHOD(App, onTick) 
{
	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_app_methods[] = {
	PHP_ME(App, run,           php_ui_app_run_info,         ZEND_ACC_PUBLIC)
	PHP_ME(App, quit,          php_ui_app_quit_info,        ZEND_ACC_PUBLIC)
	PHP_ME(App, onTick,        php_ui_app_tick_info,        ZEND_ACC_PROTECTED)
	PHP_ME(App, onShouldQuit,  php_ui_app_should_quit_info, ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_App) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "App", php_ui_app_methods);

	uiApp_ce = zend_register_internal_class(&ce);
	uiApp_ce->create_object = php_ui_app_create;

	zend_declare_class_constant_long(uiApp_ce, ZEND_STRL("Loop"), PHP_UI_APP_LOOP);
	zend_declare_class_constant_long(uiApp_ce, ZEND_STRL("Wait"), PHP_UI_APP_WAIT);

	memcpy(&php_ui_app_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_app_handlers.offset = XtOffsetOf(php_ui_app_t, std);
	php_ui_app_handlers.free_obj = php_ui_app_free;

	return SUCCESS;
} /* }}} */
#endif
