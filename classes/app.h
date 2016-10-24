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
#ifndef HAVE_PHP_UI_APP_H
#define HAVE_PHP_UI_APP_H

extern zend_class_entry *uiApp_ce;

typedef struct _php_ui_app_t {
	HashTable windows;
	struct php_ui_app_quit_t {
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
	} quit;
	struct php_ui_app_tick_t {
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
	} tick;
	zend_long ticks;
	zend_object std;
} php_ui_app_t;

#define php_ui_app_from(o) ((php_ui_app_t*) ((char*) o - XtOffsetOf(php_ui_app_t, std)))
#define php_ui_app_fetch(z) php_ui_app_from(Z_OBJ_P(z))

void php_ui_app_window(zval *app, zval *window);

PHP_MINIT_FUNCTION(UI_App);
#endif
