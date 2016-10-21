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

#include <classes/control.h>
#include <classes/app.h>
#include <classes/window.h>

zend_object_handlers php_ui_app_handlers;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);

int php_ui_app_should_quit_handler(void *arg) {
	php_ui_app_t *app = (php_ui_app_t*) arg;
	zval rv;
	zval windows;
	int result = 1;

	if (!app->quit.fci.size) {
		uiQuit();

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

zend_object* php_ui_app_create(zend_class_entry *ce) {
	php_ui_app_t *app = 
		(php_ui_app_t*) ecalloc(1, sizeof(php_ui_app_t) + zend_object_properties_size(ce));

	zend_object_std_init(&app->std, ce);

	object_properties_init(&app->std, ce);

	app->std.handlers = &php_ui_app_handlers;

	zend_hash_init(&app->windows, 8, NULL, ZVAL_PTR_DTOR, 0);

	{
		const char *initError = NULL;

		memset(&app->options, 0, sizeof(uiInitOptions));

		initError = uiInit(&app->options);

		if (initError) {
			zend_error(E_ERROR,
				"Cannot initialize libui: %s", initError);
			uiFreeInitError(initError);

			return NULL;
		}

		uiMainSteps();
	}

	uiOnShouldQuit(php_ui_app_should_quit_handler, app);

	php_ui_set_call(&app->std, ZEND_STRL("onshouldquit"), &app->quit.fci, &app->quit.fcc);

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
	ZEND_ARG_TYPE_INFO(0, loop, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, block, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void App::run([bool loop = false, bool block = false])*/
PHP_METHOD(App, run)
{
	zend_bool loop = 0;
	zend_bool block = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|bb", &loop, &block) != SUCCESS) {
		return;
	}

	
	if (!loop) {
		uiMain();
		return;
	}
	
	uiMainStep(block);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_app_quit_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ */
PHP_METHOD(App, quit)
{
	uiQuit();
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_app_should_quit_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, windows, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void App::onShouldQuit(array windows) */
PHP_METHOD(App, onShouldQuit) 
{
	
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_app_methods[] = {
	PHP_ME(App, run,           php_ui_app_run_info,         ZEND_ACC_PUBLIC)
	PHP_ME(App, quit,          php_ui_app_quit_info,        ZEND_ACC_PUBLIC)
	PHP_ME(App, onShouldQuit,  php_ui_app_should_quit_info, ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_App) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "App", php_ui_app_methods);

	uiApp_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiApp_ce->create_object = php_ui_app_create;

	memcpy(&php_ui_app_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_app_handlers.offset = XtOffsetOf(php_ui_app_t, std);
	php_ui_app_handlers.free_obj = php_ui_app_free;

	return SUCCESS;
} /* }}} */
#endif
